//#include "definitions.h"
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

const string lines[] = {"blue1", "blue2", "yellow", "magenta"};
vector<vector<int>> graph;
unordered_map<int, string> idName;
unordered_map<string, int> nameId;
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
    //string lineColor;
    for(string it : lines){
        //if(isdigit(it.back())) lineColor = it.substr(0, it.size()-1);
        //else lineColor = it;

        string name = it + ".txt";
        ifstream myline(name.c_str());
        while(getline(myline, line)){
            if(nameId.find(line) == nameId.end()){
                idName[numberStations] = line;
                nameId[line] = numberStations;
                //idLine[numberStations].push_back(lineColor);
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
        string name = it + ".txt";
        ifstream myline(name.c_str());
        getline(myline, line1);
        while(getline(myline, line2)){
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
    for(auto it: idLine){
        cout << it.first;
        for(auto it2: it.second){
            cout << it2;
        }
        cout << endl;
    }
}

void printVector(vector<int> input){
    for(auto it: input){
        cout << idName[it] << " -> ";
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


int main(){
    totalNumStations = totalStations();
    fillGraph();
    addToGraph();
    printGraph();
    printVector(shortestTimePath(0, 93));
    printMap();
    cout << endl << endl;
    searchName("SECTOR 1");
    //src = sourceStation();
    //dest = destStation();
    //choice = shortestWhat();
    //givePath(src, dest, choice);
    return 0;
}
