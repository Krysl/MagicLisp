#include "stdio.h"

#ifdef __WIN32
#include "conio.h"
#define KEYNO_ENTER     13
#define KEYNO_BACKSPACE  8
#endif // __WIN32

#ifdef linux
#include <termios.h>
#define KEYNO_ENTER      10
#define KEYNO_BACKSPACE 127
static struct termios oldt;
//restore terminal settings
void restore_terminal_settings(void)
{
    // Apply saved settings
    tcsetattr(0, TCSANOW, &oldt);
}
//make terminal read 1 char at a time
void disable_terminal_return(void)
{
    struct termios newt;

    //save terminal settings
    tcgetattr(0, &oldt);
    //init new settings
    newt = oldt;
    //change settings
    newt.c_lflag &= ~(ICANON | ECHO);
    //apply settings
    tcsetattr(0, TCSANOW, &newt);

    //make sure settings will be restored when program ends
    atexit(restore_terminal_settings);
}
#endif // linux

#include "string.h"
#include "malloc.h"
#include "BaseCalculation.h"
#include "MLEngine.h"
#include "MLLib.h"
#include "MLDebug.h"
extern struct list_head MLWorkList;
extern int g_suspend;
extern int g_stop;
extern void *g_arg[9];
extern int g_debug_on;


int main()
{
    int a = 1, b = 2, c = 3, ret = 0, r=0;
    //int i;
    char inputBuffer[20000], ch, *pch;
    char cmdArgs[ARG_NUM][ARG_CHARLEN];
    //char fnArg[3][20];
    //char arg[9];
//    MLFn pFn;
//    EMLFnType fnType;
    struct list_head *tmpFL;
    printf("MagicLisp ver 0.01\n");
    printf("\"Simple test for MLEngine\"\n");
    printf("\tLoad test Fns...\n");
    //printf("中文测试\n");
    MLLib_init();
    MLEngine_init();
    LIST_HEAD(funlist_test);
    g_arg[3] = &b;
    g_arg[4] = &c;
    g_arg[5] = &r;
    MLFLAddFn((MLFn)BC_plus, MLFnT3, (void **)(g_arg+3), 0b0000, &funlist_test);
    g_arg[0] = &a;
    g_arg[1] = &funlist_test;
    g_arg[2] = &ret;
    g_suspend = false;
    g_stop = false;
    g_debug_on = true;
    MLWLAddFn((MLFn)BC_plus, MLFnT3, g_arg, 0b0100);
    printf("\tTest Fns Load Finished\n");
#ifdef linux
    disable_terminal_return();
#endif // linux
    while(1)
    {
        printf("\n>");
        pch = inputBuffer;
        while(1)
        {
#ifdef __WIN32
            ch = getch();
#endif // __WIN32
#ifdef linux
            ch = getchar();
#endif // linux
            if(ch == -1)
                continue;
            if(ch == KEYNO_ENTER)//enter
                break;
            if(ch == 3)//Ctrl+C
                goto EXIT;
            if(ch == KEYNO_BACKSPACE)//backspace
            {
                printf("\b \b");
                pch--;
            }
            else
            {
                printf("%c", ch);
                //printf("%c %d\n", ch, ch);
                *pch++ = ch;
            }
        }
        printf("\n");
        *pch = 0;
        if (!strcmp(inputBuffer,"run"))
        {
            MLEngineRun(&MLWorkList);
            printf("\n");
        }
        else if (!strcmp(inputBuffer,"exit"))
            goto EXIT;
        else
        {
            pch = inputBuffer;
#if 0 //simple support forms like "fun arg0 arg1 arg2"
            getAllArgs(cmdArgs, pch);
            pFn = getFnByName(cmdArgs[0], &fnType);
//EXEC://execute
            if (pFn)
            {
                switch(fnType)
                {
                case MLFnT0:
                    (*(MLFn0)pFn)();
                    break;
                case MLFnT1:
                    (*(MLFn1)pFn)(cmdArgs[1]);
                    break;
                case MLFnT2:
                    (*(MLFn2)pFn)(cmdArgs[1], cmdArgs[2]);
                    break;
                case MLFnT3:
                    (*(MLFn3)pFn)(cmdArgs[1], cmdArgs[2], cmdArgs[3]);
                    break;
                default:
                    break;
                }
            }
#else
            pch = getFirstArg(cmdArgs[0], pch);
            if(!strcmp(cmdArgs[0],"("))
            {
                printf("编译链接...\n");
                tmpFL = (struct list_head *)add2rootWL(&pch);
                printf("执行...\n");
                MLEngineRun(tmpFL);
                //printf("\n");
                //MLEngineRun(tmpFL);
                free(tmpFL);
                printf("退出\n");
            }
            else
            {
                printf("unknow cmd:%s.maybe you need input \"(\" first\n",cmdArgs[0]);
            }
#endif
        }
    }
EXIT:
    printf("\n>exiting...\n");
    printf("ret = %d\n", ret);
#ifdef linux
    restore_terminal_settings();
#endif // linux
    return ret;
}


