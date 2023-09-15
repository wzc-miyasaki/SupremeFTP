#ifndef NETWORK_ANALYZER
#define NETWORK_ANALYZER

#include <netinet/in.h>

#define BUFFER_SIZE 1450    // maximum number of bytes that can be sent without receiving an ACK
#define RTT 11000


void WhatsMyHostname();

// It measures the RTT using UDP. 
double RequestRttMeasure(const int &sockfd, sockaddr_in src, sockaddr_in trg);
void ResponseRttMeasure(const int &sockfd, sockaddr_in trg);

// Fill out the sockaddr_in trg with provided ip and port number
void FillAddress(struct sockaddr_in& trg, const char* ip, const char* port);

#endif