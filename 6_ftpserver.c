#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char command[10], filename[100], buffer[1024];
    int n;
    FILE *fp;

    /* 1. Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* 2. Server address setup */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    /* 3. Bind */
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    /* 4. Listen */
    listen(server_fd, 1);
    printf("FTP Server waiting...\n");

    addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    printf("Client connected successfully.\n");

    /* 5. Receive command and filename */
    read(client_fd, command, sizeof(command));
    read(client_fd, filename, sizeof(filename));

    /* -------- PUT -------- */
    if (strcmp(command, "PUT") == 0) {
        fp = fopen(filename, "w");
        while ((n = read(client_fd, buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, n, fp);
        }
        fclose(fp);
        printf("PUT successful: File '%s' received from client.\n", filename);
    }

    /* -------- GET -------- */
    else if (strcmp(command, "GET") == 0) {
        fp = fopen(filename, "r");
        if (fp == NULL) {
            write(client_fd, "File not found", 14);
            printf("GET failed: File '%s' not found.\n", filename);
        } else {
            while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
                write(client_fd, buffer, n);
            }
            fclose(fp);
            printf("GET successful: File '%s' sent to client.\n", filename);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
