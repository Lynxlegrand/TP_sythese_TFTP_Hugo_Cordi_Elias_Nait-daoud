#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    ////////////////////////////////////////////////// get the host name, the file from the user and the port number
    if (argc != 4) {
        printf("Error\n");
        return 1;
    }
    char *host = argv[1];    // get host and file given by the user
    char *file = argv[2];
    char *port = argv[3];
    printf("Host: %s, File: %s, Port %s\n", host, file, port);
    
    ///////////////////////////////////////////////// get the IP address of the host
    
    struct addrinfo hints;     
    struct addrinfo *res, *r;  
    int e;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // Use UDP sockets
    hints.ai_protocol = IPPROTO_UDP;  // Protocol UDP (optional)
    
    // Use of getaddrinfo
    e = getaddrinfo(argv[1], NULL, &hints, &res);
    
    if (e != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(e));
        exit(EXIT_FAILURE);
    }
    
    // Show the results
    printf("Results for the host name : %s\n", argv[1]);
    for (r = res; r != NULL; r = r->ai_next) {
        printf("Family = %d, Type = %d, Protocol = %d\n",
               r->ai_family, r->ai_socktype, r->ai_protocol);
    }
        //////////////////////////////////////// create socket
    
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket creation failed");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }
    
    ///////////////////////////////////////////////////// send data to the server
    
    
    ///////////////////////////////////////////////////// receive response from the server
    
    
    
    
    ////////////////////////////////////////////////////// close socket
    close(sockfd);
    ///////////////////////////////////////////////////// Free the memory
    freeaddrinfo(res);
    exit(EXIT_SUCCESS);

    return 0; 
}
