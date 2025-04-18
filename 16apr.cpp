#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>

#define PORT 8080

int main(){

    int server_fd , client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET , SOCK_STREAM , 0);
    if (server_fd == -1){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&address , 0 ,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr= INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd , (struct sockaddr*)&address , sizeof(address)) < 0){
    
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd , 5) < 0) {

        perror("listen failed");
        exit(EXIT_FAILURE);

    }
    std::cout<<"Server is listening on port" << PORT <<"...\n";

    client_socket = accept(server_fd , (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (client_socket < 0){
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    char buffer[30000] = {0};
    read(client_socket , buffer , 30000);
    std::cout<<"------HTTP Request------\n"<<buffer<<"------------\n";

    std::string response = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, world!";

    send(client_socket , response.c_str() , response.size(), 0);
    std::cout<<"response sent\n";

    close(client_socket);
    close(server_fd);

    return 0;
}
