#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <unordered_map>
#include <deque>
#include <numeric>
#include <chrono>

using namespace std;

struct Valve {
    string name;
    int flow;
};
using distanceMap = unordered_map<uint64_t, unordered_map<uint64_t, int>>;

struct ValveSpace {
    // vector<Valve> valves;
    unordered_map<uint64_t, Valve> valves;
    vector<uint64_t> nonZero;
    map<string, vector<string>> adjMap;
    unordered_map<uint64_t, string> idToValve;
    unordered_map<string, uint64_t> valveToId;
};

struct ValveRouteTime {
    Valve start;
    Valve end;
    vector<Valve> route;
    int travelTime;
};

vector<string> findRegexString(string s, string regExp){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return foundStr;
}
vector<string> backtrace(string start, string end, map<string, string> parent){
    vector<string> path = {end};
    while(path.back() != start){
        path.push_back(parent.at(path.back()));
    }
    reverse(path.begin(), path.end());
    return path;

}
vector<string> BFS(string start, string end, map<string, vector<string>> adj){
    map<string, string> parent;
    deque<string> q;
    q.push_back(start);
    vector<string> visited;
    while(q.size()){
        string current = q.front();
        visited.push_back(current);
        q.pop_front();
        if(current == end){
            return backtrace(start, end, parent);
        }
        vector<string> adjacent = adj.at(current);
        for(string a : adjacent){
            if(find(visited.begin(), visited.end(), a)==visited.end() && find(q.begin(), q.end(), a)==q.end()){
                q.push_back(a);
                parent.insert({a,current});
            }
        }
    }
    return {}; // no path

}

distanceMap genAllShortestRoutes(const ValveSpace & vs){
    // map<pair<string, string>, vector<Valve>> out;
    distanceMap distance;
    // only interested in valves that have flow, and the starting valve.
    vector<uint64_t> toVisit(vs.nonZero.begin(), vs.nonZero.end());

    toVisit.push_back(vs.valveToId.at("AA"));
    for(auto v1 = toVisit.begin(); v1 != prev(toVisit.end()); v1++){
        for(auto v2 = next(v1); v2 != toVisit.end(); v2++){
                string v1_name = vs.idToValve.at(*v1);
                string v2_name = vs.idToValve.at(*v2);
                vector<string> routes = BFS(v1_name, v2_name, vs.adjMap);
                vector<Valve> valveRoutes;
                for(string valveString : routes){
                    valveRoutes.push_back(vs.valves.at(vs.valveToId.at(valveString)));
                }

                distance[*v1][*v2] = valveRoutes.size();
                distance[*v2][*v1] = valveRoutes.size();
        }

    }
    return distance;
}

