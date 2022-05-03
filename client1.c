#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

//JSON Objects
#include <json-c/json.h>

//Actual Date library
#include <time.h>

char *name;
// Requests buffer
char buffer[1024];

//Request Instruction
char *requestI;

// Socket
int sockfd, n;

void error(const char *msg){
    perror(msg);
    exit(1);
}

void* clienthread(void* args){
    int client_request = *((int*)args);
    char privName;
    char status;
    // Send to server
    bzero(buffer, 255);
    // fgets(buffer, 255, stdin);
    
    if(client_request==1){
        //Request GET_CHAT
    	struct json_object *general_chat = json_object_new_object();
    	json_object_object_add(general_chat, "request", json_object_new_string("GET_CHAT"));
    	json_object_object_add(general_chat, "body", json_object_new_string("all"));
    
    	requestI = json_object_to_json_string_ext(general_chat, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
        (void) strncpy(buffer, requestI, strlen(requestI));
        
    }else if(client_request==2){
    	printf("Ingrese usuario con el que quiere comunicarse:\t");
    	scanf("%s", &privName);
    	
    	if(strlen(&privName) > 0){
	    	//Request GET_CHAT
	    	struct json_object *private_chat = json_object_new_object();
	    	json_object_object_add(private_chat, "request", json_object_new_string("GET_CHAT"));
	    	json_object_object_add(private_chat, "body", json_object_new_string(&privName));
	    	
	    	requestI = json_object_to_json_string_ext(private_chat, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

		printf("\n\n\t\t\tCHAT PRIVADO\n\n\n");
		
		requestI = json_object_to_json_string_ext(private_chat, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
        	(void) strncpy(buffer, requestI, strlen(requestI));
        	
		}
    }else if(client_request==3){
	printf("Ingrese el nuevo status (0, Activo), (1, Inactivo), (2, Ocupado):\t");
	scanf("%s", &status);

	if(strlen(&status) > 0){
	
	    	//Request PUT_STATUS
	    	struct json_object *change_status = json_object_new_object();
	    	json_object_object_add(change_status, "request", json_object_new_string("PUT_STATUS"));
		json_object_object_add(change_status, "body", json_object_new_string(&status));
		    	
	    	requestI = json_object_to_json_string_ext(change_status, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
	    	(void) strncpy(buffer, requestI, strlen(requestI));
		//printf("jobj from str:\n--\n%s\n--\n", requestI);
		printf("\n\n\t\t\tCAMBIO DE STATUS EN PROCESO...\n\n\n");
		}

    }else if(client_request == 4){
    	//Request USER_LIST
    	struct json_object *user_list = json_object_new_object();
    	json_object_object_add(user_list, "request", json_object_new_string("GET_USER"));
    	json_object_object_add(user_list, "body", json_object_new_string("all"));
    
    	requestI = json_object_to_json_string_ext(user_list, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    	//printf("jobj from str:\n--\n%s\n--\n", requestI);
    	
        printf("\n\n\t\t\tLISTA DE USUARIOS CONECTADOS\n\n\n");
    	(void) strncpy(buffer, requestI, strlen(requestI));
    
    }else if(client_request == 5){
    	printf("Ingrese usuario del que quiere información:\t");
    	scanf("%s", &privName);
    	
    	if(strlen(&privName) > 0){
	    	//Request USER_INFO
	    	struct json_object *user_info = json_object_new_object();
	    	json_object_object_add(user_info, "request", json_object_new_string("GET_USER"));
	    	json_object_object_add(user_info, "body", json_object_new_string(&privName));
	    	
	    	requestI = json_object_to_json_string_ext(user_info, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
	    	(void) strncpy(buffer, requestI, strlen(requestI));
	    	//printf("jobj from str:\n--\n%s\n--\n", requestI);

		printf("\n\n\t\t\tINFORMACIÓN DE USUARIO\n\n\n");
		}
		
    }else if(client_request == 6){
    	printf("\n\n\t\t\tMANUAL DE USO\n\n\n");
        printf("Holaaa, bienvenido al CHATROOM de Sistemas Operativos\n");
        printf("Selecciona una opción del Ménu de Opciones desplegado anteriormente\n\n");
	printf("Chatear con todos los usuarios: Podras chatear con todos los usuarios conectados al chatroom.\n\n");
	printf("Chat privado: Podras chatear con un usuario en especifico\n\n");
	printf("Cambiar status: Cambias tu status entre activo(0), inactivo(1), ocupado(2).\n\n");
	printf("Usuarios conectados: Mostrata la lista de todos los usuarios conectados al servidor.\n\n");
	printf("Información de usuario: Mostrara la información de un usuario en especifico.\n\n");
	printf("Desconectarse: Te desconectaras del chatroom.\n\n");
	
    }else{
    	//Request ENDING CONNECTION
    	struct json_object *end_connection = json_object_new_object();
    	json_object_object_add(end_connection, "request", json_object_new_string("END_CONEX")); 
    	requestI = json_object_to_json_string_ext(end_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    	(void) strncpy(buffer, requestI, strlen(requestI));
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

    name = (argv[3]);

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

    //Obtener DATE
    time_t t = time(NULL);
    time(&t);
    
    char *connection_date = ctime(&t);
    
    //Request Connection
    struct json_object *init_connection = json_object_new_object();
    json_object_object_add(init_connection, "request", json_object_new_string("INIT_CONEX"));
    
    struct json_object *body = json_object_new_array();
    json_object_array_add(body, json_object_new_string(name));
    json_object_array_add(body, json_object_new_string(connection_date));
    json_object_object_add(init_connection, "body", body);
    
    requestI = json_object_to_json_string_ext(init_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    (void) strncpy(buffer, requestI, strlen(requestI));
    
    n = write(sockfd, buffer, strlen(buffer));
    
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