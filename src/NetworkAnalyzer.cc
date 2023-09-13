#include "NetworkAnalyzer/NetworkAnalyzer.h"
#include <iostream>
#include <unistd.h>  // for gethostname
#include <cstring>   // for strerror
#include <cerrno>    // for errno
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>

void WhatsMyHostname()
{
    const int bufferSize = 256;
    char buffer[bufferSize];

    if (gethostname(buffer, bufferSize) == 0) {
        std::cout << "Hostname: " << buffer << std::endl;
    } else {
        std::cerr << "Error retrieving hostname: " << strerror(errno) << std::endl;
    }
}


double RequestRttMeasure(const int &sockfd, sockaddr_in src, sockaddr_in trg)
{
    // Measure the RTT
    char buffer[] = "ping";
    socklen_t length = sizeof(src);

    auto start = std::chrono::high_resolution_clock::now();
    int status = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &trg, sizeof(trg));
    status = recvfrom(sockfd, &(buffer), sizeof(buffer), 0, (struct sockaddr *) &src, (socklen_t *) &length);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

void ResponseRttMeasure(const int &sockfd, sockaddr_in trg)
{
    // Measure the RTT
    char buffer[] = "ping";
    socklen_t length = sizeof(trg);

    int status = recvfrom(sockfd, &(buffer), sizeof(buffer), 0, (struct sockaddr *) &trg, (socklen_t *) &length);
    status = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &trg, sizeof(trg));
}

void FillAddress(struct sockaddr_in& trg, const char* ip, const char* port)
{
    trg.sin_family = AF_INET;
    trg.sin_port = htons(atoi(port));
    trg.sin_addr.s_addr = inet_addr(ip);
}