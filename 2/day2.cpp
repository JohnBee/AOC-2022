#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <map>

using namespace std;
template <typename T>
struct Move {
    T them;
    T you;
};

vector<Move<char>> loadFile(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<Move<char>> moves;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            Move<char> m;
            m.them = line.at(0);
            m.you = line.at(2); 
            moves.push_back(m);
        }
    }  
    file.close();
    return moves;
}
int whoWins(Move<char> m){
    map<char,char> convert{ {'X','R'},{'Y','P'}, {'Z','S'}, {'A','R'}, {'B','P'}, {'C','S'} };
    map<char, char> beats{ {'R','S'}, {'P', 'R'}, {'S','P'}};
    char them = convert.at(m.them);
    char you = convert.at(m.you);
    if(them == you){
        return 3; // draw
    }
    if(beats.at(them) == you){
        return 0; // you lost
    }
    if(beats.at(you) == them){
        return 6; // you won
    }
    return -1;
}
int score(Move<char> m){
    map<char,int> scoring{{'X', 1}, {'Y', 2}, {'Z', 3}};
    return whoWins(m) + scoring.at(m.you);
}
int part1(const vector<Move<char>> & moves){
    int total = 0;
    for(Move<char> m : moves){
        total+=score(m);
    }
    return total;
}
vector<Move<char>> convertedMoves( const vector<Move<char>> & moves){
    map<char, char> toWin{ {'A','Y'}, {'B', 'Z'}, {'C','X'} };
    map<char, char> toLose{ {'A', 'Z'}, {'B','X'}, {'C','Y'} };
    map<char, char> toDraw{ {'A','X'}, {'B','Y'}, {'C', 'Z'} };
    vector<Move<char>> convMoves;
    for(Move<char> m : moves){
        if(m.you == 'X'){ // lose
            Move<char> n;
            n.them = m.them;
            n.you = toLose.at(m.them);
            convMoves.push_back(n);
        }
        if(m.you == 'Y'){ // draw
            Move<char> n;
            n.them = m.them;
            n.you = toDraw.at(m.them);
            convMoves.push_back(n);
        }
        if(m.you == 'Z'){ // win
            Move<char> n;
            n.them = m.them;
            n.you = toWin.at(m.them);
            convMoves.push_back(n);
        }
    }
    return convMoves;
}
int part2(const vector<Move<char>> & moves){
    vector<Move<char>> convMoves = convertedMoves(moves);
    return part1(convMoves);
}
int main(int argc, char const *argv[])
{
    cout << "Hello world" << endl;
    vector<Move<char>> moves = loadFile("input");
    cout << "Part 1: " << part1(moves) << endl;
    cout << "Part 2: " << part2(moves) << endl;
    return 0;
}
