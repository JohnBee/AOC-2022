#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

struct Rucksack{
    string comp1;
    string comp2;
};

vector<Rucksack> loadFile(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<Rucksack> out;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            int half = line.length() / 2;
            Rucksack r;
            r.comp1 = line.substr(0, half);
            r.comp2 = line.substr(half);
            out.push_back(r);
        }
    }
    file.close();
    return out;
}
vector<vector<string>> loadFilePart2(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    int lineCount=0;
    vector<vector<string>> out;
    vector<string> group;
    while(!file.eof()){
        string line;
        getline(file, line);
        lineCount++;
        if(lineCount % 3 == 0){
            group.push_back(line);
            out.push_back(group);
            group.clear();
        }
        else{
            group.push_back(line);
        }
    }
    file.close();
    return out;
}


int priority(char L){
    if(L >= 65 && L <= 90 ){ // A-Z
        return L-'A' + 27;
    }
    if(L >= 97 && L <= 122){ // a-z
        return L-'a' + 1;
    }
    return -1; // error invalid character 
}
char getSame(Rucksack r){
    for(char c1 : r.comp1){
        if(find(r.comp2.begin(), r.comp2.end(), c1) != r.comp2.end()){
            return c1;
        }
    }
    return 0;
}
int part1(vector<Rucksack> rs){
    int total = 0;
    for(Rucksack r : rs){
        char same = getSame(r);
        total += priority(same);
    }
    return total;
}
int part2(vector<vector<string>> groups){
    int total = 0;
    for(vector<string> g : groups){
        string g1 = g.at(0);
        string g2 = g.at(1);
        string g3 = g.at(2);
        for(char c : g1){
            if(find(g2.begin(), g2.end(), c) != g2.end() && find(g3.begin(), g3.end(), c) != g3.end()){
                total += priority(c);
                break;
            }
        }
    }
    return total;
}

int main(int argc, char const *argv[])
{
    cout << "hello world" << endl;
    vector<Rucksack> rs = loadFile("input");
    cout << "Part 1: " << part1(rs) << endl;
    vector<vector<string>> groups = loadFilePart2("input");
    cout << "Part 2: " << part2(groups) << endl;
    return 0;
}
