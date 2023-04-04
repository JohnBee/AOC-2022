#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <set>
#include <math.h>
#include <algorithm>

using namespace std;

struct Move {
    char direction;
    unsigned int dist;
};

typedef pair<int,int> coord;

class Rope{
    public:
        coord head;
        vector<coord> tail;
        set<std::pair<int,int>, greater<std::pair<int,int>>> tailPositions;
    Rope(coord start={0,0}, int tailLength=1){
        head = start;
        for(size_t i = 0; i < tailLength; i++){
            tail.push_back(start);
        }
        updatePositions();
    }
    void print(){
        pair<int, int> topLeft = head;
        pair<int,int> bottomRight=head;
        int extra = 5;
        for(size_t i = 0; i < tail.size(); i++){
            coord t = tail.at(i);
            if(t.first <= topLeft.first){
                topLeft.first = t.first;
            }
            if(t.first >= bottomRight.first){
                bottomRight.first = t.first;
            }
            if(t.second <= topLeft.second){
                topLeft.second = t.second;
            }
            if(t.second >= bottomRight.second){
                bottomRight.second = t.second;
            }
        }
        cout << endl;
        for(int y = topLeft.second-extra; y < bottomRight.second+extra; y++){
            for(int x = topLeft.first-extra; x < bottomRight.first+extra; x++){
                if(x == head.first && y == head.second){
                    cout <<  "H";
                }
                else if(find(tail.begin(), tail.end(), pair<int,int>(x,y))!= tail.end()){
                    for(size_t i = 0; i < tail.size(); i++){
                        coord t = tail.at(i);
                        if(x == t.first && y == t.second){
                            cout << i;
                            break;
                        }
                    }
                }
                else{
                    cout << ".";
                }
            }
            cout << endl;
        }
        cout << endl;

    }
    void updatePositions(){
        tailPositions.insert(tail.at(tail.size()-1));
    }
    void updateKnot(coord & knot, coord & following){
        coord & ki = knot;
        coord & fi = following;

        int xDist = ki.first - fi.first;
        int yDist = ki.second - fi.second;

        int distanceTohead = sqrt(xDist*xDist + yDist*yDist);
        if(distanceTohead > 1){ //Moved further away
            // move diagonal first
            // if(head.first != tail.first && head.second != tail.second){
            if(fi.first > ki.first){
                ki = {ki.first+1, ki.second};
            }
            if(fi.first < ki.first){
                ki = {ki.first-1, ki.second};
            }
            if(fi.second > ki.second){
                ki = {ki.first, ki.second+1};
            }
            if(fi.second < ki.second){
                ki = {ki.first, ki.second-1};
            }
            updatePositions();
        }

    }
    void updateTail(){
        // index 0 is the first to follow the head, then propogate
        updateKnot(tail.at(0), head);
        for(size_t i = 1; i < tail.size(); i++){
            updateKnot(tail.at(i), tail.at(i-1));
        }
    }
    void doMove(Move m){
        for(int i = m.dist; i > 0; i--){
            if(m.direction == 'U'){
                head  = {head.first, head.second-1};
            }
            if(m.direction == 'D'){
                head  = {head.first, head.second+1};
            }
            if(m.direction == 'L'){
                head  = {head.first-1, head.second};
            }
            if(m.direction == 'R'){
                head  = {head.first+1, head.second};
            }
            // update tail
            updateTail();
            //print();
        }
    }
};


vector<string> SplitRegexString(string regExp, string s){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, -1), sregex_token_iterator());
    return foundStr;
}

vector<Move> loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<Move> out;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            Move m;
            vector<string> moveReg = SplitRegexString("\\s", line);
            m.direction = moveReg.at(0).at(0);
            m.dist = stoi(moveReg.at(1));
            out.push_back(m);
        }
        

    }
    file.close();
    return out;
}
void printVisited(set<std::pair<int,int>, greater<std::pair<int,int>>> & positions){
    pair<int, int> topLeft = {0,0};
    pair<int,int> bottomRight={0,0};
    for(coord t : positions){
        if(t.first <= topLeft.first){
                topLeft.first = t.first;
        }
        if(t.first >= bottomRight.first){
            bottomRight.first = t.first;
        }
        if(t.second <= topLeft.second){
            topLeft.second = t.second;
        }
        if(t.second >= bottomRight.second){
            bottomRight.second = t.second;
        }
    }
    for(int y = topLeft.second-1; y < bottomRight.second+1; y++){
            for(int x = topLeft.first-1; x < bottomRight.first+1; x++){
                if(find(positions.begin(), positions.end(), pair<int,int>(x,y))!= positions.end()){
                    cout << '#';
                }
                else{
                    cout << ".";
                }
            }
            cout << endl;
        }
        cout << endl;
}

int part1(vector<Move> moves){
    Rope rope;
    for(Move m : moves){
        rope.doMove(m);
    }
    //printVisited(rope.tailPositions);
    return rope.tailPositions.size();
}
int part2(vector<Move> moves){
    Rope rope({0,0},9);
    for(Move m : moves){
        rope.doMove(m);
    }
    // printVisited(rope.tailPositions);
    return rope.tailPositions.size();

}
int main(int argc, char const *argv[]){
    vector<Move> moves = loadInput("./input");
    cout << "Part 1: " << part1(moves) << endl;
    cout << "Part 2: " << part2(moves) << endl;
    return 0;
}