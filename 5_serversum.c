#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    int num1, num2, sum;

    /* 1. Create UDP socket */
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    /* 2. Server address setup */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    /* 3. Bind socket */
    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    printf("UDP Server waiting for data...\n");

    addr_len = sizeof(client_addr);

    /* 4. Receive two numbers from client */
    recvfrom(server_fd, &num1, sizeof(num1), 0,
             (struct sockaddr *)&client_addr, &addr_len);

    recvfrom(server_fd, &num2, sizeof(num2), 0,
             (struct sockaddr *)&client_addr, &addr_len);

    printf("Received numbers: %d and %d\n", num1, num2);

    /* 5. Add numbers */
    sum = num1 + num2;

    /* 6. Send sum back to client */
    sendto(server_fd, &sum, sizeof(sum), 0,
           (struct sockaddr *)&client_addr, addr_len);

    /* 7. Close socket */
    close(server_fd);

    return 0;
}

