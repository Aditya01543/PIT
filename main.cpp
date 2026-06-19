#include<iostream>
#include<Windows.h>
#include<fstream>
#include<filesystem>
#include<string>
#include"sha1.h"
#include"utils.h"

using namespace std;

namespace fs = std::filesystem;

fs::path dir = "C:/Aditya/Projects/lab-rat-folder";
fs::path objects = dir / ".pit" / "objects";
fs::path heads = dir / ".pit" / "refs" / "heads";

void init(){
    if(!fs::exists(dir / ".pit")){
        
        fs::create_directories(dir / ".pit");
        
        if(!fs::exists(dir / ".pit" / "objects")){
            fs::create_directories(dir / ".pit" / "objects");
        }
        if(!fs::exists(dir / ".pit" / "index")){
            ofstream indexFile(dir / ".pit" / "index");
            indexFile.close();
        }
        if(!fs::exists(dir / ".pit" / "refs" / "heads")){
            fs::create_directories(dir / ".pit" / "refs" / "heads");
        }
        if(!fs::exists(heads / "main")){
            ofstream mainFile(heads / "main");
            mainFile.close();
        }
        if(!fs::exists(dir / ".pit" / "HEAD")){
            ofstream HEADfile(dir / ".pit" / "HEAD");
            HEADfile.close();
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

        string hashBuffer=getContentsString(file);

        file.close();

        string hashedStr = sha1(hashBuffer);

        createObject(objects, hashedStr, hashBuffer);

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
                
        string content = readObject(objects, hash);

        cout<<endl<<content<<endl;

    }else{
        cerr<<"The object with the hash "<<hash<<" does not exists."<<endl;
        return 1;
    }

    return 0;
}

int addFile(fs::path filename){
    if(fs::exists(dir / filename)){

        ifstream file(dir / filename, ios::binary);

        if(!file.is_open()){
            cerr<<"Failed to open the file "<<dir/filename<<endl;
            return 1;
        }

        string hashBuffer = getContentsString(file);

        file.close();

        string hashedStr = sha1(hashBuffer);

        if(createObject(objects, hashedStr, hashBuffer) == 0){
            if(!fs::exists(dir/".pit"/"index")){
                ofstream indexFile(dir/".pit"/"index");
                indexFile.close();
            }
            indexThisFile(dir/".pit", (dir/filename).string(), hashedStr);
        }
        
    }else{
        cerr<<"The file "<<dir/filename<<" does not exists.";
        return 1;
    }

    return 0;
}

int writeTree(){
    if(!fs::exists(dir/".pit"/"index")){
        cerr<<"BRUH THE INDEX FILE AIN'T THERE?????"<<endl;
        return -1;
    }

    ifstream indexFile(dir/".pit"/"index", ios::binary);
    string tree = getContentsString(indexFile);
    string hashed = sha1(tree);

    if(createObject(objects, hashed, tree) == -1)return -1;
    cout<<hashed<<endl;
    return 0;
}

int commit(string message){
    if(!fs::exists(dir/".pit"/"index")){
        cerr<<"BRUH THE INDEX FILE AIN'T THERE?????"<<endl;
        return -1;
    }

    ifstream indexFile(dir/".pit"/"index", ios::binary);
    string tree = getContentsString(indexFile);
    string hashed = sha1(tree);

    if(createObject(objects, hashed, tree) == -1)return -1;
    
    ifstream mainBranch(heads/"main", ios::binary);
    string parentCommitHash;
    getline(mainBranch, parentCommitHash);
    mainBranch.close();

    string parentIndexTree = " ", parentCommit = "";

    if(parentCommitHash.length() == 40){
        parentCommit = readObject(objects, parentCommitHash);
    }

    if(parentCommit.length() > 83){
        parentIndexTree = parentCommit.substr(42, 40);
        if(parentIndexTree == hashed)return 0;
    }
    
    string commit = hashed + "\n" + parentCommitHash + "\n" + message + "\n";
    string commitHash = sha1(commit);
    createObject(objects, commitHash, commit);

    ofstream mainBranchFile(heads/"main", ios::binary);
    mainBranchFile<<commitHash<<endl;
    mainBranchFile.close();
    
    return 0;
}

int main(int argc, char* argv[]){
    init();
    hashObject("README.md");
    addFile("README.md");
    addFile("rat-1.py");
    writeTree();
    commit("Commit 1");
    return 0;
}