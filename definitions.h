#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
using namespace std;

// Current number of stations is 15
#define STATIONS 15

class Network{
    public:
    // map stores id, name, line
    unordered_map<int, pair<string, string>> id_name;
    // graph stores time from i to j
    int graph[STATIONS][STATIONS];
    Network();
};

int sourceStation();
int destStation();
int shortestWhat();
void givePath(int, int, int);