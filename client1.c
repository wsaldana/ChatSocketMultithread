#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

// Requests buffer
char buffer[256];

// Socket
int sockfd, n;

void error(const char *msg){
    perror(msg);
    exit(1);
}

void* clienthread(void* args){
    int client_request = *((int*)args);

    // Send to server
    bzero(buffer, 255);
    // fgets(buffer, 255, stdin);
    if(client_request==1){
        char msg[5] = "aaa\n";
        (void) strncpy(buffer, msg, sizeof(msg));
    }else{
        char msg[5] = "bbb\n";
        (void) strncpy(buffer, msg, sizeof(msg));
    }
    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0)
        error("Error on writing");

    // Receive from server
    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    if(n < 0)
        error("Error on reading");
    printf("Server: %s", buffer);

    // Close thread
    pthread_exit(NULL);
    return 0;
}

int main(int argc, char *argv[]){
    int portno, choice;
    struct sockaddr_in server_addr;
    struct hostent *server;
    pthread_t tid;

    if (argc < 4){
        fprintf(stderr, "Usage %s hostname port username\n", argv[0]);
        exit(1);
    }

    char *name = (argv[3]);

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening socket.");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "Error, no such host");
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(portno);
    if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        error("Connection Failed");

    while(1){
        printf("\n\n\t\tSistemas Operativos Sección 10 Chat\n\n\n");
        printf("1. Chatear con todos los usuarios \n");
        printf("2. Chat privado\n");
        printf("3. Cambiar Status\n");
        printf("4. Usuarios conectados\n");
        printf("5. Información de usuario\n");
        printf("6. Ayuda\n");
        printf("7. Desconectarse\n\n\n");
        printf("%s Ingrese opción:  ", name);
        scanf("%d", &choice);

        if(choice == 7)
            break;
        
        pthread_create(&tid, NULL, clienthread, &choice);
    }

    pthread_join(tid, NULL);
    close(sockfd);
    return 0;
}