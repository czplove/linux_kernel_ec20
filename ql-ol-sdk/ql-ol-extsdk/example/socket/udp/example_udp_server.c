#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888 
#define MAX_MSG_SIZE 1024 

void udps_respon(int sockfd) 
{ 
    struct sockaddr_in addr; 
    int addrlen,n; 
    char msg[MAX_MSG_SIZE]; 

    while(1) 
    {
        bzero(msg,sizeof(msg));
        addrlen = sizeof(struct sockaddr); 
        n=recvfrom(sockfd,msg,MAX_MSG_SIZE,0,(struct sockaddr*)&addr,&addrlen);
        msg[n]=0; 

        fprintf(stdout,"Server have received: %s",msg);
    } 
} 

int main(void) 
{ 
    int sockfd; 
    struct sockaddr_in addr; 


    sockfd=socket(AF_INET,SOCK_DGRAM,0); 
    if(sockfd<0) 
    { 
        fprintf(stderr,"Socket Error:%s\n",strerror(errno)); 
        exit(1); 
    } 


    bzero(&addr,sizeof(struct sockaddr_in)); 
    addr.sin_family=AF_INET; 
    addr.sin_addr.s_addr=htonl(INADDR_ANY); 
    addr.sin_port=htons(SERVER_PORT); 

    /* À¦°ósockfdÃèÊö·û */ 
    if(bind(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in))<0) 
    { 
        fprintf(stderr,"Bind Error:%s\n",strerror(errno)); 
        exit(1); 
    } 

    udps_respon(sockfd);
    close(sockfd); 
} 
