#include <stdio.h>
#include <string.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
	////////////////////////////////////////////////// get the host adress, the file from the user and the port number
    if (argc != 4) {
        printf("Error\n");
        return 1;
    }
    char *host = argv[1];	// get host and file given by the user
    char *file = argv[2];
    char *port = argv[3];
    printf("Host: %s, File: %s, Port %s\n", host, file, port);
    
    ///////////////////////////////////////////////// get the IP adress of the host
   
	}
	
}
