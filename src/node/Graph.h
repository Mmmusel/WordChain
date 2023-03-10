#ifndef WORDCHAIN_GRAPH_H
#define WORDCHAIN_GRAPH_H

#include "../main.h"
#include "Edge.h"

class Graph {
private:
    int pointNum;
    int inDegree[26];
    vector <Edge*> edges[26][26];
    vector <Edge*> selfEdge[26];
    vector <Edge*> edgesIn[26];
    vector <Edge*> edgesOut[26];

public:
    Graph(int _point_num){
        pointNum = _point_num;
        memset(inDegree, 0, (26 << 2));
    }

    void addEdge(Edge* _edge){
        int s = _edge -> getStart();
        int e = _edge -> getEnd();
        edges[s][e].push_back(_edge);
        inDegree[e]++;

        if (s == e){
            selfEdge[s].push_back(_edge);
        }
        else {
            edgesOut[s].push_back(_edge);
            edgesIn[e].push_back(_edge);
        }
    }

    int * getInDegree(){
        return inDegree;
    }

    vector <Edge*>* getOutEdges(int s){
        return &(edgesOut[s]);
    }
};

void getAllChain(Graph *g);
void dfsAllChain(Graph *g,int start);
void printChain(vector <Edge*> *chain);

#endif //WORDCHAIN_GRAPH_H
