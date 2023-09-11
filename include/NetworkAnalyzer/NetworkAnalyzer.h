#ifndef NETWORK_ANALYZER
#define NETWORK_ANALYZER

#include <netinet/in.h>

void WhatsMyHostname();

// It measures the RTT using UDP. 
double RequestRttMeasure(const int &sockfd, sockaddr_in src, sockaddr_in trg);
void ResponseRttMeasure(const int &sockfd, sockaddr_in trg);


#endif