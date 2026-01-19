#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }
    printf("UDP Echo Server running on port %d...\n", PORT);
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
             int bytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                             (struct sockaddr *)&client_addr, &addr_len);
        if (bytes < 0) {
            perror("Receive failed");
            break;
        }
        printf("Received: %s", buffer);
        sendto(sockfd, buffer, bytes, 0,
               (struct sockaddr *)&client_addr, addr_len);
    }
    close(sockfd);
    return 0;
}

