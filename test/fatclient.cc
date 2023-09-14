#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


void CreateTCPAddr(sockaddr_in &trg);
void CreateUDPAddr(sockaddr_in &trg);
int main()
{
    int tcpfd, udpfd;
    tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (tcpfd < 0 || udpfd < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    sockaddr_in tcpServerAddr, udpServerAddr;
    CreateTCPAddr(tcpServerAddr);
    CreateUDPAddr(udpServerAddr);
    
    // connect TCP socket
    if (connect(tcpfd, (struct sockaddr *)&tcpServerAddr, sizeof(tcpServerAddr)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        close(tcpfd);
        close(udpfd);
        return -1;
    }

    // send message
    std::string buffer;
    while(true){
        buffer.clear();
        std::cout << "Enter TCP message: ";
        std::getline(std::cin, buffer);
        // UDP
        sendto(udpfd, buffer.c_str(), buffer.length(), 0, (struct sockaddr *)&udpServerAddr, sizeof(udpServerAddr));

        // TCP
        send(tcpfd, buffer.c_str(), buffer.length(), 0);
    }

    close(tcpfd);
    close(udpfd);
    return 0;
}




// ==============================================
void CreateTCPAddr(sockaddr_in &trg)
{
    trg.sin_family = AF_INET;
    trg.sin_port = htons(9999);  // Example port number
    inet_pton(AF_INET, "127.0.0.1", &trg.sin_addr);  // Connect to localhost for this example
}

void CreateUDPAddr(sockaddr_in &trg)
{
    trg.sin_family = AF_INET;
    trg.sin_port = htons(8888);  // Example port number
    inet_pton(AF_INET, "127.0.0.1", &trg.sin_addr);  // Connect to localhost for this example
}