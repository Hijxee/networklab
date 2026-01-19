#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

void error (char *msg) {
    perror(msg);
    exit(1);
}
int main(int argc,char *argv[]) {
    int sockfd, portnumber, n;
    char buffer[256];
    struct sockaddr_in serveraddress,clientaddress;
    struct hostent *server;
    
    if(argc < 3 ) {
        fprintf(stderr,"usage : %s hostname port \n",argv[0]);
        exit(0);
    }
    portnumber = atoi(argv[2]);
    /*create socket */
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0 )
        error("error opening socket \n");
        
    /*Get Server*/
    server = gethostbyname(argv[1]);
    if (server == NULL ) { 
        fprintf(stderr,"error : no such host\n");
        exit(0);
    }
        
    /*initialize server address*/
    bzero((char *)&serveraddress, sizeof(serveraddress));
    serveraddress.sin_family = AF_INET;
    bcopy((char*) server -> h_addr,(char*)&serveraddress.sin_addr.s_addr,server -> h_length);
    serveraddress.sin_port = htons(portnumber);
    
    /*connect to server */
    if(connect(sockfd,(struct sockaddr*)&serveraddress,sizeof(serveraddress)) < 0) 
        error("error connecting\n");
        
    /*get input from user*/    
    printf("Enter Message : ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
        
    /*send to server*/
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0 )
        error("error writing to socket \n");
    
    /*recieve echo */
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0 )
        error("error reading from socket \n");
    printf("Echo from Server : %s \n",buffer);
    
    close(sockfd);
    return 0;
}
