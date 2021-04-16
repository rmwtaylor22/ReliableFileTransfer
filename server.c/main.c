#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "unistd.h"
#include "stdlib.h"
#include <time.h>

#define SERVER "10.121.218.11"
#define SPORT "8080"
// #define CLIENT "10.121.218.10"
#define MAX_CLIENT_BACKLOG 128
#define MAX_BUFFER_SIZE 4096
#define IS_MULTIPROCESS 0

int main(int argc, char* argv[]) {


    int socket_desc, accept_desc;
    int return_value;
    int enable = 1;

    char *HOST = argv[1];
    char *CLIENT = argv[2];
    char *PORT = argv[3];
    struct addrinfo hints;
    struct addrinfo * address_resource;
    struct sockaddr_storage remote_addr;
    socklen_t remote_addr_s = sizeof(remote_addr);

    printf("%s \n", HOST);
    printf("%s \n", PORT);


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_DGRAM;
    // SOCK_STREAM: ordered packets (e.g. TCP)
    // SOCK_DGRAM: possibly unordered packets (e.g. UDP)
    hints.ai_flags = AI_PASSIVE; // Listen

    return_value = getaddrinfo(SERVER, SPORT, &hints, &address_resource);
    if (return_value != 0) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    // socket(): creates a new TCP/IP socket, no address or port is assigned yet
    socket_desc = socket(
            address_resource->ai_family,    // IPv4
            address_resource->ai_socktype,  // Streaming Protocol
            address_resource->ai_protocol   // TCP
    );
    if (socket_desc == -1) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return socket_desc;
    }

    return_value = setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    // bind(): assign address and port to socket
    return_value = bind(socket_desc, address_resource->ai_addr, address_resource->ai_addrlen);
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    freeaddrinfo(address_resource);

    return_value = listen(socket_desc, MAX_CLIENT_BACKLOG);
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

/*
    while(1){


        accept_desc = accept(socket_desc, (struct sockaddr *) &remote_addr, &remote_addr_s);

#if IS_MULTIPROCESS == 1
        int pid = fork();
            if (pid == 0){
                //...child process starts
                handle_connection(accept_desc, DOCROOT);
                close(accept_desc);
                close(socket_desc);
                exit(0);
                //...child process ends
            }
            if (pid == -1){
                printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
                return pid;
            }

        close(accept_desc);

#else
        handle_connection(accept_desc, DOCROOT);
        close(accept_desc);
#endif
    }
*/

    close(socket_desc);

    return 0;
}