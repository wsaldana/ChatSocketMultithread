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

//Actual Date library
#include <time.h>

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
    
    //Obtener DATE
    time_t t = time(NULL);
    time(&t);
    
    char *connection_date = ctime(&t);
    
    char *requestI;
    char privName;
    char status;
    
    //Request Connection
    struct json_object *init_connection = json_object_new_object();
    json_object_object_add(init_connection, "request", json_object_new_string("INIT_CONEX"));
    
    struct json_object *body = json_object_new_array();
    json_object_array_add(body, json_object_new_string(name));
    json_object_array_add(body, json_object_new_string(connection_date));
    json_object_object_add(init_connection, "body", body);
    
    requestI = json_object_to_json_string_ext(init_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    
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
    pthread_create(&tid, NULL,
    		   clienthread,
    		   &requestI);
 
    // Create connection
    // depending on the input
    switch (choice) {
    case 1: {
    	//Request Connection
    	struct json_object *init_connection = json_object_new_object();
    	json_object_object_add(init_connection, "request", json_object_new_string("GET_CHAT"));
    	json_object_object_add(init_connection, "body", json_object_new_string("all"));
    
    	requestI = json_object_to_json_string_ext(init_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    	//printf("jobj from str:\n--\n%s\n--\n", requestI);
    	
        printf("\n\n\t\t\tCHAT GENERAL\n\n\n");
        
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &requestI);
        sleep(20);
        break;
    }
    case 2: {
    	printf("Ingrese usuario con el que quiere comunicarse:\t");
    	scanf("%s", &privName);
    	
    	if(strlen(&privName) > 0){
	    	//Request Connection
	    	struct json_object *init_connection = json_object_new_object();
	    	json_object_object_add(init_connection, "request", json_object_new_string("GET_CHAT"));
	    	json_object_object_add(init_connection, "body", json_object_new_string(&privName));
	    	
	    	requestI = json_object_to_json_string_ext(init_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
	    	//printf("jobj from str:\n--\n%s\n--\n", requestI);

		printf("\n\n\t\t\tCHAT PRIVADO\n\n\n");
		// Create thread
		pthread_create(&tid, NULL,
		       clienthread,
		       &requestI);
		sleep(20);
		break;
		}
    }
    case 3:{
    	printf("Ingrese el nuevo status (0, Activo), (1, Inactivo), (2, Ocupado):\t");
    	scanf("%s", &status);
        
        if(strlen(&status) > 0){
            	//Request Connection
	    	struct json_object *init_connection = json_object_new_object();
	    	json_object_object_add(init_connection, "request", json_object_new_string("PUT_STATUS"));
		json_object_object_add(init_connection, "body", json_object_new_string(&status));
		    	
	    	requestI = json_object_to_json_string_ext(init_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
		//printf("jobj from str:\n--\n%s\n--\n", requestI);
        	printf("\n\n\t\t\tCAMBIO DE STATUS EN PROCESO...\n\n\n");
        
        	// Create thread
        	pthread_create(&tid, NULL,
                       clienthread,
                       &requestI);
        	sleep(20);
        	break;
        }
    }
    case 4:{
    	//Request Connection
    	struct json_object *init_connection = json_object_new_object();
    	json_object_object_add(init_connection, "request", json_object_new_string("GET_USER"));
    	json_object_object_add(init_connection, "body", json_object_new_string("all"));
    
    	requestI = json_object_to_json_string_ext(init_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    	//printf("jobj from str:\n--\n%s\n--\n", requestI);
    	
        printf("\n\n\t\t\tLISTA DE USUARIOS CONECTADOS\n\n\n");
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &requestI);
        sleep(20);
        break;
    }
    case 5:{
        printf("Ingrese usuario del que quiere información:\t");
    	scanf("%s", &privName);
    	
    	if(strlen(&privName) > 0){
	    	//Request Connection
	    	struct json_object *init_connection = json_object_new_object();
	    	json_object_object_add(init_connection, "request", json_object_new_string("GET_USER"));
	    	json_object_object_add(init_connection, "body", json_object_new_string(&privName));
	    	
	    	requestI = json_object_to_json_string_ext(init_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
	    	//printf("jobj from str:\n--\n%s\n--\n", requestI);

		printf("\n\n\t\t\tINFORMACIÓN DE USUARIO\n\n\n");
		// Create thread
		pthread_create(&tid, NULL,
		       clienthread,
		       &requestI);
		sleep(20);
		break;
		}
    }
    case 6:{
        int client_request = 6;
        printf("\n\n\t\t\tMANUAL DE USO\n\n\n");
        printf("Holaaa, bienvenido al CHATROOM de Sistemas Operativos\n");
        printf("Selecciona una opción del Ménu de Opciones desplegado anteriormente\n\n");
	printf("Chatear con todos los usuarios: Podras chatear con todos los usuarios conectados al chatroom.\n\n");
	printf("Chat privado: Podras chatear con un usuario en especifico\n\n");
	printf("Cambiar status: Cambias tu status entre activo(0), inactivo(1), ocupado(2).\n\n");
	printf("Usuarios conectados: Mostrata la lista de todos los usuarios conectados al servidor.\n\n");
	printf("Información de usuario: Mostrara la información de un usuario en especifico.\n\n");
	printf("Desconectarse: Te desconectaras del chatroom.\n\n");
	        
        sleep(20);
        break;
    }
    case 7:{
    	//Request Connection
    	struct json_object *init_connection = json_object_new_object();
    	json_object_object_add(init_connection, "request", json_object_new_string("END_CONEX")); 
    	requestI = json_object_to_json_string_ext(init_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    	//printf("jobj from str:\n--\n%s\n--\n", requestI);
    	
	// Create thread
	pthread_create(&tid, NULL,
	       clienthread,
	       &requestI);
	sleep(20);
	break;
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
