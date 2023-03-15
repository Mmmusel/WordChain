#include <stack>
#include <set>
#include "Graph.h"

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

int gen_chains_all(char* words[], int len,char * result[]){
    Graph * g=new Graph(words,len);
    chainBuf = new vector<Edge*>();

    allChainBuf.clear();
    allChainCount=0;
    topo = new vector<int>;
    int r = topoSort(g, topo);
    CATCH(r);
    for(int i = 0; i < ALPHA_SIZE; i++){
        dfsAllChain(g,i);
    }
    //cout << "-n" << endl;
    string kkk=allChainBuf.str();
    //
    cout << allChainCount << endl;
    cout << allChainBuf.str() << endl;

    ofstream outfile;
    outfile.open("result.txt");

    outfile << allChainCount << endl ;
    outfile << allChainBuf.str() << endl;
    outfile.close();

    result[0]=(char*)kkk.data();

    return allChainCount;
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


//程序连续执行多条指令前需要重置
void resizePoint2PointEdges(Graph * rawGraph){
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

const char* vuetifyAPI(const char* input, int type, char head, char tail, char reject,  bool weighted) {
    string input_copy(input);
    vector<const char*> words;
    for (int i = 0, las = -1, size = (int) strlen(input); i < size; ++i) {
        char& c = const_cast<char &>(input_copy.data()[i]);
        auto isalpha = [](char c) {
            return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z';
        };
        auto tolower = [](char c) {
            return c | 0x20;
        };
        if (isalpha(c)) {
            if (i != las) words.push_back(&c);
            las = i + 1;
            c = tolower(c);
        }
        else {
            c = 0;
        }
    }
    vector<char*> temp(32768, nullptr);

    int ret_val = 0;


    if(type==0){
        ret_val=gen_chains_all(const_cast<char **>(words.data()), words.size(), temp.data());
    } else if (weighted) {
        ret_val=gen_chain_word(const_cast<char **>(words.data()), words.size(), temp.data(),
                               head,tail,reject,type==3);
    }else {
        ret_val=gen_chain_char(const_cast<char **>(words.data()), words.size(), temp.data(),
                               head,tail,reject,type==3);
    }

    stringstream ss;

    if (type == 0) {
        ss << ret_val << endl;
        ss << temp[0] << endl;
    } else {
        for (int i = 0; i < ret_val; ++i) {
            ss << temp[i] << endl;
        }
    }

    if (ss.str().size() == 0) {
        return (new string("WordList-GUI: no solution exists"))->data();
    }

    return (new string(ss.str()))->data();
}
