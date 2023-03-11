#include "input.h"

vector<string> word[ALPHA_SIZE][ALPHA_SIZE];

void splitWord(const char *fileName, Graph * g) {
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
            checkBuf(wordBuf, g);
        }
    }
    checkBuf(wordBuf, g);
    fclose(file);

    //参数命令
    getAllChain(g);
    wordCountMaxLoopless(g);
}

//TODO 单字符单词如何处理
void checkBuf(string& wordBuf, Graph * g) {
    if (wordBuf.length() > 0) {
        int start = wordBuf.front() - 'a';
        int end = wordBuf.back() - 'a';
        word[start][end].push_back(wordBuf);
        g->addEdge(new Edge(wordBuf));

        DEBUG(wordBuf);
        wordBuf.clear();
    }
}