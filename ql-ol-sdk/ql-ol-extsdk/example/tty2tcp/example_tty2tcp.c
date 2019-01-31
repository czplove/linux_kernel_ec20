#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <stddef.h>
//#include <linux/in.h>
#include <netinet/in.h>
#include <linux/un.h>

#ifdef RIL_SHLIB
#define LOG_TAG "tty2tcp"
#include "ql-log.h"
#define dprintf LOGD
#else
#define dprintf(fmt, arg...) do { printf(fmt, ##arg); } while(0)
#endif

char *inet_ntoa(struct in_addr in);
unsigned int inet_addr(const char *cp);
int    ptsname_r( int  fd, char*  buf, size_t  buflen);
int grantpt(int fd);
int  unlockpt( int  fd );

#define min(x,y) (((x) < (y)) ? (x) : (y))
#define SYSCHECK(c) do{if((c)<0) {dprintf("%s %d error: '%s' (code: %d)\n", __func__, __LINE__, strerror(errno), errno); return -1;}}while(0)
#define cfmakenoblock(fd) do{fcntl(fd, F_SETFL, fcntl(fd,F_GETFL) | O_NONBLOCK);}while(0)
static int s_quit_flag = 0;

static void cfmakeserail(int ttyfd) {
    struct termios  ios;

    /* disable echo on serial ports */
    memset(&ios, 0, sizeof(ios));
    tcgetattr( ttyfd, &ios );
    cfmakeraw(&ios);
    ios.c_lflag = 0;  /* disable ECHO, ICANON, etc... */
    cfsetispeed(&ios, B115200);
    cfsetospeed(&ios, B115200);
    tcsetattr( ttyfd, TCSANOW, &ios );
    tcflush(ttyfd, TCIOFLUSH);
}

static int open_serial(const char *tty_port) {
    int ttyfd;

    SYSCHECK(ttyfd = open (tty_port, O_RDWR | O_NDELAY));
    cfmakeserail(ttyfd);
    dprintf("open %s ttyfd = %d\n", tty_port, ttyfd);

    return ttyfd;
}

static int open_pts(const char *pts_name) {
    int ptsfd;
    char pts_r[64] = {0};
    char* pts = NULL;
    char command[128];

    sprintf(command,"rm %s -f", pts_name);
    system(command);
    SYSCHECK(ptsfd = open ("/dev/ptmx", O_RDWR | O_NDELAY));

    if (ptsname_r(ptsfd, pts_r, sizeof(pts_r)) == 0)
        pts = pts_r;

    if (pts == NULL) SYSCHECK(-1);

    if(symlink(pts, pts_name) != 0) {
        dprintf("Create link %s Error : %d (%s)", pts_name, errno, strerror(errno));
        return -1;
    }

    cfmakeserail(ptsfd);

    //Otherwise programs cannot access the pseudo terminals
    SYSCHECK(grantpt(ptsfd));
    SYSCHECK(unlockpt(ptsfd));

    dprintf("open %s -> %s ptsfd = %d\n", pts_name, pts_r, ptsfd);

    return ptsfd;
}

struct __kfifo {
    unsigned int    in;
    unsigned int    out;
    unsigned int    mask;
    void        *data;
};

int __kfifo_alloc(struct __kfifo *fifo, unsigned int size) {
    /*
     * round down to the next power of 2, since our 'let the indices
     * wrap' technique works only in this case.
     */
    //size = roundup_pow_of_two(size);

    fifo->in = 0;
    fifo->out = 0;

    if (size < 2) {
        fifo->data = NULL;
        fifo->mask = 0;
        return -EINVAL;
    }

    fifo->data = malloc(size);

    if (!fifo->data) {
        fifo->mask = 0;
        return -ENOMEM;
    }
    fifo->mask = size - 1;

    return 0;
}

unsigned int kfifo_used(struct __kfifo *fifo, void **pp_buf) {
    unsigned int off = fifo->out & fifo->mask;
    unsigned int len = min(fifo->in - fifo->out, (fifo->mask + 1) - off);

    if (pp_buf)
        *pp_buf = len ? (fifo->data + off) : NULL;

    return len;
}

