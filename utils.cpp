#include"utils.h"
#include<string>
#include<Windows.h>
#include<fstream>
#include<filesystem>
#include<iostream>

using namespace std;

namespace fs = filesystem;

string getContentsString(ifstream& file){
    string content = "";

    char ch;

    while(file.get(ch)){
        content += ch;
    }

    return content;
}

int createObject(fs::path objects, std::string hash, std::string content){
    if(!fs::exists(objects / hash.substr(0, 2))){
        fs::create_directories(objects / hash.substr(0, 2));
    }

    if(!fs::exists(objects / hash.substr(0, 2) / hash.substr(2, 38))){
        ofstream objFile(objects / hash.substr(0, 2) / hash.substr(2, 38), ios::binary);

        if(objFile.is_open()){
            objFile<<content;
            objFile.close();
        }else{
            cerr<<"Failed to open the hashed file lmaoooo SKILL ISSUE"<<endl<<"File : "<<objects / hash.substr(0, 2) / hash.substr(2, 38)<<endl;
            objFile.close();
            return 1;
        }
    }

    return 0;
}

string readObject(fs::path objects, string hash){
    string content = "";

    ifstream objFile(objects / hash.substr(0, 2) / hash.substr(2, 38));

    if(!objFile.is_open()){
        cerr<<"Can't open the object of the hash "<<hash<<endl;
    }else{
        content = getContentsString(objFile);
    }

    objFile.close();

    return content;
}