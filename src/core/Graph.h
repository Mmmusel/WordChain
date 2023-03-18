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
    vector <Edge*> edgesNOut[ALPHA_SIZE];
    vector <int> edgesOutPoints[ALPHA_SIZE];
    vector <Edge*> edges;


    int selfLoopSum[ALPHA_SIZE]{};
    vector <Edge*> selfEdge[ALPHA_SIZE];

    bool beforeTopoHasSelfLoop = false;


public:
    explicit Graph(int _point_num) {
        pointNum = _point_num;
        memset(inDegree, 0, (26 << 2));
    }

    explicit Graph(char* words[], int len, int reject) {
        pointNum=26;
        int in[ALPHA_SIZE];
        int out[ALPHA_SIZE];
        memset(in, 0, 26<<2);
        memset(out, 0, 26<<2);

        set<string> removeRepeat;

        for (int i=0;i<len;i++) {
            if (words[i] == nullptr || strlen(words[i]) < 1 || ((words[i][0]-'a')==reject)) continue;
            string tmp = words[i];
            if(removeRepeat.find(tmp)!=removeRepeat.end()) continue;
            else removeRepeat.insert(tmp);
            Edge * _edge = new Edge(tmp);
            int s = _edge -> getStart();
            int e = _edge -> getEnd();
            edgesMartrix[s][e].push_back(_edge);
            in[e]++;
            out[s]++;
        }
        if(removeRepeat.size()<2) throw CoreException(-2);

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

        bool afterRemoveHasEdge = false;
        for(int i=0;i<ALPHA_SIZE;i++){
            for(int j=0;j<ALPHA_SIZE;j++){
                if (edgesMartrix[i][j].empty()) continue;
                afterRemoveHasEdge=true;
                if (i==j) {
                    for (Edge * e: edgesMartrix[i][i]){
                        selfLoopSum[i]+=e->getWeight();
                    }
                    if(edgesMartrix[i][i].size()>1) beforeTopoHasSelfLoop=true;
                    continue;
                }
                edgesOutPoints[i].push_back(j);
            }
        }
        if(!afterRemoveHasEdge) {
            throw CoreException(-2);
        }
    }

    explicit Graph(char* words[], int len) {
        pointNum=26;
        int in[ALPHA_SIZE];
        int out[ALPHA_SIZE];
        memset(in, 0, 26<<2);
        memset(out, 0, 26<<2);

        set<string> removeRepeat;
        for (int i=0;i<len;i++) {
            string tmp = words[i];
            if (words[i] == nullptr || strlen(words[i]) < 1 ) continue;
            if(removeRepeat.find(tmp)!=removeRepeat.end()) continue;
            else removeRepeat.insert(tmp);
            Edge * _edge = new Edge(tmp);
            int s = _edge -> getStart();
            int e = _edge -> getEnd();
            edgesMartrix[s][e].push_back(_edge);
            in[e]++;
            out[s]++;
        }
        if(removeRepeat.size()<2) throw CoreException(-2);

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

        bool afterRemoveHasEdge=false;
        for(int i=0;i<ALPHA_SIZE;i++){
            for(int j=0;j<ALPHA_SIZE;j++){
                if (edgesMartrix[i][j].empty()) continue;
                afterRemoveHasEdge=true;
                if (i==j) {
                    for (Edge * e: edgesMartrix[i][i]){
                        selfLoopSum[i]+=e->getWeight();
                    }
                    if(edgesMartrix[i][i].size()>1) beforeTopoHasSelfLoop=true;
                    continue;
                }
                edgesOutPoints[i].push_back(j);
            }
        }
        if(!afterRemoveHasEdge) {
            throw CoreException(-2);
        }
    }

    bool hasSelfLoop() {
        return beforeTopoHasSelfLoop;
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
        edgesNOut[s].push_back(_edge);

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

    vector <Edge*>* getNOutEdges(int s) {
        return &(edgesNOut[s]);
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
void dfsAllChain(Graph *g,int start,vector<string>* dfsChain);
void printChain(vector <Edge*> *chain ,vector<string>* dfsChain);
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

const char* vuetifyAPI(const char* input, int type, char head, char tail, char reject, bool weighted) ;
#endif //WORDCHAIN_GRAPH_H
