#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

struct Operation {
    int count;
    int from;
    int to;
    Operation(int count, int from, int to){
        this->count = count;
        this->from = from;
        this->to = to;
    }
};

class CrateArrangement{
    public:
        vector<vector<char>> crates;
        CrateArrangement(vector<vector<char>> crates){
            this->crates = crates;
        }
        void doOperation(Operation op){
            for (size_t i = 0; i < op.count; i++){
                char c = (this->crates).at(op.from-1).back();
                (this->crates).at(op.from-1).pop_back();
                (this->crates).at(op.to-1).push_back(c);
            }
        }
        void doOperationPart2(Operation op){
            vector<char> buffer;
            for (size_t i = 0; i < op.count; i++){
                char c = (this->crates).at(op.from-1).back();
                (this->crates).at(op.from-1).pop_back();
                buffer.push_back(c);
            }
            for (size_t i = 0; i < op.count; i++){
                char c = buffer.back();
                buffer.pop_back();
                (this->crates).at(op.to-1).push_back(c);
            }
        }
        void printCrates(){
            int maxHeight = 0;
            for(auto k : this->crates){
                if(k.size() > maxHeight){
                    maxHeight = k.size();
                }
            }
            for(int row = 0; row < maxHeight; row++){
                for (size_t i = 0; i < this->crates.size(); i++){   
                    if(row < this->crates.at(i).size()){
                        cout << this->crates.at(i).at(row);
                    }
                    else{
                        cout << " ";
                    }
                }
                cout << endl;
            }
            
        }
};

vector<string> findRegexString(string regExp, string s){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return foundStr;
}
pair<CrateArrangement, vector<Operation>> loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    // read the initial arrangement of crates
    vector<vector<char>> arrangedCrates= {{},{},{},{},{},{},{},{},{}};
    vector<Operation> ops;
    while(!file.eof()){
        string line;
        getline(file, line);
        while(!file.eof() && line.size() > 0 && (line.at(0) == '[')){
            //
            vector<string> extractedCrates = findRegexString("(\\[[A-Z]{1}\\])|(([\\s]){4})", line);
            getline(file, line);
            if(extractedCrates.size() > 0){
                for (size_t i = 0; i < extractedCrates.size(); i++){
                    if(extractedCrates.at(i).at(0)=='['){
                        arrangedCrates.at(i).push_back(extractedCrates.at(i).at(1));
                    }
                }
            }
        }
        if(line.size() > 0 && line.at(0) == 'm'){
            vector<string> extractedMoves = findRegexString("[0-9]+", line);
            ops.push_back(Operation(stoi(extractedMoves.at(0)), stoi(extractedMoves.at(1)),stoi(extractedMoves.at(2))));
        }  
    }
    for (size_t i = 0; i < arrangedCrates.size(); i++){
        reverse(arrangedCrates.at(i).begin(), arrangedCrates.at(i).end());
    } 
    file.close();
    return {CrateArrangement(arrangedCrates), ops};
}
string part1(pair<CrateArrangement, vector<Operation>> inp){
    for(Operation op : inp.second){
        inp.first.doOperation(op);
    }
    string out = "";
    for(auto crate : inp.first.crates){
        out.push_back(crate.back());
    }
    return out;
}
string part2(pair<CrateArrangement, vector<Operation>> inp){
    for(Operation op : inp.second){
        inp.first.doOperationPart2(op);
    }
    string out = "";
    for(auto crate : inp.first.crates){
        out.push_back(crate.back());
    }
    return out;
}
int main(int argc, char const *argv[])
{
    pair<CrateArrangement, vector<Operation>> inp = loadInput("./input");
    cout << "Part 1: " << part1(inp) << endl;
    cout << "Part 2: " << part2(inp) << endl;
    return 0;
}
