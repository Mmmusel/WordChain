#include "Graph.h"
#include "../error.h"

int topoSort(Graph* graph, vector<int>* result) {
    int inDegree[ALPHA_SIZE];
    memcpy(inDegree, graph->getInDegree(), (ALPHA_SIZE << 2));

    queue<int> q;

    int cnt = graph -> getPointNum();
    for (int i = 0; i < cnt; ++i) {
        if (!inDegree[i]) {
            q.push(i);
            result->push_back(i);
        }
    }

    while(!q.empty()) {
        int from = q.front();
        q.pop();
        for(Edge* e : *(graph -> getOutEdges(from)))
        {
            int to = e->getEnd();
            if (!--inDegree[to]) {
                q.push(to);
                result -> push_back(to);
            }
        }
    }

    /*
    auto e = result -> begin();
    while(e != result -> end()) {
        cout << (*e) << ' ' ;
        e++;
    }
     */

    if (result->size() != cnt) {
        return -LOOP;
    }

    return 0;
}

vector<Edge*>* chainBuf;
vector<int>* topo;
int allChainCount=0;
ostringstream allChainBuf;

int getAllChain(Graph *g){
    chainBuf = new vector<Edge*>();
    topo = new vector<int>;
    int r = topoSort(g, topo);
    CATCH(r);
    for(int i = 0; i < ALPHA_SIZE; i++){
        dfsAllChain(g,i);
    }
    cout << "-n" << endl;
    cout << allChainCount << endl;
    cout << allChainBuf.str() << endl;
    return 0;
}

void dfsAllChain(Graph *g,int start){
    vector <Edge*>* edges = g -> getOutEdges(start);
    for (auto e : *edges) {
        chainBuf -> push_back(e);
        if(chainBuf -> size() > 1) printChain(chainBuf);

        dfsAllChain(g,e -> getEnd());
        chainBuf -> pop_back();
    }
}

//TODO：单词链末尾是否允许空格
void printChain(vector <Edge*> *chain){
    auto e = chain -> begin();
    while(e != chain -> end()) {
        allChainBuf << (*e) -> getWord() << ' ' ;
        e++;
    }
    allChainBuf << endl;
    allChainCount += 1;
}