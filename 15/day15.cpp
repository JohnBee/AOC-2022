#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <set>

using namespace std;


typedef pair<int, int> coord;

struct BSPair {
    coord sensor;
    coord beacon;
    int manhattan;
    BSPair(){};
    BSPair(coord sensor, coord beacon, int mh){
        this->sensor = sensor;
        this->beacon = beacon;
        manhattan = mh;
    }
};

vector<string> findRegexString(string s, string regExp){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return foundStr;
}
int manhattan(coord a, coord b){
    return abs(a.first - b.first) + abs(a.second - b.second);
}
vector<BSPair> loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<BSPair> out;
    while(!file.eof()){
        string line;
        getline(file, line);
        BSPair bsp;
        if(line.length() > 0){
            vector<string> coordStrings = findRegexString(line, "-?[0-9]+");
            bsp.sensor.first = stoi(coordStrings.at(0));
            bsp.sensor.second = stoi(coordStrings.at(1));
            bsp.beacon.first = stoi(coordStrings.at(2));
            bsp.beacon.second = stoi(coordStrings.at(3));
            bsp.manhattan = manhattan(bsp.sensor, bsp.beacon);
            out.push_back(bsp);
            
        }  
    }
    file.close();
    return out;
}
bool isknownEmpty(coord c, vector<BSPair> bsp){
    for(BSPair p : bsp){
        if(c == p.beacon || c == p.sensor){
            return false;
        }
        if(manhattan(c,p.sensor) <= p.manhattan){
            return true;
        }
    }
    return false;
}
bool isBeaconOrSensor(coord c, vector<BSPair> bsp){
    for(BSPair p : bsp){
        if(c == p.beacon || c == p.sensor){
            return true;
        }
    }
    return false;
}
void drawMap(vector<BSPair> bsp, vector<coord> edges = {}){
    vector<coord> beacons;
    vector<coord> sensors;
    for(auto p : bsp){
        beacons.push_back(p.beacon);
        sensors.push_back(p.sensor);
    }
    for(int y = -2; y < 22; y++){
        cout << y << "\t";
        for(int x = -2; x < 27; x++){
            if(find(beacons.begin(), beacons.end(), coord(x,y))!=beacons.end()){
                cout << 'B';
            }
            else if(find(sensors.begin(), sensors.end(), coord(x,y))!=sensors.end()){
                cout << 'S';
            }
            else if(isknownEmpty(coord(x,y), bsp)){
                cout << '#';
            }
            else if(edges.size() > 0 && find(edges.begin(), edges.end(), coord(x,y))!=edges.end()){
                cout << 'o';
            }
            else{
                cout << '.';
            }
        }
        cout << endl;
    }
}
int part1(vector<BSPair> & bsp, int y){
    
    // find start and end x coord;
    int leftx = INT_MAX;
    int rightx = INT_MIN;
    for(BSPair p : bsp){
        if(p.sensor.second+p.manhattan >= y){
            if(p.sensor.first-p.manhattan < leftx){
                leftx = p.sensor.first-p.manhattan;
            }
            if(p.sensor.first+p.manhattan > rightx){
                rightx = p.sensor.first+p.manhattan;
            }

        }
    }
    int sum = 0;
    for(int x = leftx; x < rightx; x++){
        if(isknownEmpty(coord(x,y), bsp)){
            sum++;
        }
    }
    return sum;
}
unsigned long long part2(vector<BSPair> & bsp){
    // find all perimeters
    for(const BSPair & p : bsp){
        vector<coord> cs;
        for(int y = 0; y <= p.manhattan+1; y++){
            int x = (p.manhattan+1)-y;
            cs.push_back(coord(x,y));
        }
        vector<coord> ncs;
        for(const coord & c : cs){
            ncs.push_back(coord(-1*c.first, c.second));
            ncs.push_back(coord(-1*c.first, -1*c.second));
            ncs.push_back(coord(c.first, -1*c.second));
        }
        for(coord & c : ncs){
            cs.push_back(c);
        }
        for(const auto & ctemp : cs){
            coord c = coord(ctemp.first +  p.sensor.first, ctemp.second + p.sensor.second);
            // all.push_back(coord(c->first + p.sensor.first, c->second + p.sensor.second));
            if(!isknownEmpty(c, bsp) && !isBeaconOrSensor(c, bsp)
                && isknownEmpty(coord(c.first-1, c.second), bsp)
                && isknownEmpty(coord(c.first, c.second-1), bsp)
                && isknownEmpty(coord(c.first+1, c.second), bsp)
                && isknownEmpty(coord(c.first, c.second+1), bsp)){
                    unsigned long long x = c.first;
                    unsigned long long y = c.second;
                    return x*4000000 + y;
                }
        }
    }

    return 0;
}
int main(int argc, char const *argv[])
{
    vector<BSPair> bsp = loadInput("./input");
    cout << "Part 1: " << part1(bsp, 2000000) << endl;
    cout << "Part 2: " << part2(bsp) << endl;
    return 0;
}
