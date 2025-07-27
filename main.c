#include "src/helpers.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

/// Creates a socket that listens on `port`, and returns file descriptor.
int create_server(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (!server_fd)
    {
        leave("Couldn't create the socket.");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        leave("Cannot set server options to reuse the address.");
    }

    struct sockaddr_in addr = {.sin_addr.s_addr = INADDR_ANY, .sin_port = htons(port), .sin_family = AF_INET};

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

    if (listen(server_fd, 3) < 0)
    {
        leave("Oh no. Server cannot listen");
    }

    return server_fd;
}

int main(int argc, char *argv[])
{
	printf("Hello world!");
    return 0;
}
