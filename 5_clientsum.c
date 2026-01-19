#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    int num1, num2, sum;

    /* 1. Create UDP socket */
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    /* 2. Server address setup */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    addr_len = sizeof(server_addr);

    /* 3. Read input from user */
    printf("Enter first number: ");
    scanf("%d", &num1);

    printf("Enter second number: ");
    scanf("%d", &num2);

    /* 4. Send numbers to server */
    sendto(client_fd, &num1, sizeof(num1), 0,
           (struct sockaddr *)&server_addr, addr_len);

    sendto(client_fd, &num2, sizeof(num2), 0,
           (struct sockaddr *)&server_addr, addr_len);

    /* 5. Receive sum from server */
    recvfrom(client_fd, &sum, sizeof(sum), 0,
             (struct sockaddr *)&server_addr, &addr_len);

    printf("Sum from server: %d\n", sum);

    /* 6. Close socket */
    close(client_fd);

    return 0;
}

