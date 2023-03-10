#include "main.h"
#include "input.h"

#define FILENAME "D:\\PROJECTS\\lj\\input.txt"

int main() {
    Graph* g = new Graph(ALPHA_SIZE);

    THROW(splitWord(FILENAME, g));

    return 0;
}
