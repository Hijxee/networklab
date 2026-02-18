#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char command[10], filename[100], buffer[1024];
    int n;
    FILE *fp;

    /* 1. Create socket */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* 2. Server address setup */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* 3. Connect */
    connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Connected to FTP server successfully.\n");

    /* 4. Input */
    printf("Enter command (GET / PUT): ");
    scanf("%s", command);
    printf("Enter filename: ");
    scanf("%s", filename);

    /* 5. Send command and filename */
    write(sock_fd, command, sizeof(command));
    write(sock_fd, filename, sizeof(filename));

    /* -------- PUT -------- */
    if (strcmp(command, "PUT") == 0) {
        fp = fopen(filename, "r");
        if (fp == NULL) {
            printf("PUT failed: File not found.\n");
        } else {
            while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
                write(sock_fd, buffer, n);
            }
            fclose(fp);
            printf("PUT successful: File '%s' uploaded to server.\n", filename);
        }
    }

    /* -------- GET -------- */
    else if (strcmp(command, "GET") == 0) {
        fp = fopen(filename, "w");
        while ((n = read(sock_fd, buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, n, fp);
        }
        fclose(fp);
        printf("GET successful: File '%s' downloaded from server.\n", filename);
    }

    close(sock_fd);
    return 0;
}

