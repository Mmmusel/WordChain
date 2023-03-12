#include <algorithm>
#include <set>
#include "node/Graph.h"

extern vector<int>* topo;
extern vector<Edge*>* chainBuf;
extern vector<string> word[ALPHA_SIZE][ALPHA_SIZE];

extern vector<Graph*> sccGraph;
extern vector<int> sccId2Points[ALPHA_SIZE];

int sccInnerDp[ALPHA_SIZE][ALPHA_SIZE];

//dfs状态矩阵
int toPointLoopWeightAdded [ALPHA_SIZE];

//scc内部的
extern set<int> point2points[ALPHA_SIZE];
extern vector<Edge*> point2pointEdges[ALPHA_SIZE][ALPHA_SIZE];

void sccInnerGraphDp() {
    memset(sccInnerDp, 255, sizeof(sccInnerDp));
    for (int i = 0; i < sccGraph.size(); i++)
        sccInnerDp[i][i] = 0;

    //对每个强连通分量内部
    //每个起点开始dfs计算两点之间最大链长
    for (int sccId=0;sccId < sccGraph.size();sccId++) {
        for(int x : sccId2Points[sccId]){
            memset(toPointLoopWeightAdded,0,ALPHA_SIZE);
            sccInnerDfs(x, x, 0);
        }
    }
}

extern Graph * rawGraph;

void sccInnerDfs(int start, int now, int length) {
    toPointLoopWeightAdded[now]++;

    //modify: -w不用for边，for邻接点随便选一个边就可以
    for (int to : point2points[now]){
        if(point2pointEdges[now][to].empty()) continue;
        Edge * e = point2pointEdges[now][to].back();
        point2pointEdges[now][to].pop_back();

        int toSelfWeight = (toPointLoopWeightAdded[to] ? 0:(rawGraph->getSelfEdge(to)->size()));
        int newDp=length+1+toSelfWeight;

        sccInnerDp[start][to]=max(sccInnerDp[start][to], newDp);

        sccInnerDfs(start,to,newDp);
        point2pointEdges[now][to].push_back(e);
    }
    toPointLoopWeightAdded[now]--;
}

int sccOuterDp[ALPHA_SIZE];
Edge * preEdge[ALPHA_SIZE];
int preSCCPoint[ALPHA_SIZE];

extern Graph* looplessGraph;
extern int point2sccID[ALPHA_SIZE];

int LoopGraphMaxWordDP(int head, int tail) {
    if (head < 0) {
        for (int i = 0; i < ALPHA_SIZE; i++) {
            sccOuterDp[i] = (rawGraph->getSelfEdge(i)->empty() ? 0 : rawGraph->getSelfEdge(i)->size());
        }
    } else {
        memset(sccOuterDp,255,(ALPHA_SIZE << 2));
        sccOuterDp[head] = (rawGraph->getSelfEdge(head)->empty() ? 0 : rawGraph->getSelfEdge(head)->size());
    }



    memset(preEdge, 0, ALPHA_SIZE << 2);
    memset(preSCCPoint, 255, ALPHA_SIZE << 2);

    vector<int> *sccTopo = new vector<int>;
    topoSort(looplessGraph, sccTopo);

    for (int i = 0; i < looplessGraph->getPointNum(); i++) {
        int gp[ALPHA_SIZE], gpPoint[ALPHA_SIZE];
        memset(gp, 255, ALPHA_SIZE << 2);
        memset(gpPoint, 255, ALPHA_SIZE << 2);

        int fromSccId = sccTopo->at(i);
        //找同属于这个scc的其他点
        for (int otherSccId: sccId2Points[fromSccId]) {
            if (sccOuterDp[otherSccId]<0) continue;
            for (int anotherSccId: sccId2Points[fromSccId]) {
                int newDp = sccOuterDp[otherSccId] + sccInnerDp[otherSccId][anotherSccId];
                if (newDp > gp[anotherSccId]) {
                    gp[anotherSccId] = newDp;
                    gpPoint[anotherSccId] = otherSccId;
                }
            }
        }
        for (int j = 0; j < ALPHA_SIZE; j++) {
            if (sccOuterDp[j] < gp[j]) {
                sccOuterDp[j] = gp[j];
                preSCCPoint[j] = gpPoint[j];
            }
        }

        //scc外dp
        for (Edge *e: *(looplessGraph->getOutEdges(fromSccId))) {
            string tmp = e->getWord();
            int start = tmp.front() - 'a';
            int end = tmp.back() - 'a';
            int to_weight = rawGraph->getSelfEdge(end)->size();
            if (sccOuterDp[end] < sccOuterDp[start] + 1 + to_weight) {
                sccOuterDp[end] = sccOuterDp[start] + 1 + to_weight;
                preEdge[end] = e;
            }
        }
    }

    int returnEndPoint = 0;
    if(tail >= 0) {
        returnEndPoint = tail;
    } else {
        for (int i = 1; i < ALPHA_SIZE; i++) {
            if (sccOuterDp[i] > sccOuterDp[returnEndPoint]) {
                returnEndPoint = i;
            }
        }
    }
    return returnEndPoint;
}

string wordList[20202];
int wordCnt = 0;

