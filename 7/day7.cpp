#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>

using namespace std;

typedef unsigned long ulong;

class Filespace{
    public:
        string type = "NONE";
        string name;
        Filespace * parent;
        ulong size;
        Filespace(string name, Filespace * parent, ulong size=0){
            this->name = name;
            this->parent = parent;
            this->size = size;
        }
        virtual void ls(int depth=0){
            for(int i = 0; i < depth; i++){
                cout << "\t";
            }
            cout << name << " " << size << endl;
        }
        virtual Filespace * getFilespace(string name){
            return nullptr;
        }
};
class File : public Filespace{
    public:
        string type = "FILE";
        ulong size;
        File(string name, ulong size) : Filespace(name, nullptr, size){
        }
};
class Directory : public Filespace{
    public:
        string type = "DIR";
        vector<Filespace *> files;
        Directory(string name, Filespace * parent) : Filespace(name, parent){
            // if(name != ".." && name != "/"){
            //     files.push_back(new Directory("..", parent));
            // }
        }
        void addFilespace(Filespace * fs){
            // dont add files that already exist
            if(find_if(files.begin(), files.end(), [fs](Filespace* f){
                return (f->name == fs->name);
            })==files.end()){
                files.push_back(fs); // file doesn't exist, add it.
            }

        }
        void ls(int depth=0){
            for(int i = 0; i < depth; i++){
                cout << "\t";
            }
            cout << "DIR - " + this->name << " " << size << endl;
            for(Filespace * file: files){
                file->ls(depth+1);
            }
        }
        Filespace * getFilespace(string name){
            if(name == ".."){
                return this->parent;
            }
            for(Filespace * fs : files){
                if(fs->name == name){
                    return fs;
                }
            }
            return nullptr;
        }
};
vector<string> splitString(string regExp, string s){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, -1), sregex_token_iterator());
    return foundStr;
}

vector<string> loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<string> out;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            out.push_back(line);
        }
    }
    file.close();
    return out;
}
Directory buildFilespace(vector<string> calls){
    Directory root("/", nullptr);
    Directory * currentDirectory = &root;
    for(string call : calls){
        vector<string> args = splitString("\\s+", call);
        if(args.at(0) == "$"){ //command
            if(args.at(1) == "cd"){
                if(args.at(2) == "/"){
                    currentDirectory = &root;
                    continue;
                }
                string dirName = args.at(2);
                currentDirectory = dynamic_cast<Directory*>(currentDirectory->getFilespace(dirName));
                if(currentDirectory == nullptr){
                    throw runtime_error("Cannot find directory " + dirName);
                }
            }
            if(args.at(1) == "ls"){};
        }
        else if(args.at(0) == "dir"){
            string name = args.at(1);
            currentDirectory->addFilespace(new Directory(name, currentDirectory));
        }
        else{ // must be a file
            ulong size = stoul(args.at(0));
            string name = args.at(1);
            currentDirectory->addFilespace(new File(name, size));
        }
    }
    return root;
}
ulong calculateSize(Filespace * dr){
    Directory * ds = dynamic_cast<Directory*>(dr);
    if(ds == nullptr){ // fail to cast, is file.
        return dr->size;
    }
    ulong sum = 0;
    for(Filespace * file : ds->files){
        sum += calculateSize(file);
    }
    dr->size = sum;
    return sum;
}
void sumSize(Filespace * dr, vector<ulong> & sizes, ulong sizeCap=100000){
    Directory * ds = dynamic_cast<Directory*>(dr);
    if(ds == nullptr){ // fail to cast, is file.
        return;
    }
    if(ds->size <= sizeCap){
        sizes.push_back(ds->size);
    }
    for(Filespace * file : ds->files){
        sumSize(file, sizes, sizeCap);
    }
    return;
}
void sumSizePart2(Filespace * dr, vector<ulong> & sizes, ulong sizeMin=100000){
    Directory * ds = dynamic_cast<Directory*>(dr);
    if(ds == nullptr){ // fail to cast, is file.
        return;
    }
    if(ds->size >= sizeMin){
        sizes.push_back(ds->size);
    }
    for(Filespace * file : ds->files){
        sumSizePart2(file, sizes, sizeMin);
    }
    return;
}
ulong part1(Directory * dr){
    vector<ulong> sizes;
    sumSize(dr, sizes);
    ulong sum;
    for(ulong size : sizes){
        sum+= size;
    }
    return sum;
}
ulong part2(Directory * dr){
    ulong sizeMin = 30000000 - (70000000 - dr->size);
    vector<ulong> sizes;
    sumSizePart2(dr, sizes, sizeMin);
    sort(sizes.begin(), sizes.end());
    return sizes.at(0);
}
int main(int argc, char const *argv[]){
    vector<string> input = loadInput("./input");
    Directory dr = buildFilespace(input);
    // dr.ls();
    calculateSize(&dr);
    dr.ls();
    cout << "Part 1: " << part1(&dr) << endl;
    cout << "Part 2: " << part2(&dr) << endl;
    return 0;
}
