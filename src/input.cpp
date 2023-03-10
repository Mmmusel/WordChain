#include "input.h"

vector<string> word[ALPHA_SIZE][ALPHA_SIZE];

void splitWord(const char *fileName) {
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
            checkBuf(wordBuf);
        }
    }
    checkBuf(wordBuf);
    fclose(file);
}

//TODO 单字符单词如何处理
void checkBuf(string& wordBuf) {
    if (wordBuf.length() > 0) {
        int start = wordBuf.front() - 'a';
        int end = wordBuf.back() - 'a';
        word[start][end].push_back(wordBuf);

        DEBUG(wordBuf);
        wordBuf.clear();
    }
}