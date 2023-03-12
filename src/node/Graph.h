#ifndef WORDCHAIN_GRAPH_H
#define WORDCHAIN_GRAPH_H

#include "../main.h"
#include "Edge.h"

#include <sstream>
#include <queue>

class Graph {
private:
    int pointNum;
    int inDegree[ALPHA_SIZE]{};
    vector <Edge*> edgesMartrix[ALPHA_SIZE][ALPHA_SIZE];
    vector <Edge*> selfEdge[ALPHA_SIZE];
    vector <Edge*> edgesIn[ALPHA_SIZE];
    vector <Edge*> edgesOut[ALPHA_SIZE];
    vector <Edge*> edges;

    vector <int> edgesOutPoints[ALPHA_SIZE];
    int selfLoopSum[ALPHA_SIZE]{};

public:
    explicit Graph(int _point_num) {
        pointNum = _point_num;
        memset(inDegree, 0, (26 << 2));
    }

    void calOutPoints(){
        for(int i=0;i<ALPHA_SIZE;i++){
            for(int j=0;j<ALPHA_SIZE;j++){
                if (edgesMartrix[i][j].empty()) continue;
                if (i==j) {
                    for (Edge * e: edgesMartrix[i][i]){
                        selfLoopSum[i]+=e->getWeight();
                    }
                    continue;
                }
                edgesOutPoints[i].push_back(j);
            }
        }


    }

    void addEdge(Edge* _edge) {
        int s = _edge -> getStart();
        int e = _edge -> getEnd();
        edgesMartrix[s][e].push_back(_edge);

        if (s == e){
            selfEdge[s].push_back(_edge);
        }
        else {
            edgesOut[s].push_back(_edge);
            edgesIn[e].push_back(_edge);
            inDegree[e]++;
            edges.push_back(_edge);
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

    int getselfLoopSum(int k) const {
        return selfLoopSum[k];
    }

    vector <Edge*>* getSelfEdge(int s) {
        return &(selfEdge[s]);
    }

    vector <Edge*>* getEdges() {
        return &edges;
    }
};

int getAllChain(Graph *g);
void dfsAllChain(Graph *g,int start);
void printChain(vector <Edge*> *chain);
int wordCountMaxLoopless(Graph* graph, int head, int tail);
int topoSort(Graph* graph, vector<int>* result);
void sccInnerDfs(int start, int now, int length);
void removeLoop(Graph *graph);
int wordCountMaxLoop(Graph* graph, int head, int tail);
int charCountMaxLoopless(Graph* graph, int head, int tail);
int charCountMaxLoop(Graph* graph, int head, int tail);

void sccInnerDfsChar(int start, int now, int length);
void resizePoint2PointEdges();
#endif //WORDCHAIN_GRAPH_H
