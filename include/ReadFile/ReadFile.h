#ifndef FILE_READER
#define FILE_READER

#include <string>


// Read numBytes bytes at the (begin + pos) position of the target file 
// located at filePath
void ReadFileWithRange(const std::string &filePath, const int pos, const int numBytes, char* buf);


#endif