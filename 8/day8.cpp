#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Forest{
    vector<vector<int>> trees;
    public:
        Forest(vector<vector<int>> trees){
            this->trees = trees;
        }
        int at(size_t x, size_t y) const{
            return trees.at(y).at(x);
        }
        pair<size_t,size_t> size() const{
            return {trees.at(0).size(), trees.size()};
        }
};

Forest loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<vector<int>> trees;
    while(!file.eof()){
        string line;
        getline(file, line);
        vector<int> treeLine;
        if(line.length() > 0){
            for(char c : line){
                treeLine.push_back(c-48);
            }
            trees.push_back(treeLine);
        }
        
    }
    file.close();
    return Forest(trees);
}
bool upVisible(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT, int height){
    for(int y_c = y-1; y_c >= 0; y_c--){
        if(fr.at(x,y_c) >= height){
            return false;
         }
    }
    return true;
}
bool downVisible(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT, int height){
    for(int y_c = y+1; y_c < MAX_HEIGHT; y_c++){
        if(fr.at(x,y_c) >= height){
            return false;
        }
    }
    return true;
}
bool leftVisible(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT, int height){
    for(int x_c = x-1; x_c >= 0; x_c--){
        if(fr.at(x_c,y) >= height){
            return false;
        }
    }
    return true;
}
bool rightVisible(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT, int height){
    for(int x_c = x+1; x_c < MAX_WIDTH; x_c++){
        if(fr.at(x_c,y) >= height){
            return false;
        }
    }
    return true;
}
bool isVisible(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT){
    // on the edges
    int height = fr.at(x,y);
    bool visible = true;
    if(x == 0 || y == 0 || x == MAX_WIDTH || y == MAX_HEIGHT){
        return true;
    }
    // up
    bool up = upVisible(fr, x, y, MAX_WIDTH, MAX_HEIGHT, height);
    // down
    bool down = downVisible(fr, x, y, MAX_WIDTH, MAX_HEIGHT, height);
    // left
    bool left = leftVisible(fr, x, y, MAX_WIDTH, MAX_HEIGHT, height);
    // right
    bool right = rightVisible(fr, x, y, MAX_WIDTH, MAX_HEIGHT, height);
    return up || down || left || right;
}
int part1(const Forest & fr){
    size_t MAX_WIDTH = fr.size().first;
    size_t MAX_HEIGHT = fr.size().second;
    int sum = 0;
    for (size_t y = 0; y < MAX_HEIGHT; y++){
        for(size_t x = 0; x < MAX_WIDTH; x++){
            if(isVisible(fr, x, y, MAX_WIDTH, MAX_HEIGHT)){
                sum++;
            }
        }
    }
    return sum;
}
int upScore(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT){
    int height = fr.at(x,y);
    int score = 0;
    for(int y_c = y-1; y_c >= 0; y_c--){
        if(fr.at(x,y_c) < height){
            score++;
        }
        else{
            score++;
            break;
        }
    }
    return score;
}
int downScore(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT){
    int height = fr.at(x,y);
    int score = 0;
    for(int y_c = y+1; y_c < MAX_HEIGHT; y_c++){
        if(fr.at(x,y_c) < height){
            score++;
        }
        else{
            score++;
            break;
        }
    }
    return score;
}
int rightScore(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT){
    int height = fr.at(x,y);
    int score = 0;
    for(int x_c = x+1; x_c < MAX_WIDTH; x_c++){
        if(fr.at(x_c,y) < height){
            score++;
        }
        else{
            score++;
            break;
        }
    }
    return score;
}
int leftScore(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT){
    int height = fr.at(x,y);
    int score = 0;
    for(int x_c = x-1; x_c >= 0; x_c--){
        if(fr.at(x_c,y) < height){
            score++;
        }
        else{
            score++;
            break;
        }
    }
    return score;
}
int calcScenicScore(const Forest & fr, size_t x, size_t y, size_t MAX_WIDTH, size_t MAX_HEIGHT){
    int left = leftScore(fr, x, y, MAX_WIDTH, MAX_HEIGHT);
    int right = rightScore(fr, x, y, MAX_WIDTH, MAX_HEIGHT);
    int up = upScore(fr, x, y, MAX_WIDTH, MAX_HEIGHT);
    int down = downScore(fr, x, y, MAX_WIDTH, MAX_HEIGHT);
    return left * right * up * down;
    
}
int part2(const Forest & fr){
    size_t MAX_WIDTH = fr.size().first;
    size_t MAX_HEIGHT = fr.size().second;
    vector<int> scenicScores;
    for (size_t y = 0; y < MAX_HEIGHT; y++){
        for(size_t x = 0; x < MAX_WIDTH; x++){
            scenicScores.push_back(calcScenicScore(fr, x, y, MAX_WIDTH, MAX_HEIGHT));
        }
    }
    sort(scenicScores.begin(), scenicScores.end(), greater<int>());
    return scenicScores.front();
}

int main(int argc, char const *argv[]){
    Forest fr = loadInput("./input");
    cout << "Part 1: " << part1(fr) << endl; 
    cout << "Part 2: " << part2(fr) << endl;
    return 0;
}
