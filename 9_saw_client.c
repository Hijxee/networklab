#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_addr;
    int frame, ack;
    int total_frames;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Enter number of frames to send: ");
    scanf("%d", &total_frames);

    frame = 0;

    for (int i = 0; i < total_frames; i++) {
        printf("\nClient: Sending frame %d", frame);
        write(sock, &frame, sizeof(frame));

        read(sock, &ack, sizeof(ack));

        if ((frame == 0 && ack == 1) || (frame == 1 && ack == 0)) {
            printf("\nClient: Correct ACK %d received", ack);
            frame = 1 - frame; // Toggle between 0 and 1
        } else {
            printf("\nClient: NACK received, retransmitting frame %d", frame);
            i--; // Retransmit same frame
        }
    }

    frame = -1;
    write(sock, &frame, sizeof(frame));

    close(sock);
    return 0;
}