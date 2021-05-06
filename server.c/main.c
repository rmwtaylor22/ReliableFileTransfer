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
int seqNum = 0;
// Global variable for file path
struct addrinfo hints;
struct addrinfo * address_resource;
struct sockaddr_storage remote_addr;
socklen_t remote_addr_s = sizeof(remote_addr);

void Listen();


void process_response(PORT)
{

    Listen();
}




void send_it(char *buf, char * PORT){
    int numbytes;
    int rv;
    char * send_buf[MAX_BUFFER_SIZE];
    struct addrinfo *p;

    // get address info
    if(( rv = getaddrinfo(CLIENT, PORT, &hints, &address_resource)) !=0){
        fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }

    // links up with the socket
    for(p = address_resource; p != NULL; p = p->ai_next) {
        if((socket_desc = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }
    // couldn't link up with socket: error
    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return;
    }

    int bytes_read;
    char c;

    // loop of sending char c data chunk by chunk
    while(1) {
        bytes_read = fread(&c, 1, 1500, buf);
        if (bytes_read != 1) {
            break;
        }
        sendto(socket_desc, &c, strlen(&c), 0, p->ai_addr, p->ai_addrlen);
    }
}

void process_request(char * PORT) {
    int num_bytes;
    char * ackBuf[ACK_HEADER];
    char * dataBuf[MAX_BUFFER_SIZE];
    char * recv_buf[MAX_BUFFER_SIZE];
    remote_addr_s = sizeof(remote_addr);
    num_bytes = recvfrom(socket_desc, recv_buf, MAX_BUFFER_SIZE - 1, 0, (struct sockaddr *) &remote_addr, &remote_addr_s);

    if (num_bytes == -1)
        printf("Error: recvfrom %s (line: %d)\n", strerror(errno), __LINE__);

    printf("Received packet \nPacket is %d long\n", num_bytes);

    // Marks the end of the file?
    char buf= '\0';
    printf("PACKET CONTENTS: %s \n", *recv_buf);


    char ip[15], port[4], file[100];

    sscanf((const char *) recv_buf, "%s %s %s", ip, port, file);

    // parse incoming buffer for search paths
    FILE *fp;
    fp = fopen(file, "r");
    if (fp == NULL){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
    }
    else {
        int bytes_read;
        char c;
        int totalRead = 0;

        // read all of the data into c buffer
        while (1) {
            bytes_read = fread(&c, 1, 1, fp);
            totalRead ++;
            if (bytes_read != 1) {
                break;
            }
            if (totalRead % 1500 == 0){
                // send buffer to "send_it" to have the data be sent little by little
                send_it(&c, PORT);
                memset(&c, 0, 1);
            }
        }
    }
}





void Listen(char * PORT){
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
    process_request(PORT);

}

int main(int argc, char* argv[]) {

    char *PORT = argv[1];

   // char s[INET_ADDRSTRLEN];

    printf("%s \n", PORT);


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_DGRAM;
    // SOCK_STREAM: ordered packets (e.g. TCP)
    // SOCK_DGRAM: possibly unordered packets (e.g. UDP)
    hints.ai_flags = AI_PASSIVE; // Listen

    //return value for listener

    Listen(PORT);
    freeaddrinfo(address_resource);

    close(socket_desc);

    return 0;
}