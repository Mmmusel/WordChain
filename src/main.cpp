#include "main.h"
#include "input.h"

#define FILENAME "D:\\PROJECTS\\lj\\input.txt"

Graph * rawGraph;
int main() {


    rawGraph= new Graph(ALPHA_SIZE);

    THROW(splitWord(FILENAME, rawGraph));

    return 0;
}
