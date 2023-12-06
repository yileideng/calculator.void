#ifndef CALCULATOR_H
#define CALCULATOR_H

#include"stack.h"
#define ARRSIZE 50  //数组大小
#define DEBUG 0     //调试模式
//计算器相关函数
Status Transform(char* str, char* post);
/*
将中缀表达式转化为后缀表达式
参数 str为中缀表达式指针 post为后缀表达式指针
返回值OK表示成功，ERROR表示失败
*/

double Cal_Post(char* post);
/*
计算后缀表达式
参数为后缀表达式的指针
返回值为计算结果
*/
void Clear_Input_Buffer() ;
void Save_Result(const char* file, double result,char* input) ;
/*
将结果保存在文件中
第一个参数为保存文件的指针，第二个参数为计算结果
*/

#endif
