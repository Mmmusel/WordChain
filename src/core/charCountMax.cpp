#include <algorithm>
#include <set>
#include "Graph.h"

extern vector<Edge*>* chainBuf;

bool cmp1(Edge* n1,Edge* n2)
{
    return n1->getWord().length() > n2->getWord().length();
}

//graph.cpp
extern vector<Graph*> sccGraph;
extern vector<int> sccId2Points[ALPHA_SIZE];

extern set<int> point2points[ALPHA_SIZE];

extern vector<Edge*> point2pointEdges[ALPHA_SIZE][ALPHA_SIZE];
void sortByChar(Graph * rawGraph){
    for (int i=0;i<ALPHA_SIZE;i++) {
        vector <Edge*>* k = rawGraph->getSelfEdge(i);
        sort(k->begin(),k->end(),cmp1);
    }
}

void sortByCharLoop(Graph * rawGraph) {
    sortByChar(rawGraph);

    //resizePoint2PointEdges();
    for (auto & point2pointEdge : point2pointEdges) {
        for (auto & j : point2pointEdge) {
            sort(j.begin(), j.begin() + (int)j.size(), cmp1);
        }
    }
}

//wordCountMax.cpp
extern int sccInnerDp[ALPHA_SIZE][ALPHA_SIZE];
extern int toPointLoopWeightAdded [ALPHA_SIZE];

void sccInnerGraphDpChar(Graph * rawGraph) {
    memset(sccInnerDp, 255, sizeof(sccInnerDp));

    for (int i = 0; i < sccGraph.size(); i++)
        sccInnerDp[i][i] = 0;

    //对每个强连通分量内部
    //每个起点开始dfs计算两点之间最大链长
    for (int sccId=0;sccId < sccGraph.size();sccId++) {
        for(int x : sccId2Points[sccId]){
            memset(toPointLoopWeightAdded,0,ALPHA_SIZE);
            sccInnerDfsChar(rawGraph,x, x, 0);
        }
    }
}

void sccInnerDfsChar(Graph * rawGraph,int start, int now, int length) {
    toPointLoopWeightAdded[now]++;

    //modify: -c 贪心：for邻接点选最大的
    for (int to : point2points[now]){
        if(point2pointEdges[now][to].empty()) continue;
        Edge * e = point2pointEdges[now][to].back();
        point2pointEdges[now][to].pop_back();

        int toSelfWeight = (toPointLoopWeightAdded[to] ? 0:(rawGraph->getselfLoopSum(to)));
        int newDp=length+e->getWeight()+toSelfWeight;

        sccInnerDp[start][to]=max(sccInnerDp[start][to], newDp);

        sccInnerDfsChar(rawGraph,start,to,newDp);
        point2pointEdges[now][to].push_back(e);
    }
    toPointLoopWeightAdded[now]--;
}

//graph.cpp
extern Graph* looplessGraph;
extern int point2sccID[ALPHA_SIZE];

