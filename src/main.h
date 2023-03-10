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

//DEBUG(x)：自定义用于debug的输出调试宏
//由此处DEBUG_MODE控制输出开关
#define DEBUG_MODE true

#define DEBUG(x) do { \
    if(DEBUG_MODE){       \
        std::cerr<<(x)<<std::endl; \
    }\
}while(0)

//THROW(x)：自定义输出异常信息的宏
struct MyException : public exception
{
private:
    /** 异常信息 */
    string Info;

public:
    /** 默认构造函数 */
    explicit MyException(string  InInfo): Info(std::move(InInfo))
    {
    }

    /** 获得异常信息
     * @return 异常信息
     */
    string GetInfo() const
    {
        return Info;
    }
};

#define THROW(x) do { \
try{ \
    x; \
}catch(MyException& e){ \
    cerr << e.GetInfo() << std::endl; \
    return 0; \
} \
}while(0)

#endif //WORDCHAIN_MAIN_H
