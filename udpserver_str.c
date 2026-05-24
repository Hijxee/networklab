#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main()
{
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[100];
    int len, i;

    /* Create UDP socket */
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    /* Server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    /* Bind */
    if(bind(server_fd,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        return 1;
    }

    printf("UDP Server waiting...\n");

    addr_len = sizeof(client_addr);

    /* Receive string */
    recvfrom(server_fd,
             buffer,
             sizeof(buffer),
             0,
             (struct sockaddr *)&client_addr,
             &addr_len);

    printf("Received string: %s\n", buffer);

    /* Reverse string */
    len = strlen(buffer);

    for(i = 0; i < len / 2; i++) {
        char temp = buffer[i];
        buffer[i] = buffer[len - i - 1];
        buffer[len - i - 1] = temp;
    }

    /* Send reversed string */
    sendto(server_fd,
           buffer,
           strlen(buffer) + 1,
           0,
           (struct sockaddr *)&client_addr,
           addr_len);

    close(server_fd);

    return 0;
}
