#include <set>
#include "input.h"

set<string> word;

int splitWord(char *words[],const char *fileName) {
    FILE *file = nullptr;
    fopen_s(&file, fileName, "r");

    if (file == nullptr) {
        throw InPutException("File Input Error: file not exist");
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

    //g->calOutPoints();
    int len=0;
    for (const string& w : word) {
        words[len++]=(char *)w.data();
    }

    return len;
}

//TODO 单字符单词如何处理
void checkBuf(string& wordBuf) {
    if (wordBuf.length() > 0) {
        word.insert(wordBuf);
        wordBuf.clear();
    }
}