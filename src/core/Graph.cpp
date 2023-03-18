#include <stack>
#include <set>
#include <algorithm>
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
    for (auto & point2pointEdge : point2pointEdges) {
        point2points->clear();
        sccId2Points->clear();
        for (auto & j : point2pointEdge) {
            j.clear();
        }
    }
    sccGraph.clear();
    sccStack=new stack<int>;
    dfsNum = 0;blockNum = 0;

    memset(sccInStack, 0, sizeof(sccInStack));
    memset(sccDfn, 255, sizeof(sccDfn));
    memset(point2sccID,255,sizeof(point2sccID));
    memset(sccVisit, 0, sizeof(sccVisit));
    dfsNum = blockNum = 0;
    for (int i = 0; i < ALPHA_SIZE; i++) {
        if (!sccVisit[i]) {
            sccStack = new stack<int>;
            tarjan(graph, i);
        }
    }

    for (int i = 0; i < blockNum; i++) {
        sccGraph.push_back(new Graph((int)sccId2Points[i].size()));
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

int gen_chains_all(char* words[], int len,char * result[]){
    auto * g=new Graph(words,len);
    if(g->hasSelfLoop()) throw CoreException(-1);
    chainBuf = new vector<Edge*>();
    int allChainCount=0;
    auto* loopChain = new vector<string>();
    auto* topo = new vector<int>;
    int r = topoSort(g, topo);
    if(r) throw CoreException(-1);
    for(int i = 0; i < ALPHA_SIZE; i++){
        dfsAllChain(g,i,loopChain);
    }


    if(loopChain->empty()) throw CoreException(-2);

    ofstream outfile;
    outfile.open("solution.txt");

    outfile << to_string(loopChain->size()) << endl;
    cout << to_string(loopChain->size()) << endl;
    reverse(loopChain -> begin(), loopChain -> end());
    auto s = loopChain -> begin();
    while(s != loopChain -> end()) {
        cout << (*s)  ;
        outfile << (*s)  ;
        result[allChainCount++]=(char *)(*s).data();
        s++;
    }

    outfile.close();


    return allChainCount;
}

void dfsAllChain(Graph *g,int start, vector<string>* dfsChains){
    vector <Edge*>* edges = g -> getNOutEdges(start);
    for (auto e : *edges) {
        if(std::find(chainBuf->begin(), chainBuf->end(),e)!=chainBuf->end()) continue;
        chainBuf -> push_back(e);
        if(chainBuf -> size() > 1) printChain(chainBuf,dfsChains);

        dfsAllChain(g,e -> getEnd(),dfsChains);
        chainBuf -> pop_back();
    }
}

//TODO：单词链末尾是否允许空格
void printChain(vector <Edge*> *chain,vector<string> *dfsChains){
    stringstream k;
    auto e = chain -> begin();
    while(e != chain -> end()) {
        k << (*e) -> getWord() << ' ' ;
        e++;
    }
    k << endl;
    dfsChains->push_back(k.str());

}

const char* vuetifyAPI(const char* input, int type, char head, char tail, char reject,  bool weighted) {
    stringstream ss;
    string input_copy(input);
    vector<const char*> words;
    for (int i = 0, las = -1, size = (int) strlen(input); i < size; ++i) {
        char& c = const_cast<char &>(input_copy[i]);
        auto isalpha = [](char c) {
            return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z';
        };
        auto tolower = [](char c) {
            return c | 0x20;
        };
        if (isalpha(c)) {
            if (i != las) words.push_back(&c);
            las = i + 1;
            c = (char)tolower(c);
        }
        else {
            c = 0;
        }
    }


    if(words.empty()){
        ss << "WordList-GUI: " <<"File Input Error: at least two different words" << endl;
        return (new string(ss.str()))->data();
    }
    vector<char*> temp(32768, nullptr);

    int ret_val;
    try {
        if(type==0){
            ret_val=gen_chains_all(const_cast<char **>(words.data()), (int)words.size(), temp.data());
        } else if (weighted) {
            ret_val=gen_chain_word(const_cast<char **>(words.data()), (int)words.size(), temp.data(),
                                   head,tail,reject,type==3);
        }else {
            ret_val=gen_chain_char(const_cast<char **>(words.data()), (int)words.size(), temp.data(),
                                   head,tail,reject,type==3);
        }
    }
    catch (CoreException &ee){

        ss << "WordList-GUI: " <<ee.GetInfo() << endl;
        return (new string(ss.str()))->data();
    }

    if (type == 0) {
        ss << to_string(ret_val) << endl;
        for (int i = 0; i < ret_val; ++i) {
            ss << temp[i] ;
        }
    }else {
        for (int i = 0; i < ret_val; ++i) {
            ss << temp[i] << endl;
        }
    }

    if (ss.str().empty()) {
        return (new string("WordList-GUI: no solution exists"))->data();
    }

    return (new string(ss.str()))->data();
}
