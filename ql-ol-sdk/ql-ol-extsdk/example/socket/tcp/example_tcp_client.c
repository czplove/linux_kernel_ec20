#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 

#define portnumber 3333

int main(int argc, char *argv[]) 
{ 
    int sockfd; 
    char buffer[1024]; 
    struct sockaddr_in server_addr; 
    struct hostent *host; 

    if(argc!=2) 
    { 
        fprintf(stderr,"Usage:%s hostname \a\n",argv[0]); 
        exit(1); 
    } 

    if((host=gethostbyname(argv[1]))==NULL) 
    { 
        fprintf(stderr,"Gethostname error\n"); 
        exit(1); 
    } 


    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:Internet;SOCK_STREAM:TCP
    { 
        fprintf(stderr,"Socket Error:%s\a\n",strerror(errno)); 
        exit(1); 
    } 

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;          // IPV4
    server_addr.sin_port=htons(portnumber);
    server_addr.sin_addr=*((struct in_addr *)host->h_addr);
    

    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1) 
    { 
        fprintf(stderr,"Connect Error:%s\a\n",strerror(errno)); 
        exit(1); 
    } 


    printf("Please input char:\n");
    

    fgets(buffer,1024,stdin); 
    write(sockfd,buffer,strlen(buffer)); 


    close(sockfd); 
    exit(0); 
} 
