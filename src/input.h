#ifndef WORDCHAIN_INPUT_H
#define WORDCHAIN_INPUT_H

# include "main.h"
#include "node/Graph.h"

void splitWord(const char *fileName, Graph * g, int reject);
void checkBuf(string& wordBuf, Graph * g, int reject);

#endif //WORDCHAIN_INPUT_H
