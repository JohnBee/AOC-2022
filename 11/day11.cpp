#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <functional>
#include <queue>
#include <map>
#include <math.h>

using namespace std;

typedef unsigned long long ullong;


struct Monkey{
        size_t id;
        queue<ullong> items;
        function<ullong(ullong)> operation;
        function<bool(ullong)> divisor;
        size_t passTrue;
        size_t passFalse;
        ullong divisorVal;
        size_t test(ullong item){
            if(divisor(item)){
                return passTrue;
            }
            else{
                return passFalse;
            }
        }
};

vector<string> findRegexString(string regExp, string s){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return foundStr;
}

vector<Monkey> loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<Monkey> monkeys;
    while(!file.eof()){
        string line;
        // read 6 lines for monkey info
        vector<string> monkeyLines;
        for(int i = 0; i < 6; i++){
            getline(file, line);
            monkeyLines.push_back(line);
        }
        getline(file,line); //blank line;
        // monkey id;
        Monkey m;
        int id = stoi(findRegexString("[0-9]+", monkeyLines.at(0)).at(0));
        m.id = id;
        // starting items
        vector<string>strItems = findRegexString("[0-9]+", monkeyLines.at(1));
        for(string strIt : strItems){
            m.items.push(stoi(strIt));
        }
        // operation
        vector<string> strOp = findRegexString("(old)|[+*]|([0-9]+)", monkeyLines.at(2));
        if(strOp.at(1) =="+"){
            ullong val = stoi(strOp.at(2));
            m.operation = [val](ullong x){return x+val;};
        }
        else{// *
            if(strOp.at(2)=="old"){
                m.operation = [](ullong x){return x*x;};
            }
            else{
                ullong val = stoi(strOp.at(2));
                m.operation = [val](ullong x){return x*val;};
            }
        }
        // divisible by x
        vector<string> strDivisor = findRegexString("[0-9]+", monkeyLines.at(3));
        int divisorVal = stoi(strDivisor.at(0));
        m.divisorVal = divisorVal;
        m.divisor = [divisorVal](ullong x){return x % divisorVal == 0;};
        // if true
        vector<string> strTrueId = findRegexString("[0-9]+", monkeyLines.at(4));
        m.passTrue = stoi(strTrueId.at(0));
        // if false
        vector<string> strFalseId = findRegexString("[0-9]+", monkeyLines.at(5));
        m.passFalse = stoi(strFalseId.at(0));

        monkeys.push_back(m);

    }
    file.close();
    return monkeys;
}
ullong part1(vector<Monkey> monkeys){
    // do rounds
    map<size_t,ullong> monkeyInspections;
    for(int round = 0; round < 20; round++){
        for(size_t mId = 0; mId < monkeys.size(); mId++){
            Monkey & m = monkeys.at(mId);
            while(m.items.size() > 0){
                ullong item = m.items.front();
                m.items.pop();
                item = m.operation(item);
                item /= 3;
                size_t throwTo = m.test(item);
                monkeys.at(throwTo).items.push(item);

                //add inspection to count
                if(monkeyInspections.find(mId)!=monkeyInspections.end()){
                    monkeyInspections.at(mId)++;
                }
                else{
                    monkeyInspections.insert({mId, 1});
                }
            }
            
        }
    }
    // find two monkeys that sorted most
    vector<ullong> ranking;
    for(pair<size_t,ullong> kv : monkeyInspections){
        ranking.push_back(kv.second);
    }
    sort(ranking.begin(), ranking.end(), greater<ullong>());
    return ranking.at(0)*ranking.at(1);
}
ullong part2(vector<Monkey> monkeys){
    // calculate modulo
    ullong modulo = 1;
    for(Monkey m : monkeys){
        modulo *= m.divisorVal; 
    }
    map<size_t,ullong> monkeyInspections;
    for(int round = 0; round < 10000; round++){
        for(size_t mId = 0; mId < monkeys.size(); mId++){
            Monkey & m = monkeys.at(mId);
            while(m.items.size() > 0){
                ullong item = m.items.front();
                m.items.pop();
                item = m.operation(item);
                item %= modulo;
                
                
                size_t throwTo = m.test(item);
                monkeys.at(throwTo).items.push(item);

                //add inspection to count
                if(monkeyInspections.find(mId)!=monkeyInspections.end()){
                    monkeyInspections.at(mId)++;
                }
                else{
                    monkeyInspections.insert({mId, 1});
                }
            }
            
        }
    }
    // find two monkeys that sorted most
    vector<ullong> ranking;
    for(pair<size_t,ullong> kv : monkeyInspections){
        ranking.push_back(kv.second);
    }
    sort(ranking.begin(), ranking.end(), greater<ullong>());
    return ranking.at(0)*ranking.at(1);
}

int main(int argc, char const *argv[]){

    vector<Monkey> monkeys = loadInput("./input");
    cout << "Part 1: " << part1(monkeys) << endl;
    cout << "Part 2: " << part2(monkeys) << endl;
    return 0;
}
