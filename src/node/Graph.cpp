#include <sstream>
#include "Graph.h"

vector<Edge*>* chainBuf;
int allChainCount=0;
ostringstream allChainBuf;

void getAllChain(Graph *g){
    chainBuf = new vector<Edge*>();

    for(int i = 0; i < ALPHA_SIZE; i++){
        dfsAllChain(g,i);
    }
    cout << allChainCount << endl;
    cout << allChainBuf.str() << endl;
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