#include <cstring>
#include <vector>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <limits.h>
using namespace std;

const string lines[] = {"blue1", "blue2", "green1", "green2", "grey", "magenta", "orange", "pink", "red", "violet", "yellow"};

vector<vector<int>> graph;
unordered_map<int, string> idName;
unordered_map<string, int> nameId;
unordered_map<string, vector<int>> lineId;
unordered_map<int, vector<string>> idLine;
unordered_map<string, string> colors;
vector<vector<int>> allPaths;
int totalNumStations;


// ANSI escape codes for colors
void initColors(){
    colors["red"] = "\x1b[38;2;255;0;0m";
    colors["orange"] = "\x1b[38;2;255;165;0m";
    colors["yellow"] = "\x1b[38;2;255;255;0m";
    colors["green"] = "\x1b[38;2;0;255;0m";
    colors["blue"] = "\x1b[38;2;0;0;255m";
    colors["magenta"] = "\x1b[38;2;255;0;255m";
    colors["pink"] = "\x1b[38;2;255;192;203m";
    colors["violet"] = "\x1b[38;2;238;130;238m";
    colors["normal"] = "\x1b[0m";
}


// Computes total number of stations by reading from lines specified in lines[]
// Builds idName and nameId which are key-value pairs
// idName has id, name
// nameId has name, id
// Should preferably be only called once!!
int totalStations(){
    int numberStations = 0;
    string line;
    string lineColor;
    for(string it : lines){
        if(isdigit(it.back())){
            lineColor = it;
        }
        else{
            lineColor = it;
        }

        string name = "data/" + it + ".txt";
        ifstream myline(name.c_str());
        while(getline(myline, line)){
            if(nameId.find(line) == nameId.end()){
                idName[numberStations] = line;
                nameId[line] = numberStations;
                ++numberStations;
            }

        }
        myline.close();
    }
    return numberStations;
}

// Fills graph with zeroes
void fillGraph(){
    int dimensions = totalNumStations;
    vector<int> row(dimensions);
    for(int i = 0; i < dimensions; i++){
        graph.push_back(row);
    }
}

// Makes the undirected graph
// Undirected because we can go to any stations connected to the current station
void addToGraph(){
    fillGraph();
    string line1 = "";
    string line2 = "";
    for(string it: lines){
        string name = "data/" + it + ".txt";
        ifstream myline(name.c_str());
        getline(myline, line1);
        if( find(idLine[nameId[line1]].begin(), idLine[nameId[line1]].end(), it) == idLine[nameId[line1]].end() ){
            idLine[ nameId[line1] ].push_back(it);
            lineId[ line1 ].push_back( nameId[line1] );
        }
        while(getline(myline, line2)){
            idLine[ nameId[line2] ].push_back(it);
            lineId[ line2 ].push_back( nameId[line2] );
            graph[nameId[line1]][nameId[line2]] = 2;
            graph[nameId[line2]][nameId[line1]] = 2;
            line1 = line2;
        }
    }
}

// Prints the graph(matrix actually)
void printGraph(){
    for(auto it1: graph){
        for(auto it2: it1){
            cout << it2;
        }
        cout << endl;
    }
}

void printMap(){
    for(auto it: idName){
        cout << it.first << ":" << it.second << endl;
    }
}

void printVector(vector<int> input){
    for(auto it: input){
        cout << it << " " << idName[it] << " (";
        for(auto color: idLine[it]){
            cout << color << " ";
        }
        cout << ")" << endl;
    }
    cout << "\b\b\b\b    " << endl;
}

void printLineId(){
    for(auto line:lineId){
        cout << line.first << ":";
        for(auto id:line.second){
            cout << id << " ";
        }
        cout << endl;
    }
}

void printIdLine(){
    for(auto it: idLine){
        cout << it.first << ":";
        for(auto it2:it.second){
            cout << it2 << " ";
        }
        cout << endl;
    }
}

// Calculates shortest time path
vector<int> shortestTimePath(int src, int dest){
    // q stores distance, node
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> distances(totalNumStations, INT_MAX);
    vector<int> parent(totalNumStations);
    for(int i = 0; i < totalNumStations; i++)
        parent[i] = i;

    distances[src] = 0;
    pq.push({0, src});
    while(!pq.empty()){
        int currentNode = pq.top().second;
        int currentDis = pq.top().first;
        pq.pop();
        for(int destNode = 0; destNode < totalNumStations; destNode++){
            int distToDest = graph[currentNode][destNode];
            if(distToDest){
                if(distToDest + currentDis < distances[destNode]){
                    distances[destNode] = distToDest + currentDis;
                    pq.push({distToDest+currentDis, destNode});
                    parent[destNode] = currentNode;
                }
            }
        }
    }

    vector<int> path;
    int node = dest;

    while(parent[node] != node){
        path.push_back(node);
        node = parent[node];
    }
    path.push_back(src);
    reverse(path.begin(), path.end());

    return path;
}


