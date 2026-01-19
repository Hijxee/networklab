#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serversocket, clientsocket;
    int portnumber;
    char buffer[256];
    struct sockaddr_in serveraddress, clientaddress;
    socklen_t clientlength;
    int n;

    /* Check port number */
    if (argc < 2)
    {
        fprintf(stderr, "Error: Port number not provided\n");
        exit(1);
    }

    /* Create socket */
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0)
        error("Error opening socket");

    /* Initialize server address */
    memset(&serveraddress, 0, sizeof(serveraddress));
    portnumber = atoi(argv[1]);

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(portnumber);

    /* Bind socket */
    if (bind(serversocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress)) < 0)
        error("Error on binding");

    /* Listen */
    if (listen(serversocket, 5) < 0)
        error("Error on listen");

    clientlength = sizeof(clientaddress);

    /* Accept client */
    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, &clientlength);
    if (clientsocket < 0)
        error("Error on accept");

    /* Read message from client */
    memset(buffer, 0, sizeof(buffer));
    n = read(clientsocket, buffer, sizeof(buffer) - 1);
    if (n < 0)
        error("Error reading from socket");

    buffer[n] = '\0';   // Ensure null termination
    printf("Message from client: %s\n", buffer);

    /* Echo back to client */
    n = write(clientsocket, buffer, strlen(buffer));
    if (n < 0)
        error("Error writing to socket");

    /* Close sockets */
    close(clientsocket);
    close(serversocket);

    return 0;
}