//wordCountMax.cpp
extern int sccOuterDp[ALPHA_SIZE];
extern Edge * preEdge[ALPHA_SIZE];
extern int preSCCPoint[ALPHA_SIZE];
int LoopGraphMaxWordDPChar(Graph * rawGraph,int head, int tail) {
    if (head < 0) {
        for (int i = 0; i < ALPHA_SIZE; i++) {
            sccOuterDp[i] = (rawGraph->getselfLoopSum(i));
        }
    }else {
        memset(sccOuterDp,255,sizeof(sccOuterDp));
        sccOuterDp[head] = (rawGraph->getselfLoopSum(head));
    }

    memset(preEdge, 0, ALPHA_SIZE << 2);
    memset(preSCCPoint, 255, ALPHA_SIZE << 2);

    auto *sccTopo = new vector<int>;
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
            int to_weight = rawGraph->getselfLoopSum(end);
            if (sccOuterDp[end] < sccOuterDp[start] + (int)tmp.length() + to_weight) {
                sccOuterDp[end] = sccOuterDp[start] + (int)tmp.length() + to_weight;
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

string wordListChar[20202];
extern int wordCnt;

bool dfsSccWordMaxChainChar(Graph * rawGraph,int now, int end, int length, vector<string>* sccInnerChain) {
    if (length == 0) {
        if (end == now) {
            for (int i = 1; i <= wordCnt; i++) {
                sccInnerChain->push_back(wordListChar[i]);
            }
            return true;
        }
        else { return false; }
    }

    toPointLoopWeightAdded[now]++;

    //modify: -w不用for边，for邻接点随便选一个边就可以
    for (int to : point2points[now]){
        if(point2pointEdges[now][to].empty()) continue;
        Edge * e = point2pointEdges[now][to].back();
        point2pointEdges[now][to].pop_back();

        int lastCnt = wordCnt;
        wordListChar[++wordCnt] = e->getWord();

        int toPointSelfLoopWeight=0;
        if (toPointLoopWeightAdded[to]==0){
            toPointSelfLoopWeight = rawGraph->getselfLoopSum(to);
            if (toPointSelfLoopWeight > 0) {
                for(Edge * loopE : *(rawGraph->getSelfEdge(to))) {
                    wordListChar[++wordCnt] = loopE->getWord();
                }
            }
        }

        if (dfsSccWordMaxChainChar(rawGraph,to, end, length - e->getWeight() - toPointSelfLoopWeight, sccInnerChain)) {
            return true;
        }
        wordCnt = lastCnt;
        point2pointEdges[now][to].push_back(e);
    }
    toPointLoopWeightAdded[now]--;
    return false;
}

int printWordMaxChainChar(Graph * rawGraph,int now,char *result[]) {
    auto* loopChain = new vector<string>();
    wordCnt=0;

    bool inSCC = false;
    while (preEdge[now] || !inSCC && preSCCPoint[now] >= 0) {
        if (!inSCC && preSCCPoint[now] >= 0) {
            inSCC = true;
            int from = preSCCPoint[now];
            auto* subChain = new vector<string>();

            memset(toPointLoopWeightAdded,0,ALPHA_SIZE);
            wordCnt=0;
            dfsSccWordMaxChainChar(rawGraph,from, now, sccInnerDp[from][now], subChain);

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
            int toPointSelfLoopWeight = rawGraph->getselfLoopSum(now);
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
    int toPointSelfLoopWeight = rawGraph->getselfLoopSum(now);
    if (toPointSelfLoopWeight > 0) {
        for(Edge * loopE : *(rawGraph->getSelfEdge(now))) {
            loopChain->push_back(loopE->getWord());
        }
    }
    int len=0;
    ofstream outfile;
    outfile.open("solution.txt");

    reverse(loopChain -> begin(), loopChain -> end());
    auto s = loopChain -> begin();
    while(s != loopChain -> end()) {
        cout << (*s) << endl ;
        outfile << (*s) << endl ;
        result[len++]=(char *)(*s).data();
        s++;
    }

    outfile.close();

    return len;
}

int gen_chain_char(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop){
    if(head != 0 && !isalpha(head)) throw CoreException(-3);
    if(tail != 0 && !isalpha(tail)) throw CoreException(-3);
    if(reject != 0 && !isalpha(reject)) throw CoreException(-3);
    if(head == 0 || !isalpha(head)) head='`';
    if(tail == 0 || !isalpha(tail)) tail='`';
    if(reject == 0 || !isalpha(reject)) reject='`';

    head = (char)tolower(head);
    tail = (char)tolower(tail);
    reject = (char)tolower(reject);

    int headInt=head-'a';
    int tailInt=tail-'a';
    int rejectInt=reject-'a';

    auto * hasRGraph =new Graph(words,len);
    auto * hasRTopo = new vector<int>;
    int r=topoSort(hasRGraph, hasRTopo);
    if (r && !enable_loop) throw CoreException(-LOOP);

    auto * rawGraph =new Graph(words,len,rejectInt);
    auto *topo = new vector<int>;
    if(rawGraph->hasSelfLoop()) {
        if (!enable_loop) {
            throw CoreException(-1);
        } else {
            topoSort(rawGraph, topo);
            return charCountMaxLoop(rawGraph, headInt, tailInt, result);
        }
    }



    topoSort(rawGraph, topo);
    if(enable_loop) {

        if(r==0){
            return charCountMaxLoopless(rawGraph,topo, headInt, tailInt, result);
        }
        return charCountMaxLoop(rawGraph, headInt, tailInt, result);
    }
    if (r) throw CoreException(-LOOP);
    return charCountMaxLoopless(rawGraph,topo, headInt, tailInt, result);
}

int charCountMaxLoop(Graph * rawGraph,  int head, int tail, char * result[]) {

    removeLoop(rawGraph);

    sortByCharLoop(rawGraph);

    sccInnerGraphDpChar(rawGraph);
    int now = LoopGraphMaxWordDPChar(rawGraph,head, tail);

    return printWordMaxChainChar(rawGraph,now,result);
}

int charCountMaxLoopless(Graph * rawGraph, vector<int>* topo,int head, int tail, char * result[]) {

    sortByChar(rawGraph);
    int dpChar[ALPHA_SIZE];

    //char不能像word一样由dp结果判断是否为单词链，再加一个链长dp
    int dpLineLen[ALPHA_SIZE];
    Edge* preEdges[ALPHA_SIZE];
    //dp[i]：以字母i为结尾时，能形成的单词链最大长度
    //int cnt = graph -> getPointNum();
    memset(preEdges, 0, sizeof(preEdges));
    chainBuf = new vector<Edge*>;


    if (head < 0) {
        //单词链中对于每个字母可以存在一个首尾字符相同的单词,取排序后最大的
        for (int i = 0; i < ALPHA_SIZE; i++) {
            vector <Edge*>* k = rawGraph->getSelfEdge(i);
            dpChar[i] = (k->empty() ? 0 : (int)k->back()->getWord().length());
            dpLineLen[i] = (k->empty() ? 0 : 1);
        }
    } else {
        memset(dpChar,255,sizeof(dpChar));
        memset(dpLineLen,0,sizeof(dpLineLen));
        vector <Edge*>* k = rawGraph->getSelfEdge(head);
        dpChar[head] = (k->empty() ? 0 : (int)k->back()->getWord().length());
        dpLineLen[head] = (k->empty() ? 0 : 1);
    }

    for (int i = 0; i < ALPHA_SIZE; i++) {
        int from = (*topo)[i];
        if (dpChar[from] < 0) continue;

        for(Edge* e : *(rawGraph -> getOutEdges(from))) {
            int to = e -> getEnd();

            int endSelfWeight = (rawGraph->getSelfEdge(to)->empty() ? 0 : (int)rawGraph->getSelfEdge(to)->back()->getWord().length());
            int newDp = dpChar[from] + e->getWeight() + endSelfWeight;

            if (newDp > dpChar[to]) {
                dpChar[to] = newDp;
                dpLineLen[to] = dpLineLen[from] + 1 + (endSelfWeight ? 1 : 0);
                preEdges[to] = e;
            }
        }
    }

    //TODO 单字母

    int maxEnd = 0;
    if(tail >= 0) {
        maxEnd = tail;
    } else {
        for (int i = 1; i < ALPHA_SIZE; i++) {
            if (dpChar[i] > dpChar[maxEnd] && dpLineLen[i] > 1) {
                maxEnd = i;
            }
        }
    }

    if (dpChar[maxEnd] <= 1) {
        throw CoreException(-NO_CHAIN);
    }

    auto* wordCountMaxChain = new vector<string>();

    int nowEnd = maxEnd;
    while (preEdges[nowEnd]) {
        int nowStart = preEdges[nowEnd]->getStart();
        if (!rawGraph->getSelfEdge(nowEnd)->empty()) {
            wordCountMaxChain->push_back(rawGraph->getSelfEdge(nowEnd)->back()->getWord());
        }
        wordCountMaxChain->push_back(preEdges[nowEnd]->getWord());
        nowEnd = nowStart;
    }
    //起点自环
    if (!rawGraph->getSelfEdge(nowEnd)->empty()) {
        wordCountMaxChain->push_back(rawGraph->getSelfEdge(nowEnd)->back()->getWord());
    }


    int len=0;
    ofstream outfile;
    outfile.open("solution.txt");

    //cout << to_string(wordCountMaxChain->size()) << endl ;
    reverse(wordCountMaxChain -> begin(), wordCountMaxChain -> end());
    auto s = wordCountMaxChain -> begin();
    while(s != wordCountMaxChain -> end()) {
        cout << (*s) << endl ;
        outfile << (*s) << endl ;
        result[len++]=(char *)(*s).data();
        s++;
    }

    outfile.close();

    return (int)wordCountMaxChain->size();
}