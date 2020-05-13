#ifndef CALC_H
#define CALC_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 20
#define MAX 255
#define EXIT "exit"

typedef struct
{
    double data[MAXSIZE];
    int top;
} sqstack;

sqstack *sqstack_init();              //创建栈
int sqstack_push(sqstack *, double);  //入栈
int sqstack_pop(sqstack *, double *); //出栈
int sqstack_top(sqstack *, double *); //查看栈顶值
int sqstack_is_empty(sqstack *);      // 判断栈是否为空

void Freebuff(char *);
double Str2Num(char *);

int get_pri(double ope);                                 //运算符优先级判断函数
void compute(sqstack *snum, double ope);                 //计算函数，将两个数出栈、根据ope符号计算，然后再次入栈
void deal_ope(sqstack *snum, sqstack *sope, double ope); //处理运算符函数，只有符号才会进入该函数
void deal_bracket(sqstack *snum, sqstack *sope);         //处理右括号函数，如果检测到符号时')',则执行该函数，参数为数字栈和符号栈

double calc(char *);

#endif // CALC_H
