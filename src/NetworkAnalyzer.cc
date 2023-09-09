#include "NetworkAnalyzer/NetworkAnalyzer.h"
#include <iostream>
#include <unistd.h>  // for gethostname
#include <cstring>   // for strerror
#include <cerrno>    // for errno

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