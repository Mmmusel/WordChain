#ifndef WORDCHAIN_GRAPH_H
#define WORDCHAIN_GRAPH_H

#include "../main.h"
#include "Edge.h"

#include <sstream>
#include <queue>
#include <set>
class Graph {
private:
    int pointNum;
    vector <Edge*> edgesMartrix[ALPHA_SIZE][ALPHA_SIZE];

    int inDegree[ALPHA_SIZE]{};

    vector <Edge*> edgesOut[ALPHA_SIZE];
    vector <int> edgesOutPoints[ALPHA_SIZE];
    vector <Edge*> edges;

    int selfLoopSum[ALPHA_SIZE]{};
    vector <Edge*> selfEdge[ALPHA_SIZE];


public:
    explicit Graph(int _point_num) {
        pointNum = _point_num;
        memset(inDegree, 0, (26 << 2));
    }

    //Graph(char * inputWord[], int wordCnt) {
    explicit Graph(char* words[], int len, int reject) {
        pointNum=26;
        int in[ALPHA_SIZE];
        int out[ALPHA_SIZE];
        memset(in, 0, 26<<2);
        memset(out, 0, 26<<2);
        //for(int i=0;i<wordCnt;i++) {
        for (int i=0;i<len;i++) {
            if (words[i] == NULL || ((words[i][0]-'a')==reject)) continue;
            string tmp = words[i];
            Edge * _edge = new Edge(tmp);
            int s = _edge -> getStart();
            int e = _edge -> getEnd();
            edgesMartrix[s][e].push_back(_edge);
            in[e]++;
            out[s]++;
        }

        for (int i=0;i<ALPHA_SIZE;i++) {
            if (in[i]==1 && out[i]==1 && (!edgesMartrix[i][i].empty())) {
                edgesMartrix[i][i].pop_back();
            }
        }

        for (int i=0;i<ALPHA_SIZE;i++) {
            for (int j=0;j<ALPHA_SIZE;j++) {
                if (edgesMartrix[i][j].empty()) continue;

                if (in[i]==0 && out[j]==0) {
                    edgesMartrix[i][j].clear();
                } else {
                    for (Edge* e : edgesMartrix[i][j]) {
                        fromMartrixAddEdge(e);
                    }
                }
            }
        }

        //calOutPoints()
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

    explicit Graph(char* words[], int len) {
        pointNum=26;
        int in[ALPHA_SIZE];
        int out[ALPHA_SIZE];
        memset(in, 0, 26<<2);
        memset(out, 0, 26<<2);
        //for(int i=0;i<wordCnt;i++) {
        for (int i=0;i<len;i++) {
            string tmp = words[i];
            Edge * _edge = new Edge(tmp);
            int s = _edge -> getStart();
            int e = _edge -> getEnd();
            edgesMartrix[s][e].push_back(_edge);
            in[e]++;
            out[s]++;
        }

        for (int i=0;i<ALPHA_SIZE;i++) {
            if (in[i]==1 && out[i]==1 && (!edgesMartrix[i][i].empty())) {
                edgesMartrix[i][i].pop_back();
            }
        }

        for (int i=0;i<ALPHA_SIZE;i++) {
            for (int j=0;j<ALPHA_SIZE;j++) {
                if (edgesMartrix[i][j].empty()) continue;

                if (in[i]==0 && out[j]==0) {
                    edgesMartrix[i][j].clear();
                } else {
                    for (Edge* e : edgesMartrix[i][j]) {
                        fromMartrixAddEdge(e);
                    }
                }
            }
        }

        //calOutPoints()
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
            inDegree[e]++;
            edges.push_back(_edge);
        }
    }

    void fromMartrixAddEdge(Edge* _edge) {
        int s = _edge -> getStart();
        int e = _edge -> getEnd();
        //edgesMartrix[s][e].push_back(_edge);

        if (s == e){
            selfEdge[s].push_back(_edge);
        }
        else {
            edgesOut[s].push_back(_edge);
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

//int getAllChain(char* words[], int len);
void dfsAllChain(Graph *g,int start);
void printChain(vector <Edge*> *chain);
int wordCountMaxLoopless(Graph * rawGraph, vector<int> *topo,  int head, int tail, char * result[]);
int topoSort(Graph* graph, vector<int>* result);
void sccInnerDfs(Graph* graph,int start, int now, int length);
void removeLoop(Graph *graph);
int wordCountMaxLoop(Graph * rawGraph,  int head, int tail, char * result[]);
int charCountMaxLoopless(Graph * rawGraph, vector<int> *topo, int head, int tail, char * result[]);
int charCountMaxLoop(Graph * rawGraph,  int head, int tail, char * result[]);

int gen_chains_all(char* words[], int len, char* result[]);
int gen_chain_char(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);
int gen_chain_word(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);
void sccInnerDfsChar(Graph * rawGraph,int start, int now, int length);

const char* gui_engine(const char* input, int type, char head, char tail, bool weighted) ;
#endif //WORDCHAIN_GRAPH_H
