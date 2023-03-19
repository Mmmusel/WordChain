# 结对编程项目说明

## 模块介绍

bin：命令行程序Wordlist.exe及依赖的计算模块的动态链接库core.dll

- 关于命令行程序Wordlist.exe的运行说明：在`bin`目录下使用命令`Wordlist.exe `+命令参数 + `*.txt`的文件绝对路径（其中命令参数与txt路径的顺序可以任意）
- Wordlist.exe必须与core.dll在相同目录下才能正常运行

guibin：Vuetify风格前端的可执行文件GUI_Wordlist.exe

- 使用基于Vue编写的Material Design框架Vuetify提供的组件编写的前端模块。
- 使用`node-ffi-napi`库完成Node.js环境与dll的对接。

src：项目源码，包含core模块源码与命令行程序模块源码。

- 注：src源码为可直接编译出可执行文件并运行的面向开发过程的源码；在打包命令行程序时，我们使用Visual Studio2019进行对core.dll和Wordlist.exe在`Release Win32`模式下的打包，对计算模块打包时给源码增加`pch.h`等相关头文件并使用`extern "C" _declspec(dllexport)`声明API即可。

test：使用Visual Studio2019的UnitTest框架实现的单元测试模块源码

## 图形化界面运行说明

无环境配置要求，目录下不需要放置dll文件，双击`guibin/GUI_Wordlist.exe`即可使用。以下是图形化界面的说明：

- 【参数说明板块（左上）】使用Vuetify的`v-card`卡片组件和`v-tabs`选项卡组件来在固定区域展示各参数的含义和详细介绍信息，包括`n,w,c,h,t,j,r`七个参数
- 【命令参数板块（左下）】使用`Vuetify`的`v-card`卡片组件和`v-list`列表组件来支持切换不同功能性参数（`n,w,c`），并通过右列给出功能性参数允许的附加型参数（`h,t,r,j`）。
- 【文件输入板块（右上）】使用`v-btn`按钮组件和`v-card-text`卡片输入框组件来组合实现文本输入部分，支持直接输入单词、从本地选择并导入文本文件、清空文本输入框以及求解四个功能。
- 【 结果输出板块（右下）】使用`v-btn`按钮组件、`v-card-text`文本框组件和`v-chip`纸片组件来完成输出板块，支持将输出导出为文本文件，清空输出框以及展示计算用时的功能。

## 运行结果说明

- 命令行程序：
  - 正确运行时会在命令行窗口输出正确结果并在同目录下生成包含正确结果的`solution.txt`
  - 对于合法指令与和合法输入且不存在单词链时，命令行窗口的结果与`solution.txt`中均为空，不报错
  - 对于非法指令或无效的输入文件，命令行窗口会输出异常类型的信息提示，同时`solution.txt`中为空
- GUI程序：
  - 运行结果显示在前端，并在同目录下生成包含正确结果的`solution.txt`
  - 输入支持前端键入与文件导入
  - 输出支持前端显示与文件导出