unsigned int kfifo_unused(struct __kfifo *fifo, void **pp_buf) {
    unsigned int off = fifo->in & fifo->mask;
    unsigned int len = min((fifo->mask + 1) - (fifo->in - fifo->out), (fifo->mask + 1) - off);

    if (pp_buf)
        *pp_buf = len ? (fifo->data + off) : NULL;

    return len;
}

void __kfifo_in(struct __kfifo *fifo, unsigned len) {
    fifo->in += len;
}

void __kfifo_out(struct __kfifo *fifo, unsigned len) {
        fifo->out += len;
}

void __kfifo_free(struct __kfifo *fifo) {
    free(fifo->data);
    fifo->in = 0;
    fifo->out = 0;
    fifo->data = NULL;
    fifo->mask = 0;
}

void __kfifo_reset(struct __kfifo *fifo) {
    fifo->in = 0;
    fifo->out = 0;
}

static int create_local_server(const char *name) {
    int sockfd = -1;
    int reuse_addr = 1;
    struct sockaddr_un sockaddr;
    socklen_t alen;

    /*Create server socket*/
    SYSCHECK(sockfd = socket(AF_LOCAL, SOCK_STREAM, 0));

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sun_family = AF_LOCAL;
    sockaddr.sun_path[0] = 0;
    memcpy(sockaddr.sun_path + 1, name, strlen(name) );

    alen = strlen(name) + offsetof(struct sockaddr_un, sun_path) + 1;
    if(bind(sockfd, (struct sockaddr *)&sockaddr, alen) < 0) {
        close(sockfd);
        dprintf("%s bind %s errno: %d (%s)\n", __func__, name, errno, strerror(errno));
        return -1;
    }
    SYSCHECK(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,sizeof(reuse_addr)));

    dprintf("local server: %s sockfd = %d\n", name, sockfd);

    return sockfd;
}

static int create_tcp_server(int socket_port)
{
    int sockfd = -1;
    int reuse_addr = 1;
    struct sockaddr_in sockaddr;

    /*Create server socket*/
    SYSCHECK(sockfd = socket(AF_INET, SOCK_STREAM, 0));

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(socket_port);

    if(bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
        close(sockfd);
        dprintf("%s bind %d errno: %d (%s)\n", __func__, socket_port, errno, strerror(errno));
        return -1;
    }
    SYSCHECK(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,sizeof(reuse_addr)));

    dprintf("tcp server: %d sockfd = %d\n", socket_port, sockfd);

    return sockfd;
}

static int connect_local_server(const char *name) {
    int sockfd = -1;
    int reuse_addr = 1;
    struct sockaddr_un sockaddr;
    socklen_t alen;

    /*Create server socket*/
    SYSCHECK(sockfd = socket(AF_LOCAL, SOCK_STREAM, 0));

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sun_family = AF_LOCAL;
    sockaddr.sun_path[0] = 0;
    memcpy(sockaddr.sun_path + 1, name, strlen(name) );

    alen = strlen(name) + offsetof(struct sockaddr_un, sun_path) + 1;
    if(connect(sockfd, (struct sockaddr *)&sockaddr, alen) < 0) {
        close(sockfd);
        dprintf("%s connect %s errno: %d (%s)\n", __func__, name, errno, strerror(errno));
        return -1;
    }
    SYSCHECK(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,sizeof(reuse_addr)));

    dprintf("local client: %s sockfd = %d\n", name, sockfd);

    return sockfd;
}

