#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Bind to address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // Listen for incoming connections
    listen(server_fd, 3);
    
    // Accept connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    
    // Receive data from the client
    read(new_socket, buffer, 1024);
    std::cout << "Message from client: " << buffer << std::endl;

    // Send response
    const char* response = "Hello from server";
    send(new_socket, response, strlen(response), 0);
    
    // Close socket
    close(new_socket);
    close(server_fd);
    
    return 0;
}