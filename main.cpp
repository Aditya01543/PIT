#include<iostream>
#include<Windows.h>
#include<fstream>
#include<filesystem>
#include<string>
#include"sha1.h"

using namespace std;

namespace fs = std::filesystem;

fs::path dir = "C:/Aditya/Projects/lab-rat-folder";
fs::path objects = dir / ".pit" / "objects";

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

int hashObject(fs::path filename){
    if(fs::exists(dir / filename)){

        ifstream file(dir / filename, ios::binary);

        if(!file.is_open()){
            cerr<<"Failed to open the file "<<dir/filename<<endl;
            return 1;
        }

        string hashBuffer="";
        char ch;
        
        while(file.get(ch)){
            hashBuffer += ch;
        }

        file.close();

        string hashedStr = sha1(hashBuffer);

        if(!fs::exists(objects / hashedStr.substr(0, 2))){
            fs::create_directory(objects / hashedStr.substr(0, 2));
        }
        
        if(fs::exists(objects / hashedStr.substr(0, 2) / hashedStr.substr(2, 38))){
            cout<<"File "<<dir/filename<<" already hashed"<<endl;
        }else{
            ofstream objFile(objects / hashedStr.substr(0, 2) / hashedStr.substr(2, 38));
            if(objFile.is_open()){
                objFile<<hashBuffer;
                objFile.close();
                cout<<"The file "<<dir/filename<<" hashed and stored with the hash :"<<endl;
                cout<<hashedStr<<endl;
            }else{
                cerr<<"Failed to open the hashed file lmaoooo SKILL ISSUE"<<endl<<"File : "<<objects / hashedStr.substr(0, 2) / hashedStr.substr(2, 38)<<endl;
                objFile.close();
                return 1;
            }
        }

    }else{
        cerr<< "File "<<dir/filename<<" does not exists"<<endl;
        return 1;
    }
    return 0;
}

int catFile(string hash){
    if(hash.length() != 40){
        cerr<<"Invalid hash."<<endl;
        return 1;
    }
    if(fs::exists(objects/hash.substr(0, 2)/hash.substr(2, 38))){
        ifstream hashFile(objects/hash.substr(0, 2)/hash.substr(2, 38), ios::binary);
        
        string content = "";

        if(hashFile.is_open()){
            char ch;
            while(hashFile.get(ch)){
                content+=ch;
            }
            hashFile.close();
        }else{
            cerr<<"Failed to open the object file for hash "<<hash<<endl;
            hashFile.close();
            return 1;
        }

        cout<<endl<<content<<endl;

    }else{
        cerr<<"The object with the hash "<<hash<<" does not exists."<<endl;
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]){
    init();
    hashObject("README.md");
    catFile("bf8e7f16dde9b13469f492478fc9990257f6fd58");
    return 0;
}