#include <stack>
#include <set>
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

stack<int>* sccStack;
bool sccVisit[ALPHA_SIZE], sccInStack[ALPHA_SIZE];
int sccLow[ALPHA_SIZE], sccDfn[ALPHA_SIZE];
int dfsNum = 0, blockNum = 0;

//sccGraph 数据结构
vector<Graph*> sccGraph;
vector<int> sccId2Points[ALPHA_SIZE];
int point2sccID[ALPHA_SIZE];

Graph* looplessGraph;

void tarjan(Graph* graph, int x) {
    sccLow[x] = sccDfn[x] = ++dfsNum;
    sccStack -> push(x);
    sccVisit[x] = sccInStack[x] = true;
    for(Edge* e : *(graph -> getOutEdges(x))) {
        int to = e->getEnd();
        if (!sccVisit[to]) {
            tarjan(graph, to);
            sccLow[x] = min(sccLow[x], sccLow[to]);
        }
        else if (sccInStack[to]) {
            sccLow[x] = min(sccLow[x], sccDfn[to]);
        }
    }
    int topPoint;
    if (sccLow[x] == sccDfn[x]) {
        do {
            topPoint = sccStack -> top();
            sccStack -> pop();
            sccInStack[topPoint] = false;

            sccId2Points[blockNum].push_back(topPoint);
            point2sccID[topPoint] = blockNum;

        } while (sccLow[topPoint] != sccDfn[topPoint]);
        blockNum++;
    }
}

//scc inner dfs used
set<int> point2points[ALPHA_SIZE];
vector<Edge*> point2pointEdges[ALPHA_SIZE][ALPHA_SIZE];

void removeLoop(Graph *graph){
    memset(sccVisit, 0, (26));
    dfsNum = blockNum = 0;
    for (int i = 0; i < ALPHA_SIZE; i++) {
        if (!sccVisit[i]) {
            sccStack = new stack<int>;
            tarjan(graph, i);
        }
    }

    for (int i = 0; i < blockNum; i++) {
        sccGraph.push_back(new Graph(sccId2Points[i].size()));
    }
    looplessGraph = new Graph(blockNum);



    for(Edge* e : *(graph -> getEdges())) {
        int from = e->getStart();
        int to = e->getEnd();
        //边[端点属于一个连通分量]加进局部sccGraph
        //边[端点不在一个连通分量]加进大图looplessGraph,此时端点的id不是字母，代表sccId
        if (point2sccID[from] != point2sccID[to]) {
            looplessGraph->addEdge(new Edge(e->getWord(),point2sccID[from],point2sccID[to]));
        }
        else {
            sccGraph.at(point2sccID[from])->addEdge(e);
            point2pointEdges[from][to].push_back(e);

            point2points[from].insert(to);
        }
    }
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
    //cout << "-n" << endl;
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

extern Graph * rawGraph;

//程序连续执行多条指令前需要重置
void resizePoint2PointEdges(){
    for(Edge* e : *(rawGraph -> getEdges())) {
        int from = e->getStart();
        int to = e->getEnd();
        //边[端点属于一个连通分量]加进局部sccGraph
        //边[端点不在一个连通分量]加进大图looplessGraph,此时端点的id不是字母，代表sccId
        if (point2sccID[from] == point2sccID[to]) {
            point2pointEdges[from][to].push_back(e);
        }
    }
}