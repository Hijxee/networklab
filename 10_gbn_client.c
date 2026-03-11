#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define WINDOW_SIZE 4

int main() {
    int sock;
    struct sockaddr_in server_addr;
    int total_frames;
    int next_frame = 0;

    printf("Enter total frames: ");
    scanf("%d", &total_frames);

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* Server address configuration */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Connect to server */
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    /* Send total frames */
    send(sock, &total_frames, sizeof(total_frames), 0);

    /* Sliding window transmission */
    while (next_frame < total_frames) {

        int remaining_frames =
            ((next_frame + WINDOW_SIZE) < total_frames)
                ? WINDOW_SIZE
                : (total_frames - next_frame);

        for (int i = 0; i < remaining_frames; i++) {
            send(sock, &next_frame, sizeof(next_frame), 0);
            printf("Sent frame %d\n", next_frame);
            next_frame++;
        }

        int ack_frame;
        recv(sock, &ack_frame, sizeof(ack_frame), 0);
        printf("Received ACK %d\n", ack_frame);

        next_frame = ack_frame;
    }

    close(sock);
    return 0;
}