#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>



int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);  // Example port number
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);  // Connect to localhost for this example

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        return -1;
    }

    const char* message = "Hello TCP Server!";
    send(sockfd, message, strlen(message), 0);

    std::string buffer;
    while(true){
        buffer.clear();
        std::cout << "Enter message: ";
        std::getline(std::cin, buffer);
        send(sockfd, buffer.c_str(), buffer.length(), 0);
    }

    // char buffer[1024] = {0};
    // recv(sockfd, buffer, sizeof(buffer), 0);
    // std::cout << "Server replied: " << buffer << std::endl;

    close(sockfd);
    return 0;
}
