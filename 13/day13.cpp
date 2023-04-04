#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <memory>
#include <algorithm>
using namespace std;

class Item {
    public:
        string type;
        virtual ~Item(){};
        virtual string toString(){};
        string getType(){return this->type;};
};
class List : public Item {
    public:
        vector<shared_ptr<Item>> items;
        List(){this->type = "List";};
        string toString(){
            string out = "[";
            for(auto a = items.begin(); a != items.end(); a++){
                out.append((*a).get()->toString());
                if((a+1) != items.end()){
                    out.push_back(' ');
                }
            }
            out.push_back(']');
            return out;
        }
        void push_back(shared_ptr<Item> i){
            items.push_back(i);
        }
};
class Number : public Item {
    public:
        int v;
        Number(int v){
            this->v = v;
            this->type = "Number";
        }
        string toString(){
            return to_string(v);
        }
}; 
vector<string> findRegexString(string regExp, string s){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return foundStr;
}

string getNext(vector<string> & s){
    reverse(s.begin(), s.end());
    string out = s.back();
    s.pop_back();
    reverse(s.begin(), s.end());
    return out;
}
bool isNumber(string s){
    try{
        stoi(s);
        return true;
    }
    catch(exception &err){
        return false;
    }
}
shared_ptr<Item> buildList(vector<string> & s, int depth=0){
    string c;
    shared_ptr<List> out = make_shared<List>();
    int d = depth;
    while(s.size() > 0){
        c = getNext(s);
        if(isNumber(c)){
            out.get()->push_back(make_shared<Number>(stoi(c)));
        }
        if(c == "["){
            if(d != 0){ // stop encapsulating base level list
                out.get()->push_back(buildList(s, depth+1));
            }
            d++;
            
        }
        if(c == "]"){
            return out;
        }
    }
    return out;
}
vector<pair<shared_ptr<Item>, shared_ptr<Item>>> loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<pair<shared_ptr<Item>, shared_ptr<Item>>> listTwinPairs;
    while(!file.eof()){
        string reg = "(\\[|\\]|[0-9]+)";
        string line1;
        getline(file,line1);
        vector<string> strFirstList = findRegexString(reg, line1);
        pair<shared_ptr<Item>, shared_ptr<Item>> ab;
        ab.first = buildList(strFirstList);

        string line2;
        getline(file,line2);
        vector<string> strSecondList = findRegexString(reg, line2);
        ab.second = buildList(strSecondList);

        string emptyLine;
        getline(file,emptyLine);


        listTwinPairs.push_back(ab);
    }
    file.close();
    return listTwinPairs;
}
void printPairs(vector<pair<shared_ptr<Item>, shared_ptr<Item>>> listPairs){
    for(pair<shared_ptr<Item>, shared_ptr<Item>> ab : listPairs){
        cout << ab.first->toString() << endl;
        cout << ab.second->toString() << endl;
        cout << endl;
    }
}
int pairCorrect(shared_ptr<Item> left,  shared_ptr<Item> right){ //0 same, 1 less, 2 greater
    // cout << left->toString() << endl;
    // cout << right->toString() << endl;
    if(left->getType()=="Number" && right->getType()=="Number"){
        shared_ptr<Number> a = dynamic_pointer_cast<Number>(left);
        shared_ptr<Number> b = dynamic_pointer_cast<Number>(right);
        if(a->v < b->v){
            return 1;
        }
        if(a->v > b->v){
            return 2;
        }
        return 0; //equal
    }
    if(left->getType()=="Number" && right->getType()=="List"){
        shared_ptr<List> nList = make_shared<List>();
        shared_ptr<Number> a = dynamic_pointer_cast<Number>(left);
        nList->push_back(a);
        return pairCorrect(nList, right);
    }
    if(left->getType()=="List" && right->getType()=="Number"){
        shared_ptr<List> nList = make_shared<List>();
        shared_ptr<Number> a = dynamic_pointer_cast<Number>(right);
        nList->push_back(a);
        return pairCorrect(left, nList);
    }
    // know both are lists
    shared_ptr<List> a = dynamic_pointer_cast<List>(left);
    shared_ptr<List> b = dynamic_pointer_cast<List>(right);
    size_t maxLen = a->items.size();
    if(b->items.size() > maxLen) {maxLen = b->items.size();};
    for(size_t i = 0; i < maxLen; i++){
        if((i+1) > a->items.size()){ // has lhs ran out of items
            return 1;
        }
        if((i+1) > b->items.size()){ // has RHS ran out of items
            return 2;
        }
        shared_ptr<Item> la = a->items.at(i);
        shared_ptr<Item> lb = b->items.at(i);
        if(la->getType()=="Number" && lb->getType()=="Number"){
            shared_ptr<Number> a = dynamic_pointer_cast<Number>(la);
            shared_ptr<Number> b = dynamic_pointer_cast<Number>(lb);
            if(a->v < b->v){
                return 1;
            }
            if(a->v > b->v){
                return 2;
            }
        }
        int correctType = pairCorrect(la, lb);
        if(correctType == 1){
            return 1;
        }
        if(correctType == 2){
            return 2;
        }
    }
    return 0;
}
int part1(vector<pair<shared_ptr<Item>, shared_ptr<Item>>> listPairs){
    int sum = 0;
    for(size_t i = 0; i < listPairs.size(); i++){
        if(pairCorrect(listPairs.at(i).first, listPairs.at(i).second)==1){
            // cout << listPairs.at(i).first->toString() << endl;
            // cout << listPairs.at(i).second->toString() << endl;
            // cout << endl;
            sum+= (i+1);
        }
    }
    return sum;
}
int part2(vector<pair<shared_ptr<Item>, shared_ptr<Item>>> listPairs){
    // bubble sort all packets, insert divider packets
    // big packet list
    vector<shared_ptr<Item>> packetList;
    for(auto pair : listPairs){
        packetList.push_back(pair.first);
        packetList.push_back(pair.second);
    }
    // add the dividers
    string reg = "(\\[|\\]|[0-9]+)";
    vector<string> strDiv1 = findRegexString(reg, "[[2]]");
    vector<string> strDiv2 = findRegexString(reg, "[[6]]");
    packetList.push_back(buildList(strDiv1));
    packetList.push_back(buildList(strDiv2));
    // insertion sort
    for(int i = 0; i < (packetList.size()-1); i++){
        for(int j = i+1; j < packetList.size(); j++){
            if(pairCorrect(packetList.at(i), packetList.at(j)) == 2){
                // swap them i < j
                // shared_ptr<Item> tmp = packetList.at(i);
                // packetList.at(i) = packetList.at(j);
                // packetList.at(j) = tmp;
                swap(packetList.at(i), packetList.at(j));
            }
        }
    }
    // fiind the dividers
    int key = 1;
    for(int i = 0; i < packetList.size(); i++){
        // cout << i << " " << packetList.at(i)->toString() << endl;
        if(packetList.at(i)->toString() == "[[6]]" || packetList.at(i)->toString() == "[[2]]"){
            key *=(i+1);
        }
    }
    return key;
}
int main(int argc, char const *argv[]){
    vector<pair<shared_ptr<Item>, shared_ptr<Item>>> listPairs = loadInput("./input");
    cout << "Part 1: " << part1(listPairs) << endl;
    cout << "Part 2: " << part2(listPairs) << endl;
    return 0;
}
