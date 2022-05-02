// C program for the Client Side
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
 
// inet_addr
#include <arpa/inet.h>
#include <unistd.h>
 
// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

//JSON Objects
#include <json-c/json.h>

// Function to send data to
// server socket.
void* clienthread(void* args)
{
 
    int client_request = *((int*)args);
    int network_socket;
    struct hostent *server;
 
    // Create a stream socket
    network_socket = socket(AF_INET,
                            SOCK_STREAM, 0);
 
    // Initialise port number and address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    // server_address.sin_addr.s_addr = inet_addr("155.94.218.78");
    server = gethostbyname("8.tcp.ngrok.io");
    if(server == NULL){
        fprintf(stderr, "Error, no such host");
    }
    server_address.sin_addr = *((struct in_addr *)server->h_addr);
    server_address.sin_port = htons(11017);
 
    // Initiate a socket connection
    int connection_status = connect(network_socket,
                                    (struct sockaddr*)&server_address,
                                    sizeof(server_address));
 
    // Check for connection error
    if (connection_status < 0) {
        puts("Error\n");
        return 0;
    }
 
    printf("Connection established\n");
 
    // Send data to the socket
    send(network_socket, &client_request,
         sizeof(client_request), 0);
 
    // Close the connection
    close(network_socket);
    pthread_exit(NULL);
 
    return 0;
}
 
// Driver Code
int main(int argc, char *argv[])
{
    int choice, num, i;
    unsigned long int fact;
    
    //Comprobar que hay 4 parametros
    //if(argv < 4){
    char *name = (argv[1]);
    //	    char *IP = (argv[2]);
    //	    char *port = atoi(argv[3]);
    //}

    
    //Request Connection
    /*
    struct json_object *init_conection = json_object_new_object();
    json_object_object_add(init_connection, "request", json_object_new_string("INIT_CONEX"));
    struct json_object *body = json_object_new_array();
    json_object_array_add(body, json_object_new_string(name));
    json_object_array_add(body, json_object_new_string(connection_date));
    json_object_array_add(init_connection, "body", body);
    */
    
    printf("\n\n\t\tSistemas Operativos Sección 10 Chat\n\n\n");
	
    printf("1. Chatear con todos los usuarios \n");
    printf("2. Chat privado\n");
    printf("3. Cambiar Status\n");
    printf("4. Usuarios conectados\n");
    printf("5. Información de usuario\n");
    printf("6. Ayuda\n");
    printf("7. Desconectarse\n\n\n");
    printf("%s Ingrese opción:  ", name);
    
    // Input
    scanf("%d", &choice);

    pthread_t tid;
 
    // Create connection
    // depending on the input
    switch (choice) {
    case 1: {
        int client_request = 1;
        printf("\n\n\t\t\tCHAT GENERAL\n\n\n");
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    case 2: {
        int client_request = 2;
        printf("\n\n\t\t\tCHAT PRIVADO\n\n\n");
        
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    case 3:{
        int client_request = 3;
        printf("\n\n\t\t\tSTATUS\n\n\n");
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    case 4:{
        int client_request = 4;
        printf("\n\n\t\t\tLISTA DE USUARIOS CONECTADOS\n\n\n");
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    case 5:{
        int client_request = 5;
        printf("\n\n\t\t\tINFORMACIÓN DE USUARIOS\n\n\n");
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    case 6:{
        int client_request = 6;
        printf("\n\n\t\t\tMANUAL DE USO\n\n\n");
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    case 7:{
        int client_request = 7;
        printf("\n\n\t\t\tConectese pronto...\n\n\n");
        exit(0);    // terminates the complete program execution
    }
    default:
        printf("Invalid Input\n");
        break;
    }
 
    // Suspend execution of
    // calling thread
    pthread_join(tid, NULL);
}
