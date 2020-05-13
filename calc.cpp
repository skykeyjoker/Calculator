#include "calc.h"


/*清空临时数组svalue*/
void Freebuff(char *svalue)
{
    int len = strlen(svalue);

    for (int i = 0; i < len; i++)
    {
        svalue[i] = '\0';
    }
}

/*将临时数组svalue转化为数字*/
double Str2Num(char *svalue)
{
    int len;
    int pos = -1;
    double ans = 0;
    bool minus = false;

    len = strlen(svalue);

    if (svalue[0] == '-') //判断是否为负数
    {
        minus = true;
    }

    for (int i = 0; i < len; i++)
    {
        if (svalue[i] == '.')
        {
            pos = i;
            continue;
        }
        else if (svalue[i] >= '0' && svalue[i] <= '9')
            ans = ans * 10 + svalue[i] - '0';
    }

    if (pos != -1)
    {
        ans = ans * pow(0.1, len - pos - 1);
    }

    if (minus) //如果为负数
    {
        ans = ans * -1;
    }

    return ans;
}

/* 运算符优先级判断函数 */
int get_pri(double ope)
{
    switch ((int)ope)
    {
    case '(':
        return 0;
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
    case '%':
        return 2;
    case '^':
    case '@':
        return 3;
    default:
        return -1;
    }
}

/* 将两个数出栈、根据ope符号计算，然后再次入栈 */
void compute(sqstack *snum, double ope)
{
    double n, n1, n2;
    //依次获得数值栈的栈顶两个数
    sqstack_pop(snum, &n1);
    sqstack_pop(snum, &n2);
    //计算
    switch ((int)ope)
    {
    case '+':
        n = n1 + n2;
        break;
    case '-':
        n = n2 - n1;
        break;
    case '*':
        n = n1 * n2;
        break;
    case '/':
        n = n2 / n1;
        break;
    case '%':
        n = fmod(n2, n1);
        break;
    case '^':
        n = pow(n2, n1);
        break;
    case '@':
        n = pow(n2, 1 / n1);
        break;
    }
    //计算完成再次入栈
    sqstack_push(snum, n);
}

/* 只有符号才会进入该函数 */
void deal_ope(sqstack *snum, sqstack *sope, double ope)
{
    double old_ope;
    //如果sope符号栈是空栈或者符号为‘(’
    if (sqstack_is_empty(sope) || (int)ope == '(')
    {
        //将括号(入栈
        sqstack_push(sope, ope);
        return;
    }
    //获得当前的符号栈的栈顶
    sqstack_top(sope, &old_ope);

    //将当前的符号栈的栈顶符号与传入的符号进行优先级比较
    if (get_pri(ope) > get_pri(old_ope))
    {
        //传入符号大于当前栈顶，则将传入符号入栈
        sqstack_push(sope, ope);
        return;
    }
    //如果传入的符号优先级小于当前栈顶符号
    while (get_pri(ope) <= get_pri(old_ope))
    {
        //将当前栈顶的符号取出与数字栈中顶端的两个数字进行计算
        sqstack_pop(sope, &old_ope);
        compute(snum, old_ope);
        //如果计算完毕之后符号栈为空则break;
        if (sqstack_is_empty(sope))
        {
            break;
        }
        //再次取出一个当前栈符号与传入符号比较
        sqstack_top(sope, &old_ope);
    }
    //进行完毕优先级计算之后，再将新传入的符号入栈
    sqstack_push(sope, ope);
}

/*如果检测到符号时')',则执行该函数，参数为数字栈和符号栈*/
void deal_bracket(sqstack *snum, sqstack *sope)
{
    double old_ope;
    //获得当前的符号栈的栈顶符号
    sqstack_top(sope, &old_ope);
    //直到找到预期配对的左括号。因为正确的算式中左右括号一定是配对的
    while ((double)old_ope != '(')
    {
        //当前符号出栈然后将数字出栈两个进行计算,在括号内优先级最高，
        sqstack_pop(sope, &old_ope);
        compute(snum, old_ope);
        //然后再次取出当前符号栈栈顶符号，至到出现‘(’
        sqstack_top(sope, &old_ope);
    }
    //最后将出现的左扩号出栈丢弃
    sqstack_pop(sope, &old_ope);
}

