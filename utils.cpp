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

int isIndexed(fs::path pitPath, std::string filename){
    
    ifstream indexFile(pitPath / "index", ios::binary);
    int i=0;
    string line;
    while(getline(indexFile, line)){
        int len = line.length();
        if(len > 41){
            if(line.substr(41, len-41) == filename){
                indexFile.close();
                return i;
            }
        }
        i++;
    }
    indexFile.close();
    
    return -1;
}

int indexThisFile(fs::path pitPath, string filename, string hash){
    
    int i = isIndexed(pitPath, filename);

    if(i == -1){
        ofstream indexFile(pitPath / "index", ios::app | ios::binary);
        indexFile<<hash<<" "<<filename<<endl;
        indexFile.close();
    }else{
        int j=0;
        ifstream indexFile(pitPath / "index", ios::binary);
        ofstream tempFile(pitPath / "temp", ios::binary);

        string line;
        while(getline(indexFile, line)){
            if(i==j){
                tempFile<<hash<<" "<<filename<<endl;
            }else{
                tempFile<<line<<endl;
            }
            j++;
        }

        indexFile.close();
        tempFile.close();

        fs::remove(pitPath/"index");
        fs::rename(pitPath/"temp", pitPath/"index");
    }
    
    return 0;
}