ValveSpace loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    ValveSpace out;
    uint64_t id = 1;

    while(!file.eof()){
        string line;
        getline(file, line);    
        if(line.length() > 0){
            vector<string> strValves = findRegexString(line, "[A-Z]{2}");
            vector<string> strFlow = findRegexString(line, "[0-9]+");
            vector<string> strAdjacentValves;
            copy(next(strValves.begin()), strValves.end(), back_inserter(strAdjacentValves));
            out.adjMap.insert({strValves.at(0), strAdjacentValves});
            Valve v;
            v.name = strValves.at(0);
            v.flow = stoi(strFlow.at(0));
            out.idToValve[id] = v.name;
            out.valveToId[v.name] = id;
            // out.valves.push_back(v);
            out.valves.insert({id, v});
            id = id << 1;
        }
        
    }
    file.close();
    for(auto v : out.valves){
        if(v.second.flow > 0){
            out.nonZero.push_back(v.first);
        }
    }
    return out;
}
void dfs(const distanceMap & dm, const ValveSpace & vs, uint64_t current, uint64_t visited, int flow, int timeLeft, int & best){
        visited += current;
        flow += timeLeft * vs.valves.at(current).flow;
        for(auto & to_kv : dm.at(current)){
            if(((to_kv.first & visited) == 0) && (timeLeft - to_kv.second >= 0)){
                dfs(dm, vs, to_kv.first,visited, flow, timeLeft-to_kv.second, best);
            }
        }
        if(flow > best){
            best = flow;
        }
}
void dfs_limited(const distanceMap & dm, const ValveSpace & vs, uint64_t current, uint64_t visited, uint64_t & canVisit, int flow, int timeLeft, int & best){
        visited += current;
        flow += timeLeft * vs.valves.at(current).flow;
        for(auto & to_kv : dm.at(current)){
            if(((to_kv.first & visited) == 0) && (timeLeft - to_kv.second >= 0) && ((to_kv.first & canVisit) != 0)){
                dfs_limited(dm, vs, to_kv.first, visited, canVisit, flow, timeLeft-to_kv.second, best);
            }
            else{
                // visited all or out of time
                if(flow > best){
                    best = flow;
                }
            }
        }
}
int part1(const distanceMap & dm, const ValveSpace & vs){
    int bestFlow = 0; 
    dfs(dm, vs, vs.valveToId.at("AA"), {}, 0, 30, bestFlow);  
    return bestFlow;
}
void sortedPermIndices(vector<vector<int>> & permutations, vector<int> currentPerm, vector<int> availableIndices, int permSize, int depth = 0){
    /*
        Generates the sorted permutations of a set size for indices for a list.
    */
    if(depth >= permSize){
        permutations.push_back(currentPerm);
        return;
    }
    for(size_t iter = 0; iter < availableIndices.size(); iter++){
        vector<int> curP = currentPerm;
        curP.push_back(availableIndices.at(iter));
        vector<int> indicesLeft(availableIndices.begin() + iter + 1, availableIndices.end());
        sortedPermIndices(permutations, curP, indicesLeft, permSize, depth+1);
    }
}
int part2(const distanceMap & dm, const ValveSpace & vs){
    //get all sets of splitting the turn on valves in to 2 sets, one for elf one for elephant.
    //Calculate the optimal pressure released per set and sum them up. Largest is answer.

    
    int best = 0;
    vector<vector<Valve>> elephant;
    vector<vector<Valve>> elf;

    int totalBest = 0;

    int nonZeroSize = vs.nonZero.size();


    vector<int> indices(nonZeroSize);
    iota(indices.begin(), indices.end(), 0);
    vector<vector<int>> perms;
    vector<Valve> bestElfSplit;
    vector<Valve> bestElephantSplit;
    for(int iter = 1; iter < nonZeroSize; iter++){
        sortedPermIndices(perms, {}, indices, iter);
    }

    for(vector<int> perm : perms){
        // build elf & elephant
        uint64_t elfCanVisit = 0;
        uint64_t elephantCanVisit = 0;
        for(int iter = 0; iter < nonZeroSize; iter++){
            if(find(perm.begin(), perm.end(), iter) != perm.end()){
                elfCanVisit += vs.nonZero.at(iter);
            }
            else{
                elephantCanVisit += vs.nonZero.at(iter);
            }
        }
        int elfBest = 0;
        int elephantBest = 0;
        
        dfs_limited(dm, vs, vs.valveToId.at("AA"), 0, elfCanVisit, 0, 26, elfBest);
        dfs_limited(dm, vs, vs.valveToId.at("AA"), 0, elephantCanVisit, 0, 26, elephantBest);

        if(elfBest + elephantBest > totalBest){
            totalBest = elfBest + elephantBest;
        }
            
    }
    
    return totalBest;

}
int main(int argc, char const *argv[]){
    ValveSpace vs = loadInput("./input");
    distanceMap dm = genAllShortestRoutes(vs);

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;


    cout << "Calculating Part 1..." << endl;
    auto t1 = high_resolution_clock::now();
    int p1 = part1(dm, vs);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    cout << "Part 1: " << p1 << " Calculated in " << ms_double.count() << "ms\n" << endl;


    cout << "Calculating Part 2..." << endl;
    t1 = high_resolution_clock::now();
    int p2 = part2(dm, vs);
    t2 = high_resolution_clock::now();
    ms_double = t2 - t1;
    cout << "Part 2: " << p2 << " Calculated in " << ms_double.count() << "ms\n" << endl;

    cout << "Done" << endl;
    return 0;
}
