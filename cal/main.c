#include"stack.h"
#include"calculator.h"
#include <readline/readline.h>
#include <readline/history.h>
int main(int argc, char* argv[])
{
    // 判断是否指定了保存结果的文件
    FILE* file = NULL;
    if (argc == 3 && strcmp(argv[1], "-t") == 0) {
        file = fopen(argv[2], "w");//可以通过命令行导入导出不同文件,面向对象
        if (file == NULL) {
            fprintf(stderr, "Failed to open file.\n");
            return 1;
        }
    }
    char* input;
    while (1) {
        printf("\n");
        input = readline("\033[33minteraction> \033[00m");
        // 用户输入
        if (input != NULL) {
            if (strcmp(input, "quit") == 0) {
                free(input);
                break;
            }
            //输入help时提示，并使用红色字体
            else if (strcmp(input, "help") == 0) {
                printf("\033[31mOperation commands:\n");
                printf("- Enter an expression to calculate.\n");
                printf("- Type 'quit' to exit the program.\033[00m\n");
            }
            else {
                //保存到历史
                add_history(input);
                char post[ARRSIZE];
                Transform(input, post);
                #if DEBUG
                printf("%s\n",post);
                #endif
                printf("\033[33mresult> \033[00m%lf\n", Cal_Post(post));
                // 将结果保存到文件
                if (file != NULL) {
                    Save_Result(argv[2], Cal_Post(post),input);
                }
            }
            free(input);
        }
    }
    // 关闭保存结果的文件
    if (file != NULL) {
        fclose(file);
    }
return 0;
}
