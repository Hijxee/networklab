#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main()
{
    int client_fd;
    struct sockaddr_in server_addr;
    int num1, num2, sum;

    /* Create socket */
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(client_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    /* Server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Connect */
    if(connect(client_fd,
               (struct sockaddr *)&server_addr,
               sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        return 1;
    }

    /* Input */
    printf("Enter two numbers: ");
    scanf("%d%d", &num1, &num2);

    /* Send numbers */
    write(client_fd, &num1, sizeof(num1));
    write(client_fd, &num2, sizeof(num2));

    /* Receive sum */
    read(client_fd, &sum, sizeof(sum));

    printf("Sum from server: %d\n", sum);

    close(client_fd);

    return 0;
}
