#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <algorithm>
using namespace std;

typedef pair<int,int> coord;

vector<string> findRegexString(string s, string regExp){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return foundStr;
}
coord stringToCoord(string s){
    vector<string> textCoords = findRegexString(s, "[0-9]+");
    return {stoi(textCoords.at(0)), stoi(textCoords.at(1))};
}
void drawLines(vector<coord> & lc, map<coord,int> & sm){
    for(size_t i = 0; (i+1) < lc.size(); i++){
        int sx = lc.at(i).first;
        int sy = lc.at(i).second;
        int ex = lc.at(i+1).first;
        int ey = lc.at(i+1).second;

        if(sx == ex){ // vertical line
            if(sy < ey){
                for(size_t y = sy; y <= ey; y++){
                    sm.insert({coord(sx,y), 1});
                }
            }
            
            if(sy > ey){
                for(size_t y = sy; y >= ey; y--){
                    sm.insert({coord(sx,y), 1});
                }
            }
        }
        if(sy == ey){ //horizontal line
            if(sx < ex){
                for(size_t x = sx; x <= ex; x++){
                    sm.insert({coord(x,sy), 1});
                }
            }
            if(sx > ex){
                for(size_t x = sx; x >= ex; x--){
                    sm.insert({coord(x,sy), 1});
                }
            }
        }
    }
}
map<coord, int> loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    map<coord, int> out;
    out.insert({coord(500,0), 0});
    while(!file.eof()){
        string line;
        getline(file, line);
        vector<string> textCoords = findRegexString(line, "[0-9]+,[0-9]+");
        vector<coord> lineCoords;
        for(string tc : textCoords){
            lineCoords.push_back(stringToCoord(tc));
        }
        drawLines(lineCoords, out);
    }
    file.close();
    return out;
}
coord addCoords(coord x, coord y){
    return {x.first + y.first, x.second + y.second};
}
bool getMapBlocked(map<coord, int> & sm, coord c){
    if(sm.find(c) != sm.end()){
        if(sm.at(c)==1 || sm.at(c)==2){
            return true;
        }
    }
    return false;
}
bool runSand(map<coord, int> & sm, bool part2 = false){
    coord pos = {500,0};
    coord below = {0,1};
    coord downLeft = {-1,1};
    coord downRight = {1,1};
    // get bottom
    int bottom = 0;
    for(auto kv : sm){
        if(kv.first.second > bottom && kv.second == 1){
            bottom = kv.first.second;
        }
    }
    bottom += 2;
    bool sandMoving = true;
    while(sandMoving){
        if(!part2 && pos.second > bottom){
            return false; //sand is leaving the bottom
        }
        if(part2 && addCoords(pos, below) == coord(pos.first, bottom)){
            break;
        }
        // if(part2 && sm.find(pos) != sm.end() && sm.at(pos) == 2){ //entrance is blocked
        //     return false;
        // }
        if(!getMapBlocked(sm, addCoords(pos, below))){
            pos = addCoords(pos, below);
        }
        else if(!getMapBlocked(sm, addCoords(pos, downLeft))){
            pos = addCoords(pos, downLeft);
        }
        else if(!getMapBlocked(sm, addCoords(pos, downRight))){
            pos = addCoords(pos, downRight);
        }
        else{
            break;
        }
    }
    if(pos != coord(500,0)){ //if we're not trying to cover the entrance
        sm.insert({pos, 2}); //fill spot with sand;
    }
    else{
        sm.at(pos) = 2; // entrance is filled with sand
        return false;
    }
    
    return true;
}
void drawMap(map<coord,int> & sm){
    coord topLeft = {INT_MAX, INT_MAX};
    coord bottomRight = {0,0};
    for(auto kv : sm){
        coord c = kv.first;
        if(c.first < topLeft.first) topLeft.first = c.first;
        if(c.first > bottomRight.first) bottomRight.first = c.first;
        if(c.second < topLeft.second) topLeft.second = c.second;
        if(c.second > bottomRight.second) bottomRight.second = c.second;
    }
    cout << endl;
    int buffer = 5;
    for(int y = topLeft.second-buffer; y < bottomRight.second+buffer; y++){
        for(int x = topLeft.first-buffer; x < bottomRight.first+buffer; x++){
            if(sm.find(coord(x,y)) != sm.end()){
                if(sm.at(coord(x,y))==1){
                    cout << '#';
                }
                if(sm.at(coord(x,y))==0){
                    cout << '+';
                }
                if(sm.at(coord(x,y))==2){
                    cout << 'o';
                }
            }
            else{
                cout << '.';
            }
        }
        cout << endl;
    }
}
int part1(map<coord, int> sm){
    int sandCount = 0;
    while(runSand(sm)){
        sandCount++;
    }
    return sandCount;
}
int part2(map<coord, int> sm){
    int sandCount = 0;
    while(runSand(sm, true)){
        sandCount++;
    }
    // drawMap(sm);
    return sandCount+1; // +1 for final sand that covers the entrance
}

int main(int argc, char const *argv[])
{
    map<coord, int> sm = loadInput("./input");
    cout << "Part 1: " << part1(sm) << endl;
    cout << "Part 2: " << part2(sm) << endl;
    return 0;
}