static int connect_tcp_server(const char *tcp_host, int tcp_port)
{
    int sockfd = -1;
    struct sockaddr_in sockaddr;

    dprintf("%s( tcp_host = %s, tcp_port = %d )\n", __func__, tcp_host, tcp_port);

    /*Create server socket*/
    SYSCHECK(sockfd = socket(AF_INET, SOCK_STREAM, 0));

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(tcp_host);
    sockaddr.sin_port = htons(tcp_port);

    if(connect(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
        close(sockfd);
        dprintf("%s connect %s:%d errno: %d (%s)\n", __func__, tcp_host, tcp_port, errno, strerror(errno));
        return -1;
    }

    dprintf("tcp client: %s %d sockfd = %d\n", tcp_host, tcp_port, sockfd);

    return sockfd;
}

static int wait_client_connect(int serverfd) {
    int clientfd = -1;
    unsigned char addr[128];
    socklen_t alen = sizeof(addr);

    listen(serverfd, 1);    
    clientfd = accept(serverfd, (struct sockaddr *)addr, &alen);
    dprintf("sa_family = %d\n", ((struct sockaddr *)addr)->sa_family);
    if (((struct sockaddr *)addr)->sa_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
        dprintf("clientfd = %d %s:%d connect\n", clientfd, inet_ntoa(addr_in->sin_addr), addr_in->sin_port);
    } else if (((struct sockaddr *)addr)->sa_family == AF_LOCAL) {
        struct sockaddr_un *addr_un = (struct sockaddr_un *)addr;
        dprintf("clientfd = %d sun_path:%s connect\n", clientfd, addr_un->sun_path);
    }
    dprintf("%s( clientfd=%d )\n", __func__, clientfd);
    return clientfd;
}

static int create_socket_server(const char *local_name_or_tcp_port) {
    int serverfd= -1;

    dprintf("%s( %s )\n", __func__, local_name_or_tcp_port);

    if (isdigit(local_name_or_tcp_port[0]))
        serverfd = create_tcp_server(atoi(local_name_or_tcp_port));
    else
        serverfd = create_local_server(local_name_or_tcp_port);

    if (serverfd < 0)
        return -1;

    dprintf("%s( serverfd=%d )\n", __func__, serverfd);

    return serverfd;
}

static unsigned char fifo_wrap_buff[8*1024];
typedef int (*handle_client_data)(void *buf, int len);
static void swap_fds(int swapfds[2], handle_client_data handle_funcs[2], int fifo_sizes[2]) {
    struct __kfifo fifos[2];

    dprintf("%s fds={%d, %d}, client_funcs={%p, %p}, fifo_sizes={%d, %d}\n", __func__,
        swapfds[0], swapfds[1], handle_funcs[0], handle_funcs[1], fifo_sizes[0], fifo_sizes[1]);

    cfmakenoblock(swapfds[0]);
    cfmakenoblock(swapfds[1]);
    __kfifo_alloc(&fifos[0], fifo_sizes[0]);
    __kfifo_alloc(&fifos[1], fifo_sizes[1]);

    while (s_quit_flag == 0) {
        struct pollfd pollfds[] = {{swapfds[0], POLLIN, 0}, {swapfds[1], POLLIN, 0}};
        int ne, ret, nevents = sizeof(pollfds)/sizeof(pollfds[0]);

        pollfds[0].events |= kfifo_used(&fifos[1], NULL) ? POLLOUT : 0;
        pollfds[1].events |= kfifo_used(&fifos[0], NULL) ? POLLOUT : 0;

        do {
            ret = poll(pollfds, nevents, 1000);
         } while (ret < 0 && errno == EINTR && s_quit_flag == 0);

        if (s_quit_flag != 0)
            goto __swap_fds_exit;

        if (ret < 0) {
            dprintf("%s poll=%d, errno: %d (%s)\n", __func__, ret, errno, strerror(errno));
            goto __swap_fds_exit;
        }

        for (ne = 0; ne < nevents; ne++) {
            int fd = pollfds[ne].fd;
            short revents = pollfds[ne].revents;

            if (revents & (POLLERR | POLLHUP | POLLNVAL)) {
                dprintf("%s poll fd = %d, revents = %04x\n", __func__, fd,revents);
                goto __swap_fds_exit;
            }

            if (fd == swapfds[0] || fd == swapfds[1]) {
                if (revents & POLLIN) {
                    void *buf;
                    int fdi = (fd == swapfds[0] ? 0 : 1);
                    unsigned int len = kfifo_unused(&fifos[fdi], &buf);

                    if (len == 0) {
                        dprintf("fd = %d overrun!\n", fd);
                        __kfifo_out(&fifos[fdi], fifo_sizes[fdi] / 32);
                        len = kfifo_unused(&fifos[fdi], &buf);
                    }

                    if (len) {
                        ssize_t nreads = read(fd, buf, len);
                        if (nreads <= 0) {
                            dprintf("%s read=%zd, fd=%d, errno: %d (%s)\n", __func__, nreads, fd, errno, strerror(errno));
                            goto __swap_fds_exit;
                        }
                        __kfifo_in(&fifos[fdi], nreads);

                        if (nreads == len) {
                            len = kfifo_unused(&fifos[fdi], &buf);
                            if (len) {
                                nreads = read(fd, buf, len);
                                if (nreads > 0)
                                    __kfifo_in(&fifos[fdi], nreads);
                            }
                        }
                    }

                    if (handle_funcs[fdi] != NULL) {
                        len = kfifo_used(&fifos[fdi], &buf);
                        len = handle_funcs[fdi](buf, len);
                        if (len > 0)
                            __kfifo_out(&fifos[fdi], len);
                    }
                }

                if (revents & POLLOUT) {
                    void *buf;
                    int fdi = (fd == swapfds[0] ? 1 : 0);
                    unsigned int len = kfifo_used(&fifos[fdi], &buf);

                    if (len) {
                        ssize_t nwrites;

                        if ((fifos[fdi].in & fifos[fdi].mask) < (fifos[fdi].out & fifos[fdi].mask) && len < sizeof(fifo_wrap_buff)) {
                            memcpy(fifo_wrap_buff, buf, len);
                            memcpy(fifo_wrap_buff + len, fifos[fdi].data, min((sizeof(fifo_wrap_buff) - len), (fifos[fdi].in & fifos[fdi].mask)));
                            buf = fifo_wrap_buff;
                            len += min((sizeof(fifo_wrap_buff) - len), (fifos[fdi].in & fifos[fdi].mask));
                        }

                        nwrites = write(fd, buf, len);
                        if (nwrites <= 0) {
                            dprintf("%s write=%zd, fd=%d, errno: %d (%s)\n", __func__, nwrites, fd, errno, strerror(errno));
                            goto __swap_fds_exit;
                        }
                        __kfifo_out(&fifos[fdi], nwrites);
                    }
                }
            }
        }
    }

__swap_fds_exit:
    __kfifo_free(&fifos[0]);
    __kfifo_free(&fifos[1]);
    return;
}

static void * swap_sources(void *param) {
    int swapfds[2] = {-1, -1};
    handle_client_data handle_funcs[2] = {NULL, NULL};
    int fifo_sizes[2] = {512*1024, 512*1024};
    int serverfds[2] = {-1, -1};

//s:9000, s:rild-dtmf, c:/dev/ttyUSB0, c:rild-dtmf, c:192.168.10.x:9000
    char *sources[2] = {"s:9000", "c:/dev/ttyUSB0"};
    sources[0] = strdup((const char *)(((void **)param)[0]));
    sources[1] = strdup((const char *)(((void **)param)[1]));
    if (((void **)param)[2])
        fifo_sizes[0] = fifo_sizes[1] = atoi((const char *)(((void **)param)[2]));

    dprintf("%s ( %s <> %s ) fifo_size=%d\n", __func__, sources[0], sources[1],  fifo_sizes[0]);

    while (s_quit_flag == 0) {
        int i;
        swapfds[0] = swapfds[1] = -1;
        for (i = 0; i < 2; i++) {
            if (sources[i][0] == 's') {
                if (sources[i][2] == '/')
                    swapfds[i] = open_pts(&sources[i][2]);
                else {
                    if (serverfds[i] < 0)
                        serverfds[i] = create_socket_server(&sources[i][2]);
                    if (serverfds[i] > 0)
                        swapfds[i] = wait_client_connect(serverfds[i]);
                }
            } else if (sources[i][0] == 'c') {
                if (sources[i][2] == '/')
                    swapfds[i] = open_serial(&sources[i][2]);
                else if (isdigit(sources[i][2]) && strstr(&sources[i][2], ".")) {
                    char * tcp_host = strdup(&sources[i][2]);
                    char *tcp_port = strstr(tcp_host, ":");
                    *tcp_port++ = '\0';
                    swapfds[i] = connect_tcp_server(tcp_host, atoi(tcp_port));
                    free(tcp_host);
                } else if (isdigit(sources[i][2])) {
                    swapfds[i] = dup(atoi(&sources[i][2]));
                } else {
                    swapfds[i] = connect_local_server(&sources[i][2]);
                }
            }

            if (swapfds[i] < 0) {
                dprintf("%s fail to open sources[i] { %s }\n", __func__, sources[i]);
                break;
            }
        }

        if (swapfds[0] > 0 && swapfds[1] > 0)
            swap_fds(swapfds, handle_funcs, fifo_sizes);
        close(swapfds[0]);
        close(swapfds[1]);
        sleep(1);
    }

    s_quit_flag = 2;
    return NULL;
}

#ifdef RIL_SHLIB
int ql_tty2tcp(const char *s, const char *d, int fifo_size) {
    pthread_t thread_out;
    void **params = (void **) malloc(3 * sizeof(void *));
    char fifo_str[32];

    snprintf(fifo_str, sizeof(fifo_str), "%d", fifo_size ? fifo_size : (512*1024));
    params[0] = strdup(s);
    params[1] = strdup(d);
    params[2] = strdup(fifo_str);
    pthread_create(&thread_out, NULL, swap_sources, params);
    return 0;
}
#else
static void ql_sigaction(int signal_num) {
    dprintf("###### recv signal %d #####\n", signal_num);
    if (s_quit_flag == 0 && (SIGTERM == signal_num || SIGHUP == signal_num || SIGINT == signal_num)) 
    {
        s_quit_flag = 1;
    }
}

int main(int argc, char *argv[]) {
    int opt, s = 0;
    pthread_t thread_out;
    void **params = (void **) malloc(3 * sizeof(void *));
    char sources[2][64] = {"s:9000", "c:/dev/ttyUSB0"};
//s:9000                               create tcp socket server, and wait client connect
//s:rild-dtmf                         create local socket server, and wait client connect
//s:/dev/ttyUSB0                  create virtual device file /dev/ttyUSB0, and wait client open
//c:192.168.10.138:9000        connect tcp socket server 192.168.10.138:9000
//c:rild-dtmf                          connect local socket server rild-dtmf
//c:/dev/ttyUSB0                  open device file /dev/ttyUSB0
//c:fd                                    open fd, like 0 ~ stdin, 1 ~ stdout

    signal(SIGTERM, ql_sigaction);
    signal(SIGHUP, ql_sigaction);
    signal(SIGINT, ql_sigaction);

    optind = 1;
    while ( -1 != (opt = getopt(argc, argv, "s:c:d:p:"))) {
        switch (opt) {
            case 's':
                snprintf(sources[s%2], sizeof(sources[s%2]), "s:%s", optarg);
                s++;
            break;
            case 'c':
                snprintf(sources[s%2], sizeof(sources[s%2]), "c:%s", optarg);
                s++;
            break;
            case 'd':
                snprintf(sources[1], sizeof(sources[1]), "c:%s", optarg);
            break;
            case 'p': //tcp socket host port
                snprintf(sources[0], sizeof(sources[0]), "s:%s", optarg);              
            break;
            default:
            break;
        }
   }

    params[0] = strdup(sources[0]);
    params[1] =  strdup(sources[1]);
    params[2] =  NULL; //use default fifo size ~ 512*1024
    pthread_create(&thread_out, NULL, swap_sources, params);

    while(s_quit_flag != 2) 
    {
        sleep(30);
        //printf("#### sleep now #####\n");
        //system("echo mem > /sys/power/state");
    }
    dprintf("ql-tty2tcp exit!\n");

    return 0;
}
#endif
