#ifndef WORDCHAIN_EDGE_H
#define WORDCHAIN_EDGE_H

#include "../main.h"

class Edge {
private:
    int start, end;
    int weight;
    string word;

public:
    Edge(string& _word) {
        start = _word.front() - 'a';
        end = _word.back() - 'a';
        weight = _word.length();
        word = _word;
    }

    Edge(const string& _word, int _s, int _e) {
        word = _word;
        weight = _word.length();
        start = _s;
        end = _e;
    }

    int getStart() {
        return start;
    }
    int getEnd() {
        return end;
    }
    int getWeight() {
        return weight;
    }
    string getWord(){
        return word;
    }
};

#endif //WORDCHAIN_EDGE_H
