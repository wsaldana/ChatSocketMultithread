#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

// Requests buffer
//char buffer[256];

// Messages
char *msgs[100][3];

// Users
char *users[100][3];

// Socket
int sockfd, newsockfd, n;

void error(const char *msg){
    perror(msg);
    exit(1);
}

void* serverthread(void* args){
    char buffer[256];
    int mysockfd = *((int*)args);
    
    while(1){
        // Read from client
        bzero(buffer, 256);
        n = read(mysockfd, buffer, 255);
        if(n < 0)
            error("Error on reading");
        printf("Client: %s\n", buffer);

        // send to client
        bzero(buffer, 255);
        char msg[5] = "Hola\n";
        (void) strncpy(buffer, msg, sizeof(msg));

        n = write(mysockfd, buffer, strlen(buffer));
        if(n < 0)
            error("Error on writing");
    }

    // Close thread
    pthread_exit(NULL);
    return 0;
}

int main(int arc, char *argv[]){
    pthread_t tid;

    if(arc< 2){
        fprintf(stderr, "Port Nomber Not Provided. Program terminated\n");
        exit(1);
    }
    int portno, n;
    //char buffer[255];

    struct sockaddr_in server_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening socket.");
    }

    bzero((char *) &server_addr, sizeof(server_addr));

    portno = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        error("Binding Failed.");
    
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if(newsockfd < 0)
            error("Error on Accept");
        pthread_create(&tid, NULL, serverthread, &newsockfd);
    }

    pthread_join(tid, NULL);
    close(newsockfd);
    close(sockfd);
    return 0;
}