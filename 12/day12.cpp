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

typedef pair<int, int> coord;

struct MInput {
    vector<vector<char>> hmap;
    coord start;
    coord end;
};

MInput loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    MInput out;
    int x,y = 0;
    while(!file.eof()){
        string line;
        getline(file, line);
        vector<char> mline;
        
        if(line.length() > 0){
            x = 0;
            for(char c : line){
                
                if(c == 'S'){
                    out.start = {x,y};
                    mline.push_back('a');
                }
                else if(c == 'E'){
                    out.end = {x,y};
                    mline.push_back('z');
                }
                else{
                    mline.push_back(c);
                }
                x++;
            }
            out.hmap.push_back(mline);
            y++;
        }
    }
    file.close();
    return out;
}
vector<coord> adjacent(coord current, MInput mi){
    int maxWidth = mi.hmap.at(0).size();
    int maxHeight = mi.hmap.size();
    vector<coord> check = {{0,-1},{0,1},{-1,0},{1,0}};
    vector<coord> out;
    for(coord c : check){
        coord nCurrent = {current.first + c.first, current.second + c.second};
        // checks bounds
        if(nCurrent.first >= 0 && nCurrent.first < maxWidth &&
            nCurrent.second >= 0 && nCurrent.second < maxHeight){
                char currentChar = mi.hmap.at(current.second).at(current.first);
                char next = mi.hmap.at(nCurrent.second).at(nCurrent.first);
                if(next-currentChar == 1 || next <= currentChar){
                    out.push_back(nCurrent);
                }
        }
    }
    return out;
}
vector<coord> genPath(map<coord,coord> & parent, coord current){
    coord c = current;
    vector<coord> path = {current};
    while(parent.find(c) != parent.end()){
        c = parent.at(c);
        path.push_back(c);
    }
    reverse(path.begin(), path.end());
    return path;
}
void printExplored(MInput mi, vector<coord> & explored){
    int maxWidth = mi.hmap.at(0).size();
    int maxHeight = mi.hmap.size();
    for(int y = 0; y < maxHeight; y++){
        for(int x = 0; x < maxWidth; x++){
            coord c = {x,y};
            if(find(explored.begin(), explored.end(), c)!=explored.end()){
                cout << '.';
            }
            else{
                cout << mi.hmap.at(y).at(x);
            }
        }
        cout << endl;
    }

}
vector<coord> BFS(const MInput & mi, coord start={-1,-1}){
    // queue<pair<coord,int>> Q;
    auto cmp = [](pair<coord,int> left, pair<coord,int> right) {return left.second > right.second;};
    queue<coord> Q;
    coord root;
    if(start == pair<int,int>(-1,-1)){
        root = mi.start;
    }
    else{
        root = start;
    }
    
    coord goal = mi.end;
    vector<coord> explored;
    explored.push_back(root);
    Q.push(root);
    map<coord,coord> parent;
    vector<coord> path;
    
    while(Q.size() > 0){
        coord current = Q.front();
        Q.pop();
        if(current == goal){
            // printExplored(mi, explored);
            return genPath(parent, current);
        }
        vector<coord> adj = adjacent(current, mi);
        for(coord a : adj){
            if(find(explored.begin(), explored.end(), a)==explored.end()){
                Q.push(a);
                explored.push_back(a);
                parent.insert({a,current});
            }
        }
    }
    // printExplored(mi, explored);
    return {}; //failed to find path
}


int part1(MInput mi){
    return BFS(mi).size()-1; //Counting 'steps' so -1
}
int distToTarget(coord s, coord e){
    int xDist = s.first-e.first;
    int yDist = s.second-e.second;
    return sqrt(xDist*xDist + yDist*yDist);
}
vector<coord> BFSExtended(const MInput & mi, vector<coord> & noRoute, coord start={-1,-1}){
    // queue<pair<coord,int>> Q;
    auto cmp = [](pair<coord,int> left, pair<coord,int> right) {return left.second > right.second;};
    queue<coord> Q;
    coord root;
    if(start == pair<int,int>(-1,-1)){
        root = mi.start;
    }
    else{
        root = start;
    }
    
    coord goal = mi.end;
    vector<coord> explored;
    explored.push_back(root);
    Q.push(root);
    map<coord,coord> parent;
    vector<coord> path;
    int depth = 0;
    
    while(Q.size() > 0){
        coord current = Q.front();
        Q.pop();
        if(find(noRoute.begin(), noRoute.end(), current)!=noRoute.end()){
            continue; //skip this vertices there is no route to the exit;
        }
        if(current == goal){
            // printExplored(mi, explored);
            return genPath(parent, current);
        }
        vector<coord> adj = adjacent(current, mi);
        for(coord a : adj){
            if(find(explored.begin(), explored.end(), a)==explored.end()){
                Q.push(a);
                explored.push_back(a);
                parent.insert({a,current});
            }
        }
    }
    // printExplored(mi, explored);
    return {}; //failed to find path
}


int part2(MInput mi){
    // find all 'a'
    vector<pair<coord, int>> starts;
    
    int maxWidth = mi.hmap.at(0).size();
    int maxHeight = mi.hmap.size();
    for(int y = 0; y < maxHeight; y++){
        for(int x = 0; x < maxWidth; x++){
            // only add starts if it has 'a' or 'b' next to it
            if(mi.hmap.at(y).at(x) == 'a'){
                coord curr = {x,y};
                vector<coord> adj = adjacent(curr, mi);
                for(coord c : adj){
                    if(mi.hmap.at(c.second).at(c.first)=='a' || mi.hmap.at(c.second).at(c.first)=='b'){
                        starts.push_back({{x,y},distToTarget({x,y},mi.end)});
                        break;
                    }
                }
                
            }
        }
    }
    // cout << starts.size() << endl;
    vector<int> pathLengths;
    // int checked = 0;
    vector<coord> noRoute;
    // start closest to end
    sort(starts.begin(), starts.end(), [](pair<coord, int> a, pair<coord, int> b){return a.second < b.second;} );
    for(pair<coord, int> start : starts){
        vector<coord> path = BFSExtended(mi, noRoute, start.first);
        if(path.size() > 0){
            // checked++;
            // cout << checked << " " << path.size() <<  endl;
            pathLengths.push_back(path.size());
        }
        else{
            noRoute.push_back(start.first);
        }
        
    }
    // cout << "Done" << endl;
    sort(pathLengths.begin(), pathLengths.end());
    return pathLengths.at(0)-1;

}
int main(int argc, char const *argv[])
{
    MInput mi = loadInput("./input");
    // part1(mi);
    cout << "Part 1: " << part1(mi) << endl;
    cout << "Part 2: " << part2(mi) << endl;
    return 0;
}
