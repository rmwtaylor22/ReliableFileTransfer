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
#define IS_MULTIPROCESS 1

#define SERVERPORT "4950"   // port that the user will be connecting to

// arg 1 is hostPortValue, arg2 = the *message

int main(int argc, char *argv[])
{
    int socket_desc, accept_desc;
    struct addrinfo hints, *address_resource, *p;
    int rv;
    int numbytes;
    char * buf = "We the People of the United States, in Order to form a more perfect Union, establish Justice, insure domestic Tranquility, provide for the common defence, promote the general Welfare, and secure the Blessings of Liberty to ourselves and our Posterity, do ordain and establish this Constitution for the United States of America.";

    const char * serverVal = argv[1];
    printf("DOCROOT: %s\n", serverVal);

    if (argc != 2) {
        fprintf(stderr, "usage: talker hostname message\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &address_resource)) !=0){
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

    if ((numbytes = sendto (socket_desc, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen)) == -1){
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(address_resource);

    printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(socket_desc);
    return 0;

}
