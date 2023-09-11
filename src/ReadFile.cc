#include "ReadFile/ReadFile.h" 
#include <fstream>

void ReadFileWithRange(const std::string &filePath, const int pos, const int numBytes, char* buf)
{
    std::ifstream file(filePath, std::ios::binary);  // Open in binary mode
    if (!file.is_open()) {
        throw std::runtime_error("failed to open the file at " + filePath);
    } else {
        printf("Open %s\n", filePath.c_str());
    }

    // seek to the pos-th position from the beginning
    file.seekg(pos, std::ios::beg);

    // read the next numBytes bytes of characters
    file.read(buf, numBytes);
    file.close();
}