#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 

#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 


#define portnumber 3333

int main(int argc, char *argv[]) 
{ 
    int sockfd,new_fd; 
    struct sockaddr_in server_addr; 
    struct sockaddr_in client_addr; 
    int sin_size; 
    int nbytes;
    char buffer[1024];
    


    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:IPV4;SOCK_STREAM:TCP
    { 
        fprintf(stderr,"Socket error:%s\n\a",strerror(errno)); 
        exit(1); 
    } 


    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;                 // Internet
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);  
    //server_addr.sin_addr.s_addr=inet_addr("192.168.0.3");
    server_addr.sin_port=htons(portnumber);
    

    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1) 
    { 
        fprintf(stderr,"Bind error:%s\n\a",strerror(errno)); 
        exit(1); 
    } 


    if(listen(sockfd,5)==-1) //µÈ´ýÁ¬½Ó
    { 
        fprintf(stderr,"Listen error:%s\n\a",strerror(errno)); 
        exit(1); 
    } 

    while(1) 
    { 
        sin_size=sizeof(struct sockaddr_in); 
        if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)
        { 
            fprintf(stderr,"Accept error:%s\n\a",strerror(errno)); 
            exit(1); 
        } 
        printf("Server get connection from %s\n", inet_ntoa(client_addr.sin_addr));
        
        if((nbytes=read(new_fd,buffer,1024))==-1) 
        { 
            fprintf(stderr,"Read Error:%s\n",strerror(errno)); 
            exit(1); 
        }         
        buffer[nbytes]='\0';
        printf("Server received  :%s\n",buffer);
        
        close(new_fd);
    } 


    close(sockfd); 
    exit(0); 
} 