// Outputs colored path
void printColoredPath(vector<int> path){
    int stations = path.size();
    string currentLine;
    for(int i = 0; i < stations-1; i++){
        vector<string> currentNodeColors = idLine[path[i]];
        vector<string> nextNodeColors = idLine[path[i+1]];
        for(auto it: currentNodeColors){
            bool notInNext = (find(nextNodeColors.begin(), nextNodeColors.end(), it) == nextNodeColors.end());
            if(!notInNext){
                if(isdigit(it.back())) it = it.substr(0, it.size()-1);
                currentLine = it;
                break;
            }
        }
        cout << colors[currentLine] << idName[path[i]] << colors["normal"] << " -> ";
    }
    cout << colors[currentLine] << idName[path[stations-1]] << colors["normal"] << endl;
    // for last index
}

// Knuth-Morris-Pratt algorithm
// for substring matching
int knuthMorrisPratt(string searchFrom, string stringToSearch) {
    int i = 0, j = 0, m = stringToSearch.length(), n = searchFrom.length();
    stringToSearch = ' ' + stringToSearch; //just shifting the stringToSearch indices by 1
    vector < int > piTable(m + 1, 0);
    for (int i = 2; i <= m; i++) {
        while (j <= m && stringToSearch[j + 1] == stringToSearch[i])
            piTable[i++] = ++j;
        j = 0;
    }
    j = 0;
    for (int i = 0; i < n; i++) {
        if (stringToSearch[j + 1] != searchFrom[i]) {
            while (j != 0 && stringToSearch[j + 1] != searchFrom[i])
                j = piTable[j];
        }
        j++;
        if (j == m) return i - m + 1;
    }
    return -1;

}

// returns all stations matching any substring
void searchName(string toSearch){
    for(auto it: nameId){
        if(knuthMorrisPratt(it.first, toSearch) != -1) cout << it.first << endl;
    }
}

// unused? apparently
pair<int, vector<string>> getLine(int id){
    int numOfLines = idLine[id].size();
    vector<string> theLine = idLine[id];
    vector<string> answer;
    for(auto line: theLine){
        if(isdigit(line.back())) answer.push_back(line.substr(0, line.size()-1));
        else answer.push_back(line);
    }
    return {numOfLines, answer};
}

// fills allPaths with all the possible paths
void makeAllPaths(int src, int dest, vector<int> &visited, vector<int> &path, int path_index){
    visited[src] = 1;
    path[path_index] = src;
    path_index++;

    if(src == dest){
        vector<int> aPath;
        for(int i = 0; i < path_index; i++){
            //cout << path[i] << " ";
            aPath.push_back(path[i]);
        }
        allPaths.push_back(aPath);
        //cout << endl << endl;
    }
    else{
        for(int j = 0; j < totalNumStations; j++){
            if(graph[src][j]){
                if(!visited[j]){
                    makeAllPaths(j, dest, visited, path, path_index);
                }
            }
        }
    }
    path_index--;
    visited[src] = 0;
}

// calls the recursive makeAllPaths
void actuallMakeAllPaths(int src, int dest){
    allPaths.clear();
    allPaths.shrink_to_fit();
    vector<int> visited(totalNumStations);
    vector<int> path(totalNumStations);
    int path_index = 0;
    makeAllPaths(src, dest, visited, path, path_index);
}

// calculates interchanges in a path
int interchanges(vector<int> path){
    int size = path.size();
    for(int id: path){
        int cInterchanges = idLine[id].size();
        size -= cInterchanges;
    }
    size += 1;
    size = -size;
    return size;
}

// return index of minimum interchange path from
// allPaths
vector<int> leastInterchange(){
    int leastIndex = 0;
    for(int i = 0; i < allPaths.size(); i++){
        if(interchanges(allPaths[i]) <= interchanges(allPaths[leastIndex])){
            if(allPaths[i].size() <= allPaths[leastIndex].size()){
                leastIndex = i;
            }
        }
    }
    return allPaths[leastIndex];
}

// initializes buncha stuff
void init(){
    totalNumStations = totalStations();
    initColors();
    addToGraph();
}

void clear(){
    cout << "\x1b[2J\x1b[H";
}

int main(){
    init();
    clear();
    //printLineId();
    //printIdLine();
    //printGraph();
    //printVector(shortestTimePath(0, 16));
    //printColoredPath(shortestTimePath(0, 16));
    //for(auto it:allPaths){
    //    for(auto it2:it){
    //        cout << it2 << " ";
    //    }
    //    cout << endl << endl;
    //}
    //cout <<allPaths.size() << endl;
    //cout << idName[0] << endl << idName[93] << endl;
    //printMap();
    //cout << endl << endl;
    searchName("DWARKA");
    //src = sourceStation();
    //dest = destStation();
    //choice = shortestWhat();
    //givePath(src, dest, choice);
    return 0;
}
