#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TFTP_PORT 1069
#define TFTP_OPCODE_RRQ 1
#define TFTP_OPCODE_DATA 3
#define TFTP_OPCODE_ACK 4
#define TFTP_BLOCK_SIZE 512

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

    ///////////////////////////////////////////////////// send RRQ to the server (Read Request)
    // RRQ message construction
    unsigned char rrq[2 + strlen(file) + 1 + strlen("octet") + 1];  // Adjust size for both file and mode
    rrq[0] = 0;
    rrq[1] = TFTP_OPCODE_RRQ;  // Opcode for RRQ
    strcpy((char *) &rrq[2], file);  // Copy file name
    rrq[2 + strlen(file)] = 0;  // Null-terminate the file name
    strcpy((char *) &rrq[3 + strlen(file)], "octet");  // Copy mode
    rrq[3 + strlen(file) + strlen("octet")] = 0;  // Null-terminate the mode

    struct sockaddr_in server_addr;
    memcpy(&server_addr, res->ai_addr, res->ai_addrlen);
    server_addr.sin_port = htons(TFTP_PORT);

    if (sendto(sockfd, rrq, sizeof(rrq), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto failed");
        close(sockfd);
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }
    printf("RRQ sent for file: %s\n", file);

    ///////////////////////////////////////////////////// receive DATA packets from the server
    unsigned char buffer[TFTP_BLOCK_SIZE + 4];  // Buffer for receiving DATA
    socklen_t addr_len = sizeof(server_addr);
    int block_num = 1;

    while (1) {
        int received_size = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_len);
        if (received_size == -1) {
            perror("recvfrom failed");
            close(sockfd);
            freeaddrinfo(res);
            exit(EXIT_FAILURE);
        }

        if (buffer[1] == TFTP_OPCODE_DATA && ntohs(*(unsigned short *) &buffer[2]) == block_num) {
            printf("Received block %d with %d bytes of data.\n", block_num, received_size - 4);

            // Check if the file is larger than 512 bytes
            if (received_size > TFTP_BLOCK_SIZE + 4) {
                printf("Error: File is too large, received size exceeds 512 bytes\n");
                break;
            }
            
            // Save the data received
            FILE *file_out = fopen("received_file", "ab");
            if (file_out == NULL) {
                perror("Error opening file to write");
                close(sockfd);
                freeaddrinfo(res);
                exit(EXIT_FAILURE);
            }
            fwrite(&buffer[4], 1, received_size - 4, file_out);
            fclose(file_out);
            
            if (received_size < TFTP_BLOCK_SIZE + 4) {
                break;  // End of file
            }

            ///////////////////////////////////////////////////// send ACK
            unsigned char ack[4];
            ack[0] = 0;
            ack[1] = TFTP_OPCODE_ACK;  // Opcode for ACK
            *(unsigned short *) &ack[2] = htons(block_num);
            if (sendto(sockfd, ack, sizeof(ack), 0, (struct sockaddr *)&server_addr, addr_len) == -1) {
                perror("sendto ACK failed");
                close(sockfd);
                freeaddrinfo(res);
                exit(EXIT_FAILURE);
            }
            printf("ACK sent for block %d\n", block_num);
            block_num++;
        } else if (buffer[1] == 5) {  // ERROR (Opcode 5)
            printf("Error received from server: %s\n", &buffer[4]);
            break;
        }
    }

    ////////////////////////////////////////////////////// close socket
    close(sockfd);
    ///////////////////////////////////////////////////// Free the memory
    freeaddrinfo(res);
    exit(EXIT_SUCCESS);

    return 0; 
}