bool dfsSccWordMaxChain(int now, int end, int length, vector<string>* sccInnerChain) {
    if (length == 0) {
        if (end == now) {
            for (int i = 1; i <= wordCnt; i++) {
                sccInnerChain->push_back(wordList[i]);
            }
            return 1;
        }
        else { return 0; }
    }

    toPointLoopWeightAdded[now]++;

    //modify: -w不用for边，for邻接点随便选一个边就可以
    for (int to : point2points[now]){
        if(point2pointEdges[now][to].empty()) continue;
        Edge * e = point2pointEdges[now][to].back();
        point2pointEdges[now][to].pop_back();

        int lastCnt = wordCnt;
        wordList[++wordCnt] = e->getWord();

        int toPointSelfLoopWeight = (toPointLoopWeightAdded[to]? 0:rawGraph->getSelfEdge(to)->size());
        if (toPointSelfLoopWeight > 0) {
            for(Edge * loopE : *(rawGraph->getSelfEdge(to))) {
                wordList[++wordCnt] = loopE->getWord();
            }
        }

        if (dfsSccWordMaxChain(to, end, length - 1 - toPointSelfLoopWeight, sccInnerChain)) {
            return 1;
        }
        wordCnt = lastCnt;
        point2pointEdges[now][to].push_back(e);
    }
    toPointLoopWeightAdded[now]--;
    return 0;
}

int printWordMaxChain(int now) {
    vector<string>* loopChain = new vector<string>();

    bool inSCC = false;
    while (preEdge[now] || !inSCC && preSCCPoint[now] >= 0) {
        if (!inSCC && preSCCPoint[now] >= 0) {
            inSCC = true;
            int from = preSCCPoint[now];
            vector<string>* subChain = new vector<string>();

            memset(toPointLoopWeightAdded,0,ALPHA_SIZE);
            dfsSccWordMaxChain(from, now, sccInnerDp[from][now], subChain);

            //memset一个就行了，上次dfs之后边的数组应该被回溯完整了
            reverse(subChain -> begin(), subChain -> end());
            auto s = subChain -> begin();
            while(s != subChain -> end()) {
                loopChain->push_back(*s);
                s++;
            }
            now = from;
        }
        else {
            //跨scc的边，把终点所有自环加进去，再加上前向边
            inSCC = false;
            Edge* e = preEdge[now];
            int toPointSelfLoopWeight = rawGraph->getSelfEdge(now)->size();
            if (toPointSelfLoopWeight > 0) {
                for(Edge * loopE : *(rawGraph->getSelfEdge(now))) {
                    loopChain->push_back(loopE->getWord());
                }
            }
            loopChain->push_back(e->getWord());
            now = e->getWord().front()-'a';
        }
    }
    //TODO 封装
    //最起点的所有自环边
    int toPointSelfLoopWeight = rawGraph->getSelfEdge(now)->size();
    if (toPointSelfLoopWeight > 0) {
        for(Edge * loopE : *(rawGraph->getSelfEdge(now))) {
            loopChain->push_back(loopE->getWord());
        }
    }

    cout << "-w loop" << endl;
    cout << loopChain->size() << endl;
    reverse(loopChain -> begin(), loopChain -> end());
    auto s = loopChain -> begin();
    while(s != loopChain -> end()) {
        cout << (*s) << ' ' ;
        s++;
    }
    return loopChain->size();
}

int wordCountMaxLoop(Graph* graph, int head, int tail) {
    topo = new vector<int>;
    int r = topoSort(graph, topo);
    if(r==0){
        return wordCountMaxLoopless(graph,head,tail);
    }
    removeLoop(graph);
    sccInnerGraphDp();
    int now = LoopGraphMaxWordDP(head,tail);
    return printWordMaxChain(now);
}

int wordCountMaxLoopless(Graph* graph, int head, int tail) {
    int dp[ALPHA_SIZE];
    Edge* preEdge[ALPHA_SIZE];
    //dp[i]：以字母i为结尾时，能形成的单词链最大长度
    //int cnt = graph -> getPointNum();
    memset(preEdge, 0, sizeof(preEdge));
    chainBuf = new vector<Edge*>;

    topo = new vector<int>;
    int r = topoSort(graph, topo);
    CATCH(r);

    if (head < 0) {
        //单词链中对于每个字母可以存在一个首尾字符相同的单词
        for (int i = 0; i < ALPHA_SIZE; i++) {
            dp[i] = (graph->getSelfEdge(i)->empty() ? 0 : 1);
        }
    } else {
        memset(dp,255,(ALPHA_SIZE << 2));
        dp[head] = (graph->getSelfEdge(head)->empty() ? 0 : 1);
    }


    for (int i = 0; i < ALPHA_SIZE; i++) {
        int from = (*topo)[i];

        if (dp[from] < 0) continue;

        for(Edge* e : *(graph -> getOutEdges(from))) {
            int to = e -> getEnd();
            int newDp = dp[from] + 1 + (graph->getSelfEdge(i)->empty() ? 0 : 1);
            if (newDp > dp[to]) {
                dp[to] = newDp;
                preEdge[to] = e;
            }
        }
    }

    int maxEnd = 0;
    if(tail >= 0) {
        maxEnd = tail;
    } else {
        for (int i = 1; i < ALPHA_SIZE; i++) {
            if (dp[i] > dp[maxEnd]) {
                maxEnd = i;
            }
        }
    }


    if (dp[maxEnd] <= 1) {
        cout << "no chain" << endl;
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