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
struct hostent *server;
struct sockaddr_in server_addr;
int portno;

void error(const char *msg){
    perror(msg);
    exit(1);
}


char sendServer(char *jsonMsg){
	bzero(buffer, 1000);
    (void) strncpy(buffer, jsonMsg, strlen(jsonMsg));
    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0)
        error("Error on writing");
        
    // Receive response from server
    bzero(buffer, 1000);
    n = read(sockfd, buffer, 1000);
    if(n < 0)
        error("Error on reading");
        	
   	printf("Server: %s", buffer);
}

void GetAllChats(){
    char msg[150];
    printf("Mensaje (ingrese '*' para salir): ");
    //fgets(msg, 150, stdin);
    scanf("%s", &msg);

    if (strlen(msg) > 0){
        //Obtener DATE
        time_t t = time(NULL);
        time(&t);
        char *deliver_date = ctime(&t);
        //Request POST_CHAT
        struct json_object *post_chat = json_object_new_object();
        json_object_object_add(post_chat, "request", json_object_new_string("POST_CHAT"));
        
        struct json_object *body = json_object_new_array();
        json_object_array_add(body, json_object_new_string(msg));
        json_object_array_add(body, json_object_new_string(name));
        json_object_array_add(body, json_object_new_string(deliver_date));
        json_object_array_add(body, json_object_new_string("all"));
        
        json_object_object_add(post_chat, "body", body);
        requestI = json_object_to_json_string_ext(post_chat, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
        sendServer(requestI);
    }

    //Request GET_CHAT
    struct json_object *general_chat = json_object_new_object();
    json_object_object_add(general_chat, "request", json_object_new_string("GET_CHAT"));
    json_object_object_add(general_chat, "body", json_object_new_string("all"));
    requestI = json_object_to_json_string_ext(general_chat, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    sendServer(requestI);
}

void GetPrivateChat(){
    char privName[150];
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
    sendServer(requestI);    
    }
}

void ChangeStatus(){
    char status[100];
	printf("Ingrese el nuevo status (0, Activo), (1, Inactivo), (2, Ocupado):\t");
	scanf("%s", &status);

	if(strlen(&status) > 0){
        //Request PUT_STATUS
        struct json_object *change_status = json_object_new_object();
        json_object_object_add(change_status, "request", json_object_new_string("PUT_STATUS"));
		json_object_object_add(change_status, "body", json_object_new_string(&status));
		    	
        requestI = json_object_to_json_string_ext(change_status, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
        sendServer(requestI);
		//printf("jobj from str:\n--\n%s\n--\n", requestI);
		printf("\n\n\t\t\tCAMBIO DE STATUS EN PROCESO...\n\n\n");
	}
}

void UsersList(){
	//Request USER_LIST
    struct json_object *user_list = json_object_new_object();
    json_object_object_add(user_list, "request", json_object_new_string("GET_USER"));
    json_object_object_add(user_list, "body", json_object_new_string("all"));

    requestI = json_object_to_json_string_ext(user_list, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    //printf("jobj from str:\n--\n%s\n--\n", requestI);
    sendServer(requestI);
    printf("\n\n\t\t\tLISTA DE USUARIOS CONECTADOS\n\n\n");
}

void UserInfo(){
	char privName[150];
	printf("Ingrese usuario del que quiere información:\t");
    scanf("%s", &privName);
    
    if(strlen(&privName) > 0){
        //Request USER_INFO
        struct json_object *user_info = json_object_new_object();
        json_object_object_add(user_info, "request", json_object_new_string("GET_USER"));
        json_object_object_add(user_info, "body", json_object_new_string(&privName));
        
        requestI = json_object_to_json_string_ext(user_info, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
        sendServer(requestI);
        //printf("jobj from str:\n--\n%s\n--\n", requestI);

        printf("\n\n\t\t\tINFORMACIÓN DE USUARIO\n\n\n");
    }
}

void Manual(){
	printf("\n\n\t\t\tMANUAL DE USO\n\n\n");
    printf("Holaaa, bienvenido al CHATROOM de Sistemas Operativos\n");
    printf("Selecciona una opción del Ménu de Opciones desplegado anteriormente\n\n");
	printf("Chatear con todos los usuarios: Podras chatear con todos los usuarios conectados al chatroom.\n\n");
	printf("Chat privado: Podras chatear con un usuario en especifico\n\n");
	printf("Cambiar status: Cambias tu status entre activo(0), inactivo(1), ocupado(2).\n\n");
	printf("Usuarios conectados: Mostrata la lista de todos los usuarios conectados al servidor.\n\n");
	printf("Información de usuario: Mostrara la información de un usuario en especifico.\n\n");
	printf("Desconectarse: Te desconectaras del chatroom.\n\n");
}

void Disconnect(){
	//Request ENDING CONNECTION
    struct json_object *end_connection = json_object_new_object();
    json_object_object_add(end_connection, "request", json_object_new_string("END_CONEX")); 
    requestI = json_object_to_json_string_ext(end_connection, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
    sendServer(requestI);
    printf("\nTe has desconectado\n");
    exit(0);
}

int createNewSocket(char *ip, int nport){
    int network_socket;
 
    // Create a stream socket
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
 
    // Initialise port number and address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(nport);
 
    // Initiate a socket connection
    int connection_status = connect(
        network_socket,
        (struct sockaddr*)&server_address,
        sizeof(server_address)
    );
 
    // Check for connection error
    if (connection_status < 0) {
        printf("Error connecting to socket at %s\n", ip);
        return -1;
    }

    return network_socket;
}

int createNewSocketHostname(char *hostname, int nport){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening socket.");
    }

    server = gethostbyname(hostname);
    if(server == NULL){
        fprintf(stderr, "Error, no such host");
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(nport);
    if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        error("Connection Failed");
    
    return sockfd;
}

int main(int argc, char *argv[]){
    int choice;

    if (argc < 5){
        fprintf(stderr, "Usage %s IP hostname port username\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[3]);
    name = (argv[4]);

    if(strcmp(argv[1], "0") != 0)
        sockfd = createNewSocket(argv[1], portno);
    else if(strcmp(argv[2], "0") != 0)
        sockfd = createNewSocketHostname(argv[2], portno);
    else
        error("Socket not found\n");

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
    sendServer(requestI);
    
    /*Check REQUEST parsed
    struct json_object *parsed_json;
    struct json_object *requestMade;
    parsed_json = json_tokener_parse(requestI);
    json_object_object_get_ex(parsed_json, "request", &requestMade);
    
    printf("Request made: %s", json_object_get_string(requestMade));
    */
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

	    switch(choice){
		case 1:
		GetAllChats();	
		break;
		
		case 2:
		GetPrivateChat();
		break;
		
		case 3:
		ChangeStatus();
		break;
		
		case 4:
		UsersList();
		break;
		
		case 5:
		UserInfo();
		break;
		
		case 6:
		Manual();
		break;
		
		case 7:
		Disconnect();
		break;

		}
    }

    close(sockfd);
    return 0;
}