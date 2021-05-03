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
#define MAX_BUFFER_SIZE 1500
#define ACK_HEADER 4
#define NUM_HEADER 32

int socket_desc;
int init = 1;
// Global variable for file path

void send_it(struct addrinfo hints, char * buf, struct addrinfo * address_resource, char * PORT){
    int numbytes;
    int rv;
    char * send_buf[MAX_BUFFER_SIZE];
    struct addrinfo *p;
    if(( rv = getaddrinfo(CLIENT, PORT, &hints, &address_resource)) !=0){
        fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }
    //links up with the socket


    for(p = address_resource; p != NULL; p = p->ai_next) {
        if((socket_desc = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return;
    }
    if ((numbytes = sendto (socket_desc, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen)) == -1){
        perror("talker: sendto");
        exit(1);
    }
    printf("talker: sent %d bytes to %s\n", numbytes, CLIENT);

}

void process_content(socklen_t remote_addr_s,struct sockaddr_storage remote_addr, struct addrinfo hints, struct addrinfo * address_resource, char * PORT) {
    int num_bytes;
    char * buf[ACK_HEADER];
    char * recv_buf[MAX_BUFFER_SIZE];
    remote_addr_s = sizeof(remote_addr);
    num_bytes = recvfrom(socket_desc, recv_buf, MAX_BUFFER_SIZE - 1, 0, (struct sockaddr *) &remote_addr, &remote_addr_s);

    if (num_bytes == -1)
        printf("Error: recvfrom %s (line: %d)\n", strerror(errno), __LINE__);

    printf("Received packet \nPacket is %d long\n", num_bytes);
    buf[num_bytes] = '\0';
    printf("PACKET CONTENTS: %s \n", recv_buf);


    //If this is the first communication from client
    if(init == 1){


        init = 0;

        // parse incoming buffer for search paths

        FILE *fp;
        char *home_dir = malloc(MAX_BUFFER_SIZE);

        strcpy(home_dir, DOCROOT);
        strcat(home_dir, file);
        fp = fopen(home_dir, "r");
        if (fp == NULL){

            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            strcpy(buf, "NACK");
        }

    }
    else{
            // loop that sends the data requested from buf

        }
    if(strcmp(buf, "PACK") == 0)
        send_it(hints, buf, address_resource, PORT);
}
void Listen(struct addrinfo *address_resource, struct addrinfo hints, socklen_t remote_addr_s,struct sockaddr_storage remote_addr, char * PORT){
    // socket(): creates a new UDP socket, no address or port is assigned yet
    int return_value;
    struct addrinfo *p;

    return_value = getaddrinfo(NULL, PORT, &hints, &address_resource);
    if (return_value != 0) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
    }
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
        return;
    }

    printf("Listener: waiting to recv from... \n");
    process_content(remote_addr_s, remote_addr, hints, address_resource, PORT);
    return;
}

int main(int argc, char* argv[]) {

    char *PORT = argv[1];
    struct addrinfo hints;
    struct addrinfo * address_resource;
    struct sockaddr_storage remote_addr;
    char *buf[MAX_BUFFER_SIZE];
    strcpy(buf, "Hello there");
    socklen_t remote_addr_s = sizeof(remote_addr);
   // char s[INET_ADDRSTRLEN];

    printf("%s \n", PORT);


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_DGRAM;
    // SOCK_STREAM: ordered packets (e.g. TCP)
    // SOCK_DGRAM: possibly unordered packets (e.g. UDP)
    hints.ai_flags = AI_PASSIVE; // Listen

    //return value for listener

    Listen(address_resource, hints, remote_addr_s, remote_addr, PORT);
    freeaddrinfo(address_resource);

    close(socket_desc);

    return 0;
}