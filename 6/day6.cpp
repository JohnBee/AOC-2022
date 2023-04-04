#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;


string loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    string out;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            out = line;
        }
    }
    file.close();
    return out;
}
template <typename T> 
class Buffer{
    int maxSize;
    vector<T> buffer;
    public:
        Buffer(int maxSize){
            this->maxSize = maxSize;
        }
        void push_back(T item){
            if(buffer.size() < maxSize){
                buffer.push_back(item);
            }
            else{
                reverse(buffer.begin(), buffer.end());
                buffer.pop_back();
                reverse(buffer.begin(), buffer.end());
                buffer.push_back(item);
            }
        }
        vector<T> getBuffer(){
            return buffer;
        }
        size_t size(){
            return buffer.size();
        }
        T at(size_t index){
            return buffer.at(index);
        }
};

bool allDifferent(Buffer<char> b){
    for (size_t i = 0; i < b.size(); i++){
        for (size_t k = i+1; k < b.size(); k++){
            if(b.at(i) == b.at(k)){
                return false;
            }
        }
    }
    return true;
    
}
int part1(string sequence){
    Buffer<char> b(4);
    int charsCount = 0;
    for(char c : sequence){
        b.push_back(c);
        charsCount++;
        if(b.size()==4){
            if(allDifferent(b)){
                return charsCount;
            }
        }
    }
    return -1;
}
int part2(string sequence){
    Buffer<char> b(14);
    int charsCount = 0;
    for(char c : sequence){
        b.push_back(c);
        charsCount++;
        if(b.size()==14){
            if(allDifferent(b)){
                return charsCount;
            }
        }
    }
    return -1;
}
int main(int argc, char const *argv[]){
    string input = loadInput("./input");
    cout << "Part 1: " << part1(input) << endl;
    cout << "Part 2: " << part2(input) << endl;
    return 0;
}
