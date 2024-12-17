#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TFTP_PORT 1069
#define TFTP_OPCODE_RRQ 1   // Opcode for Read Request (RRQ)

int main(int argc, char *argv[]) {
    // a) Construct a properly formed RRQ (Read Request) and send it to the server.
    // Check if the correct number of arguments are passed
    if (argc != 4) {
        printf("Usage: %s <host> <file> <port>\n", argv[0]);
        return 1;
    }
    
    char *host = argv[1];  // Get the server's address (host)
    char *file = argv[2];  // Get the file name to download
    char *port = argv[3];  // Get the port (not used here, as the port is fixed)

    printf("Host: %s, File: %s, Port: %s\n", host, file, port);
    
    ///////////////////////////////////////////////// Get the IP address of the server
    struct addrinfo hints;     
    struct addrinfo *res, *r;  
    int e;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // Use UDP sockets
    hints.ai_protocol = IPPROTO_UDP;  // UDP protocol (optional)
    
    // Get the server address from the hostname
    e = getaddrinfo(argv[1], NULL, &hints, &res);
    
    if (e != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(e));
        exit(EXIT_FAILURE);
    }
    
    // Print the results from the address lookup
    printf("Results for the host: %s\n", argv[1]);
    for (r = res; r != NULL; r = r->ai_next) {
        printf("Family = %d, Type = %d, Protocol = %d\n",
               r->ai_family, r->ai_socktype, r->ai_protocol);
    }

    //////////////////////////////////////// Create the UDP socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Socket creation failed");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    ///////////////////////////////////////////////////// a) Construct the RRQ and send it
    unsigned char rrq[4 + strlen(file) + 1 + 2 + 1];
    rrq[0] = 0;                     // Byte 0: 0 (high byte of the Opcode)
    rrq[1] = TFTP_OPCODE_RRQ;       // Byte 1: 1 (Opcode for RRQ)
    strcpy((char *) &rrq[2], file); // Bytes 2 and onward: file name
    rrq[2 + strlen(file)] = 0;      // Byte following the filename: Null-terminator
    strcpy((char *) &rrq[3 + strlen(file)], "octet");  // Mode "octet" for binary transfer
    rrq[3 + strlen(file) + 5] = 0;  // Null-terminator for the end of the mode string

    // Copy the server address for sending the packet
    struct sockaddr_in server_addr;
    memcpy(&server_addr, res->ai_addr, res->ai_addrlen);
    server_addr.sin_port = htons(TFTP_PORT);  // Set the port for TFTP

    // Send the RRQ to the server
    if (sendto(sockfd, rrq, sizeof(rrq), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to send RRQ");
        close(sockfd);
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }
    printf("RRQ sent for the file: %s\n", file);

    //////////////////////////////////////////////////////// Close the socket and clean up
    close(sockfd);           // Close the socket after sending
    freeaddrinfo(res);       // Free the memory allocated for the address info
    return 0;                // End the program successfully
}
