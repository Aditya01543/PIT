#ifndef UTILS_H
#define UTILS_H

#include<string>
#include<fstream>
#include<filesystem>

namespace fs = std::filesystem;

std::string getContentsString(std::ifstream& file);
int createObject(fs::path objects, std::string hash, std::string content);
std::string readObject(fs::path objects, std::string hash);
int isIndexed(fs::path pitPath, std::string filename);
int indexThisFile(fs::path pitPath, std::string filename, std::string hash);

#endif