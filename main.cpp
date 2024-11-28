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
int totalNumStations;

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

// Makes the undirected graph
// Undirected because we can go to any stations connected to the current station
void addToGraph(){
    string line1;
    string line2;
    for(string it: lines){
        string name = "data/" + it + ".txt";
        ifstream myline(name.c_str());
        getline(myline, line1);
        while(getline(myline, line2)){
            idLine[ nameId[line1] ].push_back(it);
            lineId[ line1 ].push_back( nameId[line1] );
            graph[nameId[line1]][nameId[line2]] = 2;
            graph[nameId[line2]][nameId[line1]] = 2;
            line1 = line2;
        }
        myline.close();
    }
}

// Fills graph with zeroes
void fillGraph(){
    int dimensions = totalNumStations;
    vector<int> row(dimensions);
    for(int i = 0; i < dimensions; i++){
        graph.push_back(row);
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

void searchName(string toSearch){
    for(auto it: nameId){
        if(knuthMorrisPratt(it.first, toSearch) != -1) cout << it.first << endl;
    }
}

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

void printAllPaths(int src, int dest, vector<int> &visited, vector<int> &path, int path_index){
    visited[src] = 1;
    path[path_index] = src;
    path_index++;

    if(src == dest){
        for(int i = 0; i < path_index; i++){
            cout << path[i] << " ";
        }
        cout << endl << endl;
    }
    else{
        for(int j = 0; j < totalNumStations; j++){
            if(graph[src][j]){
                if(!visited[j]){
                    printAllPaths(j, dest, visited, path, path_index);
                }
            }
        }
    }
    path_index--;
    visited[src] = 0;
}

void actuallPrintAllPaths(int src, int dest){
    vector<int> visited(totalNumStations);
    vector<int> path(totalNumStations);
    int path_index = 0;
    printAllPaths(src, dest, visited, path, path_index);
}

int main(){
    totalNumStations = totalStations();
    fillGraph();
    addToGraph();
    //printLineId();
    //printIdLine();
    //printGraph();
    printVector(shortestTimePath(0, 93));
    pair<int, vector<string>> aaa = getLine(28);
    cout << aaa.first << ":";
    for(auto it: aaa.second)
        cout << it << " ";
    cout << endl;
    //actuallPrintAllPaths(0, 93);
    //cout << idName[0] << endl << idName[93] << endl;
    //printMap();
    //cout << endl << endl;
    //searchName("SECTOR 1");
    //src = sourceStation();
    //dest = destStation();
    //choice = shortestWhat();
    //givePath(src, dest, choice);
    return 0;
}
