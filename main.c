#include "src/helpers.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define _DEBUG_MODE

#define BUFF_SIZE 16384

typedef struct
{
    int server_fd;
    struct sockaddr_in *addr_ptr;
    int *addrlen;
} server_t;

/// Creates a socket that listens on `port`, and returns file descriptor.
server_t create_server(int port)
{
    server_t server;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.server_fd = server_fd;
    server.addr_ptr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    int opt = 1;

    if (!server_fd)
    {
        leave("Couldn't create the socket.");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        leave("Cannot set server options to reuse the address.");
    }

    // server.addr_ptr = {.sin_addr.s_addr = INADDR_ANY, .sin_port = htons(port), .sin_family = AF_INET};
    server.addr_ptr->sin_addr.s_addr = INADDR_ANY;
    server.addr_ptr->sin_port = htons(port);
    server.addr_ptr->sin_family = AF_INET;

    if (bind(server_fd, (struct sockaddr *)server.addr_ptr, sizeof(*server.addr_ptr)) < 0)
    {
        leave("Could not bind server");
    }

    server.addrlen = (int *)malloc(sizeof(int));
    *server.addrlen = sizeof(*server.addr_ptr);

    if (listen(server_fd, 3) < 0)
    {
        leave("Oh no. Server cannot listen");
    }

#ifdef _DEBUG_MODE

    printf("Server is listening on port %d\n", port);

#endif

    return server;
}

void handle_connection(int client_socket)
{
    char buffer[BUFF_SIZE] = {0};
    ssize_t bytes_read = read(client_socket, buffer, BUFF_SIZE - 1);

    if (bytes_read > 0)
    {
        printf("Received: %.*s\n", (int)bytes_read, buffer);
        const char *resp = "HTTP/1.1 200 OK\nContent-Length: 12\n\nGotcha.";
        send(client_socket, resp, strlen(resp), 0);
    }
    close(client_socket);
}

void simple_event_loop(server_t server)
{
    int req_socket;
    char *buffer = (char *)malloc(sizeof(char) * BUFF_SIZE);

    while (true)
    {
        if ((req_socket = accept(server.server_fd, (struct sockaddr *)server.addr_ptr, (socklen_t *)server.addrlen)) <
            0)
        {
            leave("Couldn't handle incoming request.");
        }

#ifdef _DEBUG_MODE
        printf("Handling incoming request.\n");
#endif

        handle_connection(req_socket);

        close(req_socket);
    }

    close(server.server_fd);
    free(server.addr_ptr);
    free(server.addrlen);
}

int main(int argc, char *argv[])
{
    server_t server = create_server(8585);
    simple_event_loop(server);

    return 0;
}
