#include"calculator.h"
//将中缀表达式转化为后缀表达式
Status Transform(char* str, char* post) {
    //str为中缀表达式字符串，p为后缀表达式字符串

    Stack* stack = Stack_Construct(sizeof(char));
    char cTemp, cChar;
    int i, j = 0;
    for (i = 0; i < strlen(str); i++) { //遍历中缀表达式
        //遍历中缀表达式到数字和小数点,直接输出
        while (isdigit(str[i]) || '.' == str[i]) {
            post[j++] = str[i++];
            if (!isdigit(str[i]) && '.' != str[i])
                post[j++] = ' ';   //一个数字输出后使用空格与其它运算符分隔开
        }

        //遍历中缀表达式到左括号直接入栈
        if ('(' == str[i]) {
            Stack_Push(stack, &str[i]);
        }

        //遍历中缀表达式到右括号,直接将栈内符号出栈，直到栈顶为左括号
        if (')' == str[i]) {
            while (1) {
                cChar = *((char*)stack->pTop - 1);
                if ('(' == cChar) break;
                Stack_Pop(stack, &cTemp);
                post[j++] = cTemp;
                post[j++] = ' ';

            }
            Stack_Pop(stack, &cTemp);  //左括号出栈但不输出
        }

        //遍历中缀表达式到+或—
        if ('+' == str[i] || '-' == str[i]) {
            while (1) {
                cChar = *((char*)stack->pTop - 1);
                if (Is_Empty(stack) || '(' == cChar) break;
                Stack_Pop(stack, &cTemp);//中缀表达式中+-优先级最低，直接把字符栈中运算符出栈
                post[j++] = cTemp;
                post[j++] = ' ';
            }
            if ('(' == str[i - 1] || i == 0) { //支持负数，如：当（-5）入栈时，在后缀表达式中添加0，将（-5）表示成0-5；
                post[j++] = '0';
                post[j++] = ' ';
            }
            Stack_Push(stack, &str[i]);//把+-入栈
        }

        //遍历中缀表达式到*或/
        if ('*' == str[i] || '/' == str[i] || '%' == str[i]) {
            while (1) {
                cChar = *((char*)stack->pTop - 1);
                if (Is_Empty(stack) || '(' == cChar || '+' == cChar || '-' == cChar) break;
                Stack_Pop(stack, &cTemp);//栈中除了+-，其它运算符优先级更高，将其出栈
                post[j++] = cTemp;
                post[j++] = ' ';
            }
            Stack_Push(stack, &str[i]);//将*/入栈
        }

        //遍历中缀表达式到三角函数
        if (strncmp(&str[i], "sin", 3) == 0 || strncmp(&str[i], "cos", 3) == 0 || strncmp(&str[i], "tan", 3) == 0) {
            Stack_Push(stack, &str[i]);//将*/入栈
            i += 2;
        }
        if (strncmp(&str[i], "arcsin", 6) == 0) {
            char ch = 'q';
            Stack_Push(stack, &ch);//将*/入栈
            i += 5;
        }
        if (strncmp(&str[i], "arccos", 6) == 0) {
            char ch = 'w';
            Stack_Push(stack, &ch);//将*/入栈
            i += 5;
        }
        if (strncmp(&str[i], "arctan", 6) == 0) {
            char ch = 'e';
            Stack_Push(stack, &ch);//将*/入栈
            i += 5;
        }
        if (strncmp(&str[i], "square", 6) == 0) {
            char ch = 'r';
            Stack_Push(stack, &ch);//将*/入栈
            i += 5;
        }
        //遍历中缀表达式到^
        if ('^' == str[i]) {
            Stack_Push(stack, &str[i]);
        }
        //判断输入的表达式是否错误
        if (('(' == str[i]) && (')' == str[i + 1])) {
            printf("\033[31mERROR：表达式有误\033[00m\n");
            return ERROR;
        }
    }

    //判断栈空,非空则直接出栈并输出
    while (!Is_Empty(stack)) {
        Stack_Pop(stack, &cTemp);
        if ('(' != cTemp) {
            post[j++] = cTemp;
            post[j++] = ' ';
        }
    }
    post[--j] = '\0';
    StackDestruct(stack);
    return OK;
}
//计算后缀表达式的结果:
double Cal_Post(char* post) {
    double temp, dPre, dPost;
    char d[ARRSIZE];
    Stack* stack = Stack_Construct(sizeof(double));
    for (int i = 0; i < strlen(post); i++) {
        int j = 0;//每次for循环j都被重新赋值成0
        while (isdigit(post[i]) || '.' == post[i]) { //一次循环可以将一个数据转换成double类型并入栈
            d[j++] = post[i++];//把字符串里的数字和'.'放到另一个字符串中，以将其转换为double类型
            d[j] = '\0';//atof函数读取到'\0’时自动停止

            if (!isdigit(post[i]) && '.' != post[i]) { //当str中读取的不是数字(即str[]中的运算符)后，将d[]中字符转换成double类型
                temp = atof(d);   //使用atof()将字符串形式的运算数转换为double型数据                
                Stack_Push(stack, &temp);   //将类型转化成double后的运算数入栈
            }
        }
        int c, d;//c,d用来把double类型的数强制转换成int类型 以进行取余运算
        if (post[i] == '+' || post[i] == '-' || post[i] == '*' || post[i] == '/' || post[i] == '%' || post[i] == '^') {
            Stack_Pop(stack, &dPost);
            Stack_Pop(stack, &dPre);
            switch (post[i]) {
            case '+':
                dPre += dPost;
                break;
            case '-':
                dPre -= dPost;
                break;
            case '*':
                dPre *= dPost;
                break;
            case '/':
                if (dPost == 0) {
                    printf("\033[31mERROR：分母不能为0\033[00m\n");//分母为0时：ERROR
                    return ERROR;
                }
                dPre /= dPost;
                break;
            case '%':
                c = (int)dPre;
                d = (int)dPost;
                c %= d;
                dPre = (double)c;
                break;
            case '^':
                c = (int)dPre;
                if (dPost == 0) {
                    dPre = 1;
                }
                else {
                    for (int i = 0; i < dPost - 1; i++) {
                        dPre *= c;
                    }
                }
                break;
            }
            Stack_Push(stack, &dPre);
        }
        if (post[i] == 's' || post[i] == 'c' || post[i] == 't' || post[i] == 'q' || post[i] == 'w' || post[i] == 'e' || post[i] == 'r') {
            Stack_Pop(stack, &dPost);
            switch (post[i]) {
            case 's'://sin
                dPre = sin(dPost);
                break;
            case 'c'://cos
                dPre = cos(dPost);
                break;
            case 't'://tan
                dPre = tan(dPost);
                break;
            case 'q'://arcsin
                dPre = asin(dPost);
                break;
            case 'w'://arccos
                dPre = acos(dPost);
                break;
            case 'e'://arctan
                dPre = atan(dPost);
                break;
            case 'r'://arctan
                dPre = sqrt(dPost);
                break;
            }
            Stack_Push(stack, &dPre);
        }
    }
    Stack_Pop(stack, &temp);//将栈中最后计算出的结果放到变量e中
    StackDestruct(stack);
    return temp;
}
void Clear_Input_Buffer() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

//将结果写入文件
void Save_Result(const char* file,double result,char* input) {
    FILE* fp = fopen(file, "a");
    if (fp != NULL) {
        fprintf(fp,"%s=%lf\n",input,result);
        fclose(fp);
    }
}
