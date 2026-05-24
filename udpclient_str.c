#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main()
{
    int client_fd;
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    char buffer[100];

    /* Create UDP socket */
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(client_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    /* Server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    addr_len = sizeof(server_addr);

    /* Input string */
    printf("Enter string: ");
    scanf("%s", buffer);

    /* Send string */
    sendto(client_fd,
           buffer,
           strlen(buffer) + 1,
           0,
           (struct sockaddr *)&server_addr,
           addr_len);

    /* Receive reversed string */
    recvfrom(client_fd,
             buffer,
             sizeof(buffer),
             0,
             (struct sockaddr *)&server_addr,
             &addr_len);

    printf("Reversed string: %s\n", buffer);

    close(client_fd);

    return 0;
}
