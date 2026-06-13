#include<iostream>
#include<Windows.h>
#include<fstream>
#include<filesystem>
#include<string>
#include"sha1.h"

using namespace std;

namespace fs = std::filesystem;

fs::path dir = "C:/Aditya/Projects/lab-rat-folder";

void init(){
    if(!fs::exists(dir / ".pit")){
        
        fs::create_directories(dir / ".pit");
        
        if(!fs::exists(dir / ".pit" / "objects")){
            fs::create_directories(dir / ".pit" / "objects");
        }
        if(!fs::exists(dir / ".pit" / "index")){
            fs::create_directories(dir / ".pit" / "index");
        }
    }else{
        cout << "Directory already exists!" << endl;
    }
}

int main(int argc, char* argv[]){
    init();
    return 0;
}