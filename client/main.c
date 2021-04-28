#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVER "10.121.218.11"
#define CLIENT "10.121.218.10"
#define MAX_CLIENT_BACKLOG 128
#define MAX_BUFFER_SIZE 4096
#define SPORT "4950"
#define IS_MULTIPROCESS 1

// arg 1 is hostPortValue, arg2 = the *message

void sendIt()
{

}

int process_contents(socket_desc)
{
    int num_bytes;
    char buf[MAX_BUFFER_SIZE];
    struct sockaddr_storage remote_addr;
    socklen_t remote_addr_s = sizeof(remote_addr);

    remote_addr_s = sizeof(remote_addr);
    num_bytes = recvfrom(socket_desc, buf, MAX_BUFFER_SIZE-1, 0, (struct sockaddr *) &remote_addr, &remote_addr_s);

    if (num_bytes == -1)
        printf("Error: recvfrom %s (line: %d)\n", strerror(errno), __LINE__);

    printf("Received packet \nPacket is %d long\n", num_bytes);
    buf[num_bytes] = '\0';
    printf("PACKET CONTENTS: %s ", buf);

    if (strcmp(buf,"ACK") == 0){
        close(socket_desc);
        return 1;

    } else {
        fprintf(stderr, "Server sent something other than an acknowledgement.\n");
        close(socket_desc);
        return -1;
    }
}


int listener()
{
    int return_value;
    int num_bytes;
    struct addrinfo hints, *p;
    struct addrinfo * address_resource;
    struct sockaddr_storage remote_addr;
    char buf[MAX_BUFFER_SIZE];
    socklen_t remote_addr_s = sizeof(remote_addr);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_DGRAM;
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
            return -1;
        }
        if (bind(socket_desc, p->ai_addr, p->ai_addrlen ) == -1){
            close(socket_desc);
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            return -1;
        }
        break;
    }

    if(p == NULL){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return -1;
    }

    freeaddrinfo(address_resource);

    printf("Listener: waiting to recvfrom... \n");

    process_contents(socket_desc);

    return 1;
}



int main(int argc, char *argv[])
{
    int socket_desc;
    struct addrinfo hints, *address_resource, *p;
    int rv;
    int numbytes;

    char * buf = "We the People of the United States, in Order to form a more perfect Union, establish Justice, insure domestic Tranquility, provide for the common defence, promote the general Welfare, and secure the Blessings of Liberty to ourselves and our Posterity, do ordain and establish this Constitution for the United States of America.";

    const char * serverVal = argv[1];
    printf("SERVER_IP: %s\n", serverVal);
    const char * serverPort = argv[2];
    printf("SERVER_PORT: %s\n", serverPort);
    const char * remotePath = argv[3];
    printf("REMOTE_PATH: %s\n", remotePath);
    const char * localPath = argv[4];
    printf("LOCAL_PATH: %s\n", localPath);

    const char * ack = "ACK";

    if (argc != 5) {
        fprintf(stderr, "usage: talker hostname message\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if((rv = getaddrinfo(serverVal, serverPort, &hints, &address_resource)) !=0){
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

    // send the ACK
    if ((numbytes = sendto (socket_desc, ack, strlen(ack), 0, p->ai_addr, p->ai_addrlen)) == -1){
        perror("talker: sendto");
        exit(1);
    }

    printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);


    // listen for follow-up ACK
    // listener == 1 if it received something, -1 if not
    for (int i=0; i<9; i++) {
        if (listener() == -1) {
            if (i == 8) {
                fprintf(stderr, "listener failed to receive acknowledgement.\n");
                exit(1);
            }
            continue;
        } else {
            break;
        }
    }

    if ((numbytes = sendto (socket_desc, remotePath, strlen(remotePath), 0, p->ai_addr, p->ai_addrlen)) == -1){
        perror("talker: sendto");
        exit(1);
    }

    // receive data
    listener();

    freeaddrinfo(address_resource);

    close(socket_desc);
    return 0;

}
