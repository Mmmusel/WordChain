#ifndef WORDCHAIN_GRAPH_H
#define WORDCHAIN_GRAPH_H

#include "../main.h"
#include "Edge.h"

#include <sstream>
#include <queue>


class Graph {
private:
    int pointNum;
    int inDegree[26]{};
    vector <Edge*> edges[26][26];
    vector <Edge*> selfEdge[26];
    vector <Edge*> edgesIn[26];
    vector <Edge*> edgesOut[26];

public:
    explicit Graph(int _point_num) {
        pointNum = _point_num;
        memset(inDegree, 0, (26 << 2));
    }

    void addEdge(Edge* _edge) {
        int s = _edge -> getStart();
        int e = _edge -> getEnd();
        edges[s][e].push_back(_edge);

        if (s == e){
            selfEdge[s].push_back(_edge);
        }
        else {
            edgesOut[s].push_back(_edge);
            edgesIn[e].push_back(_edge);
            inDegree[e]++;
        }
    }

    int * getInDegree() {
        return inDegree;
    }

    vector <Edge*>* getOutEdges(int s) {
        return &(edgesOut[s]);
    }

    vector <Edge*>* getInEdges(int s) {
        return &(edgesIn[s]);
    }

    int getPointNum() const {
        return pointNum;
    }

    vector <Edge*>* getSelfEdge(int s) {
        return &(selfEdge[s]);
    }
};

int getAllChain(Graph *g);
void dfsAllChain(Graph *g,int start);
void printChain(vector <Edge*> *chain);
int wordCountMaxLoopless(Graph * graph);
int topoSort(Graph* graph, vector<int>* result);

#endif //WORDCHAIN_GRAPH_H
