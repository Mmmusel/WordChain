#include "main.h"
#include "input.h"
#include "core/Graph.h"
int main(int argc, char *argv[]){
    try{
        parseCmd(argc,argv);
    }
    catch (InPutException &e){
        cout << e.GetInfo() <<endl;
        ofstream outfile;
        outfile.open("solution.txt");
        outfile.close();
        return 0;
    }
    catch (CoreException &ee){
        if(ee.GetInfo()!="CoreError: No Chain"){
            cout << ee.GetInfo() <<endl;
        }
        ofstream outfile;
        outfile.open("solution.txt");
        outfile.close();

        return 0;
    }

}
int parseCmd(int argc, char *argv[]) {
    vector<int> filename;
    bool loop_enabled=false;

    vector<char> chainCmd;
    char htj[3]={'`','`','`'};

    for (int i=1;i<argc;i++) {
        char* str = argv[i];
        int len = (int)strlen(str);

        if(len<2) throw InPutException("Cmd Input Error: cmd format error");

        if (str[0]=='-'){
            if (len > 2) {
                if (len > 3) {
                    if (str[len - 4] == '.' && str[len - 3] == 't' && str[len - 2] == 'x' && str[len - 1] == 't')
                        filename.push_back(i);
                    else throw InPutException("Cmd Input Error: cmd format error");
                }
                else throw InPutException("Cmd Input Error: cmd format error");
            }
            if (str[1]=='n' || str[1]=='w' || str[1]=='c') {
                if(chainCmd.empty())
                {
                    chainCmd.push_back(str[1]);
                }
                else throw InPutException("Cmd Input Error: to many cmd. -n -w -c can only choose one and use once");
            } else if (str[1] == 'r'){
                loop_enabled = true;
            } else if (str[1]=='h' || str[1]=='t' || str[1]=='j') {
                char x = str[1];
                if (i<argc-1) {
                    i++;
                    if(strlen(argv[i])!=1) {
                        string errorInfo = "Cmd Input Error: ";
                        errorInfo.append(str);
                        errorInfo.append(" must followed by one char");
                        throw InPutException(errorInfo);
                    }

                    int charCmd;
                    if (x=='h') {
                        charCmd=0;
                    } else if (x=='t') {
                        charCmd=1;
                    } else charCmd=2;
                    if (htj[charCmd]!='`') {
                        string errorInfo = "Cmd Input Error: redeclaration of";
                        errorInfo.append(str);
                        throw InPutException(errorInfo);
                    }

                    char c=argv[i][0];
                    if (isupper(c)) c= tolower(c);
                    if (islower(c)) {
                        htj[charCmd]=c;
                    } else {
                        string errorInfo = "Cmd Input Error: ";
                        errorInfo.append(str);
                        errorInfo.append(" must followed by a char");
                        throw InPutException(errorInfo);
                    }
                } else {
                    string errorInfo = "Cmd Input Error: ";
                    errorInfo.append(str);
                    errorInfo.append(" must followed by a char");
                    throw InPutException(errorInfo);
                }
            } else {
                throw InPutException("Cmd Input Error: undefined cmd.");
            }
        }
        else if (len > 3) {
            if (str[len-4]=='.' && str[len - 3] == 't' && str[len - 2] == 'x' && str[len - 1] == 't')
                filename.push_back(i);
            else throw InPutException("File Input Error: filename must be *.txt");
        } else throw InPutException("Cmd Input Error: cmd format error");
    }
    if (chainCmd.empty())
        throw InPutException("Cmd Input Error: no chain cmd. -n -w -c must use one");

    if (filename.empty()) {
        throw InPutException("File Input Error: please enter a filename");
    } else if (filename.size()>1) {
        throw InPutException("File Input Error: you can only input one file");
    }

    char cmdType = chainCmd.back();
    if(cmdType=='n') {
        if (loop_enabled) throw InPutException("Cmd Input Error: -n should use seperately without -r");
        if (htj[0]!='`' || htj[1]!='`' || htj[2]!='`') throw InPutException("Cmd Input Error: -n should use seperately without -h -t -j");

    }

    if (htj[0]!='`' && htj[0] == htj[2])
        throw InPutException("-h -j use same char. No chain.");


    vector<char*> words(32768, nullptr);
    int len = splitWord(words.data(),argv[filename.front()]);
    if (len<2) {
        ofstream outfile;
        outfile.open("solution.txt");
        outfile.close();
        return 0;
    }

    vector<char*> result(32768, nullptr);

    if(cmdType=='n') {
        gen_chains_all(words.data(),len,result.data());
    } else if (cmdType=='w') {
        gen_chain_word(words.data(),len,result.data(), htj[0]=='`'?0:htj[0],htj[1]=='`'?0:htj[1],htj[2]=='`'?0:htj[2],loop_enabled);
    } else {
        gen_chain_char(words.data(),len,result.data(),
                       htj[0]=='`'?0:htj[0],htj[1]=='`'?0:htj[1],htj[2]=='`'?0:htj[2],loop_enabled);
    }

    return 0;
}
