#include "main.h"
#include "input.h"
#include "core/Graph.h"

int main(int argc, char *argv[]) {
    vector<int> filename;
    bool loop_enabled=false;

    vector<char> chainCmd;
    char htj[3]={'`','`','`'};

    for (int i=1;i<argc;i++) {
        char* str = argv[i];
        int len = (int)strlen(str);

        if(len<2) continue;

        if (str[0]=='-'){
            if (str[1]=='n' || str[1]=='w' || str[1]=='c') {
                if(!chainCmd.empty())

                THROW(throw MyException("to many cmd. -n -w -c can only use one"));

                chainCmd.push_back(str[1]);
            } else if (str[1] == 'r'){
                loop_enabled=true;

            } else if (str[1]=='h' || str[1]=='t' || str[1]=='j') {
                char x = str[1];
                if (i<argc) {
                    i++;
                    if(strlen(argv[i])!=1) {
                        THROW(throw MyException(to_string((str[1]))+" must follow a char"));
                    }

                    int charCmd;
                    if (x=='h'){
                        charCmd=0;
                    } else if (x=='t') {
                        charCmd=1;
                    } else charCmd=2;
                    if (htj[charCmd]!='`'){
                        THROW(throw MyException("redeclare  cmd of "+ to_string(x)));
                    }

                    char c=argv[i][0];

                    if (isupper(c)) c= tolower(c);
                    if (islower(c)) {
                        htj[charCmd]=c;
                    } else {
                        THROW(throw MyException(to_string((str[1]))+" must follow a char"));
                    }

                } else {
                    THROW(throw MyException(to_string((str[1]))+" must follow a char"));
                }
            }
        }
        else if (len > 3) {
            if (str[len - 3] == 't' && str[len - 2] == 'x' && str[len - 1] == 't')
                filename.push_back(i);
        }
    }

    if (filename.empty()) {
        THROW(throw MyException("please enter right filename"));
        return 0;
    } else if (filename.size()>1) {
        THROW(throw MyException("filename must == 1"));
        return 0;
    }

    vector<char*> words(32768, nullptr);
    int len=0;
    THROW(len = splitWord(words.data(),argv[filename.front()],htj[2]-'a'));

    FILE *file = nullptr;
    fopen_s(&file, argv[filename.front()], "r");

    if (file == nullptr) {
        throw MyException("file not exist");
    }

    vector<char*> result(32768, nullptr);
    char x = chainCmd.back();
    try{
    if(x=='n') {
        gen_chains_all(words.data(),len,result.data());

    } else if (x=='w') {
        gen_chain_word(words.data(),len,result.data(),
                       htj[0],htj[1],htj[2],loop_enabled);

    } else {
        gen_chain_char(words.data(),len,result.data(),
                       htj[0],htj[1],htj[2],loop_enabled);
    }}
    catch (MyException &e){
        cout << e.GetInfo() <<endl;
        return 0;
    }
    return 0;
}
