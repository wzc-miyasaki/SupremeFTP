#include <iostream>
#include <string.h>
#include "NetworkAnalyzer/NetworkAnalyzer.h"
#include "ReadFile/ReadFile.h"


using namespace std;

int main(int argc, char* argv[])
{
    WhatsMyHostname();
    
    char* path = argv[1];
    int readSz = 128;
    char buf[readSz];
    memset(buf, 0, sizeof(buf));
    ReadFileWithRange(path, 0, readSz-1, buf);
    cout << buf << endl;
    cout << getFileSize(path) << endl;

    return 0;
}