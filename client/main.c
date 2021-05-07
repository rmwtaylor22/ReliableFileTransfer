#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define CLIENT "10.121.201.7"

#define MAX_BUFFER_SIZE 4096
#define MAX_PAYLOAD_SIZE 1500
#define SPORT "4950"


// arg 1 is hostPortValue, arg2 = the *message

struct packet{
    int seqNum;
    char ack[4];
    char payload[MAX_PAYLOAD_SIZE];
};

int socket_desc;
int init = 1;
int listener();
struct addrinfo hints, *p;
struct addrinfo * address_resource;
struct sockaddr_storage remote_addr;
socklen_t remote_addr_s = sizeof(remote_addr);


void send_it(char *buf, char * PORT){
    int rv;
    struct addrinfo *p;    // get address info
    if(( rv = getaddrinfo(CLIENT, PORT, &hints, &address_resource)) !=0){
        fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }    // links up with the socket
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

    printf("SENT BRO! \n");
    sendto(socket_desc, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen);
}


// Reads the contents
int process_contents(char * PORT)
{
    int num_bytes;
    char buf[MAX_PAYLOAD_SIZE];

    remote_addr_s = sizeof(remote_addr);
    num_bytes = recvfrom(socket_desc, buf, MAX_PAYLOAD_SIZE, 0, (struct sockaddr *) &remote_addr, &remote_addr_s);

    if (num_bytes == -1)
        printf("Error: recvfrom %s (line: %d)\n", strerror(errno), __LINE__);

    printf("Received packet \nPacket is %d long\n", num_bytes);
    buf[num_bytes] = '\0';
    printf("PACKET CONTENTS: %s ", buf);
    printf("%lu", sizeof(buf));
    memset(buf, 0, sizeof(buf));
    FILE *fp;
    char theBuf[1500];
    fp = fopen("file.txt", "w");

    while(num_bytes != -1) {
        memcpy(theBuf, buf, sizeof(theBuf));
        fwrite(theBuf, 1, sizeof(buf), fp);
        listener(PORT);
        num_bytes = recvfrom(socket_desc, buf, MAX_PAYLOAD_SIZE, 0, (struct sockaddr *) &remote_addr, &remote_addr_s);
        send_it("PACK", PORT);
    }
    fclose(fp);


}

// Listens for communication from server
int listener(char * PORT)
{
    int return_value;


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // Listen

    return_value = getaddrinfo(NULL, SPORT, &hints, &address_resource);
    if (return_value != 0) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
    }

    // socket(): creates a new UDP socket, no address or port is assigned yet

    // Loops until client is found
    for  (p = address_resource; p != NULL ; p = p->ai_next) {
        if( (socket_desc = socket(p->ai_family, p->ai_socktype, p->ai_protocol))  == -1) {
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            return -1;
        }
        if (init == 1) {
            if (bind(socket_desc, p->ai_addr, p->ai_addrlen) == -1) {
                close(socket_desc);
                printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
                return -1;
            }
        }
        break;
    }
    // client not found
    if(p == NULL){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return -1;
    }

    freeaddrinfo(address_resource);


    printf("Listener: waiting to recvfrom... \n");
    if(init == 1) {
        init = 0;
        process_contents(PORT);
    }

    return 1;
}



int main(int argc, char *argv[])
{

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int numbytes;

    // File Request: ip address, port, file
    const char * fileReq = "10.121.201.7 4950 /home/CS/users/rwhite/.linux/joshroseproj/rwhite-jmeleski/serverFiles/dictionary.txt";

    // Read in the arg vals from the command line
    const char * serverVal = argv[1];
    printf("SERVER_IP: %s\n", serverVal);
    const char * serverPort = argv[2];
    printf("SERVER_PORT: %s\n", serverPort);
    const char * remotePath = argv[3];
    printf("REMOTE_PATH: %s\n", remotePath);
    const char * localPath = argv[4];
    printf("LOCAL_PATH: %s\n", localPath);

    // Makes sure program received correct # args
    if (argc != 5) {
        fprintf(stderr, "usage: talker hostname message\n");
        exit(1);
    }

    send_it(fileReq, serverPort);

    // listen for the following response
  /*  for (int i=0; i<9; i++) {
        // listener returns -1 if the ACK didn't == "PACK"
        if (listener() == -1) {
            if (i == 8) {
                fprintf(stderr, "listener failed to receive acknowledgement.\n");
                exit(1);
            } else {
                if ((numbytes = sendto(socket_desc, fileReq, strlen(fileReq), 0, p->ai_addr, p->ai_addrlen)) == -1) {
                    perror("talker: sendto (second try)");
                    exit(1);
                }
                printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
            }
        } else {
            break;
        }
    }*/


    // receive data
    listener(serverPort);

  //  freeaddrinfo(address_resource);

    close(socket_desc);
    return 0;

}