#ifndef main_h
#define main_h

#include <fstream>
#include <string>

void writeFile(std::string& code);
bool readFile(const char* path, std::string& source);

#endif

