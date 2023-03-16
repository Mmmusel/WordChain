#ifndef WORDCHAIN_MAIN_H
#define WORDCHAIN_MAIN_H

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <cstring>
using namespace std;

#define ALPHA_SIZE 26

#define LOOP 1
#define NO_CHAIN 2

int parseCmd(int argc, char *argv[]);
//THROW(x)：自定义输出异常信息的宏
struct InPutException : public exception
{
private:
    /** 异常信息 */
    string Info;

public:
    /** 默认构造函数 */
    explicit InPutException(string  InInfo): Info(std::move(InInfo))
    {
    }

    explicit InPutException(int errorCode)
    {
        if (errorCode==-1) {
            Info = "ERROR: ";
        } else {
            Info = "ERROR";
        }
    }

    /** 获得异常信息
     * @return 异常信息
     */
    string GetInfo() const
    {
        return Info;
    }
};

struct CoreException : public exception
{
private:
    /** 异常信息 */
    string Info;

public:
    /** 默认构造函数 */
    explicit CoreException(string  InInfo): Info(std::move(InInfo))
    {
    }

    explicit CoreException(int errorCode)
    {
        if (errorCode==-1) {
            Info = "CoreError: LOOP!";
        } else if(errorCode==-2){
            Info = "CoreError: No Chain";
        }else if(errorCode==-3){
            Info = "CoreError: -h -j -t must be a char";
        }else {
            Info = "CoreError";
        }
    }

    /** 获得异常信息
     * @return 异常信息
     */
    string GetInfo() const
    {
        return Info;
    }
};
/*
struct MyException : public exception
{
private:

    string Info;

public:

    explicit MyException(string  InInfo): Info(std::move(InInfo))
    {
    }

    explicit MyException(int errorCode)
    {
        if (errorCode==-1) {
            Info = "CoreError: LOOP!";
        } else {
            Info = "CoreError: undefined ERROR";
        }
    }


    string GetInfo() const
    {
        return Info;
    }
};
*/


#endif //WORDCHAIN_MAIN_H
