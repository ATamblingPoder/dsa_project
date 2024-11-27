#include "definitions.h"

Network::Network(){
    id_name[0] = {};
    id_name[1] = {};
    id_name[2] = {};
    id_name[3] = {};
    id_name[4] = {};
    id_name[5] = {};
    id_name[6] = {};
    id_name[7] = {};
    id_name[8] = {};
    id_name[9] = {};
    id_name[10] = {};
    id_name[11] = {};
    id_name[12] = {};
    id_name[13] = {};
    id_name[14] = {};
    for(int i = 0; i < STATIONS; i++){
        for(int j = 0; j < STATIONS; j++){
            graph[i][j] = 0;
        }
    }
    // Add data to graph
}