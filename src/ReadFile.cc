#include "ReadFile/ReadFile.h" 
#include <fstream>

void ReadFileWithRange(const std::string &filePath, const int pos, const int numBytes, char* buf)
{
    std::ifstream file(filePath, std::ios::binary);  // Open in binary mode
    if (!file.is_open()) {
        throw std::runtime_error("failed to open the file at " + filePath);
    }

    // seek to the pos-th position from the beginning
    file.seekg(pos, std::ios::beg);

    // read the next numBytes bytes of characters
    file.read(buf, numBytes);
    file.close();
}

long int getFileSize(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);  // Open at end of file in binary mode

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file & get the size of it. ");
    }

    long long fileSize = file.tellg();  // get position, which is the file size
    file.close();
    return fileSize;
}