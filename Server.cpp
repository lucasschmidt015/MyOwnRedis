#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    
    // Connect to the server
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    // Send message to the server
    const char* message = "Hello from client";
    send(sock, message, strlen(message), 0);
    
    // Receive response from the server
    read(sock, buffer, 1024);
    std::cout << "Message from server: " << buffer << std::endl;
    
    // Close socket
    close(sock);
    
    return 0;
}