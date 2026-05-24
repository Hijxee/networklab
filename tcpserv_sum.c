#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    int num1, num2, sum;

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

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

    /* Listen */
    if(listen(server_fd, 5) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("Server waiting...\n");

    addr_len = sizeof(client_addr);

    /* Accept client */
    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &addr_len);

    if(client_fd < 0) {
        perror("Accept failed");
        return 1;
    }

    /* Receive numbers */
    read(client_fd, &num1, sizeof(num1));
    read(client_fd, &num2, sizeof(num2));

    printf("Received numbers: %d %d\n", num1, num2);

    /* Add */
    sum = num1 + num2;

    /* Send sum */
    write(client_fd, &sum, sizeof(sum));

    close(client_fd);
    close(server_fd);

    return 0;
}
