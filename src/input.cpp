#include <set>
#include "input.h"

set<string> word;
extern Graph * rawGraph;
void splitWord(const char *fileName, Graph * g, int reject) {
    FILE *file = nullptr;
    fopen_s(&file, fileName, "r");

    if (file == nullptr) {
        throw MyException("file not exist");
    }

    int c;
    string wordBuf;

    while ((c = fgetc(file)) && c != EOF) {
        if (isupper(c)) {
            c = tolower(c);
        }
        if (islower(c)) {
            wordBuf += (char)c;
        } else {
            checkBuf(wordBuf, g, reject);
        }
    }
    checkBuf(wordBuf, g, reject);
    fclose(file);

    //g->calOutPoints();

    rawGraph=new Graph(word);
    g=rawGraph;

    //参数命令
    //getAllChain(g);
    //wordCountMaxLoopless(g,-1,-1);
    //wordCountMaxLoop(g,-1,-1);
    //charCountMaxLoopless(g,-1,-1);
    charCountMaxLoop(g,-1,-1);


}

//TODO 单字符单词如何处理
void checkBuf(string& wordBuf, Graph * g ,int reject) {
    if (wordBuf.length() > 0) {
        int start = wordBuf.front() - 'a';
        if (start != reject) {
            word.insert(wordBuf);
            //g->addEdge(new Edge(wordBuf));
            DEBUG(wordBuf);
        }
        wordBuf.clear();
    }
}