#ifndef NETWORK_ANALYZER
#define NETWORK_ANALYZER

#include <netinet/in.h>

void WhatsMyHostname();

// It measures the RTT using UDP. 
double RequestRttMeasure(const int &sockfd, sockaddr_in src, sockaddr_in trg);
void ResponseRttMeasure(const int &sockfd, sockaddr_in trg);

// Fill out the sockaddr_in trg with provided ip and port number
void FillAddress(struct sockaddr_in& trg, const char* ip, const char* port);

#endif