#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<vector<int>> loadFile(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<vector<int>> elves;
    vector<int> calories;
    while(!file.eof()){
        
        string line;
        getline(file, line);
        if(line.length() > 0){
            calories.push_back(stoi(line));
        }
        else{
            elves.push_back(calories);
            calories.clear();
        }
    }
    file.close();
    return elves;
}
int sumVec(const vector<int> & numbs){
    int sum = 0;
    for(int i : numbs){
        sum += i;
    }
    return sum;
}
int part1(const vector<vector<int>> & elves){
    int mostCalories = 0;
    for(auto elf : elves){
        int sum = sumVec(elf); 
        if(sum > mostCalories){
            mostCalories = sum;
        }
    }
    return mostCalories;
}
int part2(const vector<vector<int>> & elves){
    vector<int> calories;
    for(auto elf : elves){
        calories.push_back(sumVec(elf));
    }
    sort(calories.begin(), calories.end(), greater<int>());
    return calories.at(0) + calories.at(1) + calories.at(2);
}


int main(int argc, char const *argv[])
{
    /* code */
    vector<vector<int>> elves = loadFile("./input");
    cout << "Part 1: " << part1(elves) << endl;
    cout << "Part 2: " << part2(elves) << endl;
    return 0;
}