/* 创建栈 */
sqstack *sqstack_init()
{
    sqstack *L;

    L = (sqstack *)malloc(sizeof(*L));

    L->top = -1;

    return L;
}

/* 入栈 */
int sqstack_push(sqstack *L, double x)
{
    //如果入栈超出栈空间
    if (L->top == MAXSIZE - 1)
    {
        return -1;
    }
    //压栈
    L->data[++L->top] = x;
    return 0;
}

/* 出栈 */
int sqstack_pop(sqstack *L, double *x)
{
    //如果出栈超出栈空间
    if (L->top == -1)
    {
        return -1;
    }
    //利用传出参数传出栈顶元素
    *x = L->data[L->top--];
    return 0;
}

/* 获得栈顶值 */
int sqstack_top(sqstack *L, double *x)
{
    if (L->top == -1)
    {
        return -1;
    }

    *x = L->data[L->top];
    return 0;
}

/* 判断栈是否为空 */
int sqstack_is_empty(sqstack *L)
{
    return (L->top == -1);
}

double calc(char *str)
{
    int i = 0;
    int k = 0;
    char svalue[MAXSIZE]; //临时存取字符型数字
    double value = 0;     //数字的值
    int flag = 0;         //标记是否有数字
    double old_ope;       //临时存取运算符

    sqstack *snum, *sope; // 定义两个指向栈结构体的指针

    snum = sqstack_init(); // 创建存放数字的栈

    sope = sqstack_init(); // 创建存放运算符号的栈

    /* 表达式字符串解析函数,然后将高优先级的符号/(*)进行计算重新入栈
                    退出while大家的优先级都一样+-*/
    while (str[i] != '\0')
    {
        //获取输入的数字

        if (str[i] == '-') //对负数进行预判
        {
            if (i == 0) //第一个数字是负数的情况
            {
                svalue[k++] = str[i];
                flag = 1;
                i++;
                continue;
            }

            if (str[i - 1] == '(' || str[i - 1] == '-') //其余负数的情况，例如(-1)或者--1
            {
                svalue[k++] = str[i];
                flag = 1;
                i++;
                continue;
            }
        }

        if ((str[i] >= '0' && str[i] <= '9') || (str[i] == '.')) //num
        {

            //value = value * 10 + str[i] - '0';
            svalue[k++] = str[i];
            flag = 1;
        }
        else //ope
        {
            if (flag)
            {
                //flag = 1说明value里面存储了数字，将其入栈
                //转化数字
                value = Str2Num(svalue);
                sqstack_push(snum, value);

                //flag数字标志清零，value存放数字的变量清零
                flag = 0;
                value = 0;
                //k归零，清空临时字符数组svalue
                Freebuff(svalue);
                k = 0;
            }
            if ((int)str[i] == ')')
            {
                //如果是右括号，则
                deal_bracket(snum, sope);
            }
            else //+-*/(等情况
            {
                deal_ope(snum, sope, str[i]);
            }
        }
        i++;
    }
    //如果flag = 1.说明value里面还有数值,将其入栈
    if (flag)
    {
        value = Str2Num(svalue);
        sqstack_push(snum, value);
    }
    //然后将符号与数字依次出栈计算。数字出栈计算完成之后回再次在compute中入栈
    while (!sqstack_is_empty(sope))
    {
        sqstack_pop(sope, &old_ope);
        compute(snum, old_ope);
    }
    //取出数字栈最后剩下的数字，就是最后的答案
    sqstack_pop(snum, &value);

    //结果
    return value;

    //释放空间
    free(snum);
    free(sope);
}
