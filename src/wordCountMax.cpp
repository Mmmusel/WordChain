#include <algorithm>
#include "node/Graph.h"

extern vector<int>* topo;
extern vector<Edge*>* chainBuf;
extern vector<string> word[ALPHA_SIZE][ALPHA_SIZE];

int wordCountMaxLoopless(Graph* graph) {
    int dp[ALPHA_SIZE];
    Edge* preEdge[ALPHA_SIZE];
    //dp[i]：以字母i为结尾时，能形成的单词链最大长度
    //int cnt = graph -> getPointNum();
    memset(preEdge, 0, sizeof(preEdge));
    chainBuf = new vector<Edge*>;

    topo = new vector<int>;
    int r = topoSort(graph, topo);
    CATCH(r);

    //单词链中对于每个字母可以存在一个首尾字符相同的单词
    for (int i = 0; i < ALPHA_SIZE; i++) {
        dp[i] = (graph->getSelfEdge(i)->empty() ? 0 : 1);
    }

    for (int i = 0; i < ALPHA_SIZE; i++) {
        int from = (*topo)[i];

        for(Edge* e : *(graph -> getOutEdges(from)))
        {
            int to = e -> getEnd();
            int newDp = dp[from] + 1 + (graph->getSelfEdge(i)->empty() ? 0 : 1);
            if (newDp > dp[to]) {
                dp[to] = newDp;
                preEdge[to] = e;
            }
        }
    }

    int maxEnd = 0;
    for (int i = 1; i < ALPHA_SIZE; i++) {
        if (dp[i] > dp[maxEnd]) {
            maxEnd = i;
        }
    }

    if (dp[maxEnd] <= 1) {
        return 0;
    }

    vector<string>* wordCountMaxChain = new vector<string>();

    int nowEnd = maxEnd;
    while (preEdge[nowEnd]) {
        int nowStart = preEdge[nowEnd]->getStart();
        if (!graph->getSelfEdge(nowEnd)->empty()) {
            wordCountMaxChain->push_back(word[nowEnd][nowEnd].front());
        }
        wordCountMaxChain->push_back(preEdge[nowEnd]->getWord());
        nowEnd = nowStart;
    }
    //起点自环
    if (!graph->getSelfEdge(nowEnd)->empty()) {
        wordCountMaxChain->push_back(word[nowEnd][nowEnd].front());
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