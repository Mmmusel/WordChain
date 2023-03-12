#include <algorithm>
#include <set>
#include "node/Graph.h"

extern vector<int>* topo;
extern vector<Edge*>* chainBuf;
extern vector<string> word[ALPHA_SIZE][ALPHA_SIZE];

extern Graph * rawGraph;

bool cmp1(Edge* n1,Edge* n2)
{
    return n1->getWord().length() > n2->getWord().length();
}

void sortByChar(){
    for (int i=0;i<ALPHA_SIZE;i++) {
        vector <Edge*>* k = rawGraph->getSelfEdge(i);
        sort(k->begin(),k->end(),cmp1);
    }
}

int charCountMaxLoopless(Graph* graph) {
    sortByChar();
    int dpChar[ALPHA_SIZE];

    //char不能像word一样由dp结果判断是否为单词链，再加一个链长dp
    int dpLineLen[ALPHA_SIZE];
    Edge* preEdge[ALPHA_SIZE];
    //dp[i]：以字母i为结尾时，能形成的单词链最大长度
    //int cnt = graph -> getPointNum();
    memset(preEdge, 0, sizeof(preEdge));
    chainBuf = new vector<Edge*>;

    topo = new vector<int>;
    int r = topoSort(graph, topo);
    CATCH(r);

    //单词链中对于每个字母可以存在一个首尾字符相同的单词,取排序后最大的
    for (int i = 0; i < ALPHA_SIZE; i++) {
        vector <Edge*>* k = rawGraph->getSelfEdge(i);
        dpChar[i] = (k->empty() ? 0 : k->back()->getWord().length());
        dpLineLen[i] = (k->empty() ? 0 : 1);
    }

    for (int i = 0; i < ALPHA_SIZE; i++) {
        int from = (*topo)[i];

        for(Edge* e : *(graph -> getOutEdges(from))) {
            int to = e -> getEnd();

            int endSelfWeight = (graph->getSelfEdge(i)->empty() ? 0 : graph->getSelfEdge(i)->back()->getWord().length());
            int newDp = dpChar[from] + e->getWeight() + endSelfWeight;

            if (newDp > dpChar[to]) {
                dpChar[to] = newDp;
                dpLineLen[to] = dpLineLen[from] + 1 + (endSelfWeight ? 1 : 0);
                preEdge[to] = e;
            }
        }
    }

    int maxEnd = 0;
    for (int i = 1; i < ALPHA_SIZE; i++) {
        if (dpChar[i] > dpChar[maxEnd] && dpLineLen[i]>1) {
            maxEnd = i;
        }
    }

    if (dpChar[maxEnd] <= 1) {
        return 0;
    }

    vector<string>* wordCountMaxChain = new vector<string>();

    int nowEnd = maxEnd;
    while (preEdge[nowEnd]) {
        int nowStart = preEdge[nowEnd]->getStart();
        if (!graph->getSelfEdge(nowEnd)->empty()) {
            wordCountMaxChain->push_back(rawGraph->getSelfEdge(nowEnd)->back()->getWord());
        }
        wordCountMaxChain->push_back(preEdge[nowEnd]->getWord());
        nowEnd = nowStart;
    }
    //起点自环
    if (!graph->getSelfEdge(nowEnd)->empty()) {
        wordCountMaxChain->push_back(rawGraph->getSelfEdge(nowEnd)->back()->getWord());
    }

    cout << "-w loopless" << endl;
    cout << wordCountMaxChain->size() << endl ;
    reverse(wordCountMaxChain -> begin(), wordCountMaxChain -> end());
    auto s = wordCountMaxChain -> begin();
    while(s != wordCountMaxChain -> end()) {
        cout << (*s) << ' ' ;
        s++;
    }

    return wordCountMaxChain->size();
}