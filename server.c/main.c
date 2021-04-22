#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>


// #define SERVER "10.121.218.11"
#define SPORT "4950"

// #define CLIENT "10.121.218.10"
// #define MAX_CLIENT_BACKLOG 128
#define MAX_BUFFER_SIZE 4096
// #define IS_MULTIPROCESS 0

int main(int argc, char* argv[]) {


    int return_value;
    int num_bytes;
    char *HOST = argv[1];
    char *CLIENT = argv[2];
    char *PORT = argv[3];
    struct addrinfo hints, *p;
    struct addrinfo * address_resource;
    struct sockaddr_storage remote_addr;
    char buf[MAX_BUFFER_SIZE];
    socklen_t remote_addr_s = sizeof(remote_addr);
   // char s[INET_ADDRSTRLEN];

    printf("%s \n", HOST);
    printf("%s \n", PORT);


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_DGRAM;
    // SOCK_STREAM: ordered packets (e.g. TCP)
    // SOCK_DGRAM: possibly unordered packets (e.g. UDP)
    hints.ai_flags = AI_PASSIVE; // Listen

    return_value = getaddrinfo(NULL, SPORT, &hints, &address_resource);
    if (return_value != 0) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
    }

    // socket(): creates a new UDP socket, no address or port is assigned yet
    int socket_desc;
    // Loops until client is found
    for  (p = address_resource; p != NULL ; p = p->ai_next) {
        if( (socket_desc = socket(p->ai_family, p->ai_socktype, p->ai_protocol))  == -1) {
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            continue;
        }
        if (bind(socket_desc, p->ai_addr, p->ai_addrlen ) == -1){
            close(socket_desc);
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            continue;
        }
        break;
    }

    if(p == NULL){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
    }

    freeaddrinfo(address_resource);

    printf("Listener: waiting to recvfrom... \n");

    remote_addr_s = sizeof(remote_addr);
    num_bytes = recvfrom(socket_desc, buf, MAX_BUFFER_SIZE-1, 0, (struct sockaddr *) &remote_addr, &remote_addr_s);

    if (num_bytes == -1)
        printf("Error: recvfrom %s (line: %d)\n", strerror(errno), __LINE__);

    printf("Received packet \nPacket is %d long\n", num_bytes);
    buf[num_bytes] = '\0';
    printf("PACKET CONTENTS: %s ", buf);

    close(socket_desc);

    return 0;
}