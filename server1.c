#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <json-c/json.h>

// Messages
char *msgs[100][3];

// Users
char *users[100][4];

// Socket
int sockfd, newsockfd, n;

void error(const char *msg){
    perror(msg);
    exit(1);
}

void* serverthread(void* args){
    char buffer[1000];
    int mysockfd = *((int*)args);
    struct json_object *parsed_json;
    struct json_object *requestMade;
    struct json_object *body;
    char *resI;

    char fd[5];
    sprintf(fd, "%d", mysockfd); 
    printf("%d", mysockfd); 
    while(1){
        // Read from client
        bzero(buffer, 1000);
        n = read(mysockfd, buffer, 999);
        if(n < 0)
            error("Error on reading");
        printf("Client: %s\n", buffer);
	
        if(buffer != ""){
            parsed_json = json_tokener_parse(buffer);
            json_object_object_get_ex(parsed_json, "request", &requestMade);
            json_object_object_get_ex(parsed_json, "body", &body);
            char req[256];
            strcpy(req, json_object_get_string(requestMade));
            
            if (strcmp(req, "INIT_CONEX") == 0){
                for(int i=0; i<100; i++){
                    if(users[i][0] == NULL){
                        users[i][0] = fd;
                        users[i][1] = json_object_get_string(json_object_array_get_idx(body, 0));
                        users[i][2] = "189.10.0.1";
                        users[i][3] = "0";
                        break;
                    }
                }
                struct json_object *res_init_conex = json_object_new_object();
                json_object_object_add(res_init_conex, "response", json_object_new_string("INIT_CONEX"));
                json_object_object_add(res_init_conex, "code", json_object_new_int(200));
                resI = json_object_to_json_string_ext(res_init_conex, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

            }else if(strcmp(req, "GET_CHAT") == 0){
                struct json_object *res_get_chat = json_object_new_object();
                json_object_object_add(res_get_chat, "response", json_object_new_string("GET_CHAT"));
                json_object_object_add(res_get_chat, "code", json_object_new_int(200));
                struct json_object *body = json_object_new_array();
                for(int i=0; i<100; i++){
                    if(msgs[i][0] != NULL){
                        struct json_object *message = json_object_new_array();
                        json_object_array_add(message, json_object_new_string(msgs[i][0]));
                        json_object_array_add(message, json_object_new_string(msgs[i][1]));
                        json_object_array_add(message, json_object_new_string(msgs[i][2]));

                        json_object_array_add(body, message);
                    }
                }
                json_object_object_add(res_get_chat, "body", body);
                
                resI = json_object_to_json_string_ext(res_get_chat, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
                
                if (strcmp(json_object_get_string(body), "all") == 0){
                    printf("all");
                }else{
                    printf("username");
                }
                
            }else if(strcmp(req, "POST_CHAT") == 0){
                for(int i=0; i<100;  i++){
                    if(msgs[i][0] == NULL){
                        msgs[i][0] = json_object_get_string(json_object_array_get_idx(body, 0));
                        msgs[i][1] = json_object_get_string(json_object_array_get_idx(body, 1));
                        msgs[i][2] = json_object_get_string(json_object_array_get_idx(body, 2));
                        break;
                    }
                }
                struct json_object *res_init_conex = json_object_new_object();
                json_object_object_add(res_init_conex, "response", json_object_new_string("INIT_CONEX"));
                json_object_object_add(res_init_conex, "code", json_object_new_int(200));
                resI = json_object_to_json_string_ext(res_init_conex, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
                
            }else if(strcmp(req, "GET_USER") == 0){
                char b[256];
                strcpy(b, json_object_get_string(body));

                struct json_object *res_get_user = json_object_new_object();
                json_object_object_add(res_get_user, "response", json_object_new_string("GET_USER"));
                json_object_object_add(res_get_user, "code", json_object_new_int(200));
                struct json_object *body = json_object_new_array();
                for(int i=0; i<100; i++){
                    if(users[i][0] != NULL){
                        if (strcmp(b, "all") == 0){
                            struct json_object *usr = json_object_new_array();
                            json_object_array_add(usr, json_object_new_string(users[i][1]));
                            json_object_array_add(usr, json_object_new_string(users[i][3]));

                            json_object_array_add(body, usr);
                        }else{
                            if(strcmp(b, users[i][0]) == 0){
                                json_object_array_add(body, json_object_new_string(users[i][2]));
                                json_object_array_add(body, json_object_new_string(users[i][3]));
                            }
                        }
                    }
                }
                json_object_object_add(res_get_user, "body", body);
                
                resI = json_object_to_json_string_ext(res_get_user, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
                
            }else if(strcmp(req, "PUT_STATUS") == 0){
                for(int i=0; i<100;  i++){
                    if(strcmp(users[i][0], fd) == 0){
                        users[i][3] = json_object_get_string(body);
                        break;
                    }
                }
                struct json_object *res_init_conex = json_object_new_object();
                json_object_object_add(res_init_conex, "response", json_object_new_string("PUT_STATUS"));
                json_object_object_add(res_init_conex, "code", json_object_new_int(200));
                resI = json_object_to_json_string_ext(res_init_conex, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);            
            }
            else if(strcmp(req, "END_CONEX") == 0){
                for(int i=0; i<100;  i++){
                    if(strcmp(users[i][0], fd) == 0){
                        users[i][3] = "1";
                        n = write(mysockfd, "Sesion finalizada", strlen("Sesion finalizada"));
                        break;
                    }
                }
                break;             
            }
        }

        bzero(buffer, 1000);
        n = write(mysockfd, resI, strlen(resI));
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