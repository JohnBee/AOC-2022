#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>

using namespace std;

vector<string> findRegexString(string regExp, string s){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return foundStr;
}
struct Section {
    int start;
    int stop;
    Section(int start, int stop){
        this->start = start;
        this->stop = stop;
    }
};

vector<pair<Section,Section>> loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<pair<Section,Section>> out;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            vector<string> matchedSections = findRegexString("[0-9]+", line);
            pair<Section, Section> elfPair = {Section(stoi(matchedSections.at(0)), stoi(matchedSections.at(1))), 
                                            Section(stoi(matchedSections.at(2)), stoi(matchedSections.at(3)))};
            out.push_back(elfPair);
        }
           
    }
    file.close();
    return out;
}
int part1(const vector<pair<Section, Section>> & elfPairs){
    int count = 0;
    for(auto elfPair : elfPairs){
        if((elfPair.first.start <= elfPair.second.start && elfPair.first.stop >= elfPair.second.stop) || 
           (elfPair.second.start <= elfPair.first.start && elfPair.second.stop >= elfPair.first.stop)){
            count++;
        }
    }
    return count;
}
int part2(const vector<pair<Section, Section>> & elfPairs){
    int count = 0;
    for(auto elfPair : elfPairs){
        if((elfPair.first.start >= elfPair.second.start && elfPair.first.start <= elfPair.second.stop) ||
            (elfPair.first.stop >= elfPair.second.start && elfPair.first.stop <= elfPair.second.stop) ||
            (elfPair.second.start >= elfPair.first.start && elfPair.second.start <= elfPair.first.stop) ||
            (elfPair.second.stop >= elfPair.first.start && elfPair.second.stop <= elfPair.first.stop)){
            count++;
        }
    }
    return count;
}
int main(int argc, char const *argv[])
{
    vector<pair<Section,Section>> elfPairs = loadInput("./input");
    cout << "Part 1: " << part1(elfPairs) << endl;
    cout << "Part 2: " << part2(elfPairs) << endl;
    return 0;
}
