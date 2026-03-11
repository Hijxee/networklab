#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int frame, ack;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    printf("Server waiting for connection...\n");
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);

    while (1) {
        read(client_fd, &frame, sizeof(frame));

        if (frame == -1)
            break;

        printf("\nServer: Frame received = %d", frame);

        if (frame == 0) {
            ack = 1;
            printf("\nServer: Sending ACK %d\n", ack);
            write(client_fd, &ack, sizeof(ack));
        }
        else if (frame == 1) {
            ack = 0;
            printf("\nServer: Sending ACK %d\n", ack);
            write(client_fd, &ack, sizeof(ack));
        }
        else {
            ack = -1; // Negative ACK
            printf("\nServer: Invalid frame, sending NACK\n");
            write(client_fd, &ack, sizeof(ack));
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}