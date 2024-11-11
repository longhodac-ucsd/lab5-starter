#include "http-server.h"
#include <string.h>

int num = 0;

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
char const HTTP_202_OK[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);
    const char* headerNotFound = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
    
    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator

    // TODO: send response back to client?
    write(client_sock, headerNotFound, strlen(headerNotFound));
    write(client_sock, response_buff, strlen(response_buff));
}

void handle_root(int client_sock){
    char message[] = "Got http 200 ok response, current number to come";
    write(client_sock, HTTP_202_OK, strlen(HTTP_202_OK));
    write(client_sock, message, strlen(message));
}

void handle_increment(int client_sock){
    char message[BUFFER_SIZE];
    num += 1;
    snprintf(message, BUFFER_SIZE, "Number incremented to: %dd\n", num);
    write(client_sock, HTTP_202_OK, strlen(HTTP_202_OK));
    write(client_sock, message, strlen(message));
}

void handle_response(char *request, int client_sock) {
    char path[256];

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }

    if (strcmp(path, "/") == 0){
        handle_root(client_sock);
    }
    else if (strcmp(path, "/increment") == 0){
        handle_increment(client_sock);
        return;
    }
    else if (strcmp(path, "/add") == 0){
        printf("Handling add %s\n", path);
        return;
    }
    else{
        handle_404(client_sock, path);
    }
    
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    start_server(&handle_response, port);
}
