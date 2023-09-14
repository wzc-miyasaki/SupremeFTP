// Server program
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>

constexpr int UDP_PORT = 8888;
constexpr int TCP_PORT = 9999;


void udp_thread_func() {
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in udp_server_addr{};
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(UDP_PORT);
    udp_server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(udp_socket, reinterpret_cast<sockaddr*>(&udp_server_addr), sizeof(udp_server_addr));

    char buffer[64];
    while (true) {
        // Receive data via UDP
        auto sz = recvfrom(udp_socket, buffer, sizeof(buffer), 0, nullptr, nullptr);
        std::cout << "Received UDP message: " << buffer << std::endl;
        bzero(buffer, sz);
    }

    close(udp_socket);
}

void tcp_thread_func() {
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in tcp_server_addr{};
    tcp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_port = htons(TCP_PORT);
    tcp_server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(tcp_socket, reinterpret_cast<sockaddr*>(&tcp_server_addr), sizeof(tcp_server_addr));
    listen(tcp_socket, 1);

    char buffer[64];
    int client_socket = accept(tcp_socket, nullptr, nullptr);
    // Receive data via TCP
    while(true){
        
        auto sz = recv(client_socket, buffer, sizeof(buffer), 0);
        std::cout << "Received TCP message: " << buffer << std::endl;
        bzero(buffer, sz);
    }
    close(client_socket);

    close(tcp_socket);
}

int main(int argc, char **argv)
{
    
    std::thread udp_thread(udp_thread_func);
    std::thread tcp_thread(tcp_thread_func);

    udp_thread.join();
    tcp_thread.join();

    return 0;

    return 0;
}