#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);  // Example port number
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);  // Connect to localhost for this example

    const char* message = "Hello UDP Server!";
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    std::string buffer;
    while(true){
        buffer.clear();
        std::cout << "Enter message: ";
        std::getline(std::cin, buffer);
        sendto(sockfd, buffer.c_str(), buffer.length(), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    }

    // char buffer[1024] = {0};
    // socklen_t addr_len = sizeof(server_addr);
    // recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_len);
    // std::cout << "Server replied: " << buffer << std::endl;

    close(sockfd);
    return 0;
}
