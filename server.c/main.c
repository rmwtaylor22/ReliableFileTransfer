#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/types.h>
//#include <arpa/inet.h>


// #define SERVER "10.121.218.11"


#define CLIENT "10.121.218.10"
// #define MAX_CLIENT_BACKLOG 128
#define MAX_BUFFER_SIZE 4096

int socket_desc;
void send_it(struct addrinfo *p){
    char *buf;
    buf = "HELLO THERE";
    int numbytes;
    if ((numbytes = sendto (socket_desc, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen)) == -1){
        perror("talker: sendto");
        exit(1);
    }
    printf("talker: sent %d bytes to %s\n", numbytes, CLIENT);
}

void process_content(socklen_t remote_addr_s,struct sockaddr_storage remote_addr) {
    int num_bytes;
    char buf[MAX_BUFFER_SIZE];
    remote_addr_s = sizeof(remote_addr);
    num_bytes = recvfrom(socket_desc, buf, MAX_BUFFER_SIZE - 1, 0, (struct sockaddr *) &remote_addr, &remote_addr_s);

    if (num_bytes == -1)
        printf("Error: recvfrom %s (line: %d)\n", strerror(errno), __LINE__);

    printf("Received packet \nPacket is %d long\n", num_bytes);
    buf[num_bytes] = '\0';
    printf("PACKET CONTENTS: %s \n", buf);

}
void Listen(struct addrinfo *address_resource, struct addrinfo *p, socklen_t remote_addr_s,struct sockaddr_storage remote_addr){
    // socket(): creates a new UDP socket, no address or port is assigned yet

    // Loops until client is found
    for  (p = address_resource; p != NULL ; p = p->ai_next) {
        if( (socket_desc = socket(p->ai_family, p->ai_socktype, p->ai_protocol))  == -1) {
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            return;
        }
        if (bind(socket_desc, p->ai_addr, p->ai_addrlen ) == -1){
            close(socket_desc);
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            return;
        }
        break;
    }
    if(p == NULL){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
    }

    printf("Listener: waiting to recv from... \n");
    process_content(remote_addr_s, remote_addr);
}

int main(int argc, char* argv[]) {

    int return_value, rv;
    char *PORT = argv[1];
    struct addrinfo hints, *p;
    struct addrinfo * address_resource;
    struct sockaddr_storage remote_addr;
    char buf[MAX_BUFFER_SIZE];
    socklen_t remote_addr_s = sizeof(remote_addr);
   // char s[INET_ADDRSTRLEN];

    printf("%s \n", PORT);


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_DGRAM;
    // SOCK_STREAM: ordered packets (e.g. TCP)
    // SOCK_DGRAM: possibly unordered packets (e.g. UDP)
    hints.ai_flags = AI_PASSIVE; // Listen

    return_value = getaddrinfo(NULL, PORT, &hints, &address_resource);
    if (return_value != 0) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
    }



    Listen(address_resource, p, remote_addr_s, remote_addr);
    freeaddrinfo(address_resource);

    if((rv = getaddrinfo(CLIENT, PORT, &hints, &address_resource)) !=0){
        fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    for(p = address_resource; p != NULL; p = p->ai_next) {
        if((socket_desc = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }

    send_it(p);
    close(socket_desc);

    return 0;
}