#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "BaseCalculation.h"
#include "MLLib.h"
#include "LinuxList.h"
#include "MLDebug.h"
extern struct list_head MLFnTree;
VarNode_t varList[VARLIST_LEN];//符号表

static char cmdArgs[ARG_NUM][ARG_CHARLEN];
//以符号名、符号类型、符号值或该值的指针，在符号表中创建符号
void var_create(char *str,EVarType *type, void *ptr)
{
    int i = 0;
    printf("var_create %s %d %p", str, *((int*)type), ptr);
    while(varList[i].name[0] != 0)
    {
        if(i >= VARLIST_LEN)
        {
            if(g_debug_on)
                printf("Error! varList is full!\n");
            return ;
        }
        i++;
    }
    strcpy(varList[i].name, str);
    varList[i].type = *((char*)type);//varList[i].type = *((char*)type) - '0';
    //printf("type:%d\n",(int)varList[i].type);
    switch(varList[i].type)
    {
    case VarType_char:
        printf("type 0:%d\n",VarType_char);
        varList[i].pvar = ptr;//参数为某个数字值的void*类型强制转换，直接传入值节省内存
        break;
    case VarType_uchar:
        printf("type 1:%d\n",VarType_uchar);
        varList[i].pvar = ptr;//参数为某个数字值的void*类型强制转换，直接传入值节省内存
        break;
    case VarType_pchar:
        printf("type 2:%d\n",VarType_pchar);
        varList[i].pvar = ptr;//参数为某个数字值的void*类型强制转换，直接传入值节省内存
        break;
    case VarType_int:
        printf("type 3:%d\n",VarType_int);
        varList[i].pvar = ptr;//参数为某个数字值的void*类型强制转换，直接传入值节省内存
        break;
    case VarType_uint:
        printf("type 4:%d\n",VarType_uint);
        varList[i].pvar = (void*)atoi(ptr);//参数为某个数字值的void*类型强制转换，直接传入值节省内存
        printf("int:%d\n",(int)varList[i].pvar);
        break;
    case VarType_pvoid:
        printf("type 6:%d\n",VarType_pvoid);
        varList[i].pvar = ptr;
        break;
    case VarType_branch:
        printf("type 8:%d\n",VarType_branch);
        varList[i].pvar = ptr;
        printf("branch=%p\n",varList[i].pvar);
        break;
    default:
        printf("type X:%d\n",varList[i].type);
        varList[i].pvar = (*(void**)ptr);//某块内存区域的指针
        break;
    }
}

//显示所有符号
void var_show_all()
{
    int i;
    if(g_debug_on)
        printf("fn var_show_all start\n");
    for (i = 0; i < VARLIST_LEN; i++)
    {
        if (varList[i].name[0] != 0)
        {
            printf("[%s] ",varList[i].name);
            switch(varList[i].type)
            {
            case VarType_char:
                printf("char:%c\n", ((char)(varList[i].pvar)));
                break;
            case VarType_pchar:
                printf("pchar:%s\n", ((char*)(varList[i].pvar)));
                break;
            case VarType_int:
                printf("int:%d %p\n", *((int*)(varList[i].pvar)), varList[i].pvar);
                break;
            case VarType_pvoid:
                printf("pvoid:%p\n", varList[i].pvar);
                break;
            default:
                printf("Type(%d):%p\n", varList[i].type, varList[i].pvar);
                break;
            }
        }
    }
}

void* var_get(char *varName)
{
    int i;
    if(g_debug_on)
        printf("fn var_get\n");
    for (i = 0; i < VARLIST_LEN; i++)
    {
        if (!strcmp(varList[i].name, varName))
        {
            printf("find [%s] ",varList[i].name);
            switch(varList[i].type)
            {
            case VarType_char:
                printf("char:%c\n", ((char)(varList[i].pvar)));
                return ((char)(varList[i].pvar));
                break;
            case VarType_pchar:
                printf("pchar:%s\n", ((char*)(varList[i].pvar)));
                return ((char*)(varList[i].pvar));
                break;
            case VarType_int:
                printf("int:%d\n", *((int*)(varList[i].pvar)));
                return varList[i].pvar;//return *((int*)varList[i].pvar);
                break;
            case VarType_pvoid:
                printf("pvoid:%p\n", varList[i].pvar);
                return varList[i].pvar;
                break;
            case VarType_branch:
                printf("branch:%p\n", varList[i].pvar);
                return varList[i].pvar;
                break;
            default:
                printf("Type(%d):%p\n", varList[i].type, varList[i].pvar);
                return varList[i].pvar;
                break;
            }
        }
    }
    return 0;
}

void* var_set(char *varName, void *pvar)
{
    int i;
    if(g_debug_on)
        printf("fn var_set\n");
    for (i = 0; i < VARLIST_LEN; i++)
    {
        if (!strcmp(varList[i].name, varName))
        {
            printf("set [%s]=%p", varList[i].name, pvar);
            switch(varList[i].type)
            {
            case VarType_char:
                printf("char:%c\n", ((char)(varList[i].pvar)));
                return ((char)(varList[i].pvar));
                break;
            case VarType_pchar:
                printf("pchar:%s\n", ((char*)(varList[i].pvar)));
                return ((char*)(varList[i].pvar));
                break;
            case VarType_int:
                printf("int:%d\n", *((int*)(pvar)));
                *((int*)(varList[i].pvar)) = *((int*)(pvar));
                return varList[i].pvar;//return *((int*)varList[i].pvar);
                break;
            case VarType_pvoid:
                printf("pvoid:%p\n", varList[i].pvar);
                return varList[i].pvar;
                break;
            case VarType_branch:
                printf("branch:%p\n", varList[i].pvar);
                return varList[i].pvar;
                break;
            default:
                printf("Type(%d):%p\n", varList[i].type, varList[i].pvar);
                return varList[i].pvar;
                break;
            }
        }
    }
    return 0;
}
//内存空间分配
void* ML_malloc(void *ptr, void *len)
{
    printf("fn malloc(%d) start\n",*((int*)len));
    *(void **)ptr = malloc(*((int*)len));
    return ptr;
}

void* a2i(void *str, void *ret)
{
    if(ret)
    {
        *(int *)ret = (int)atoi((char *)str);
        if(g_debug_on)
            printf("atoi(%s)=%d\n",(char *)str,*(int *)ret);
        return ret;
    }
    else
        return (void*)atoi(str);
}

char* getFirstArg(char *dest, char *src)
{
    int i = 0;
    while (*src == ' ')
                src++;
    while ( (*src != ' ') && (*src != 0) )
    {
        dest[i++] = *src++;
    }
    dest[i] = 0;
    //if(g_debug_on)
    //    printf("[getFirstArg:%s]",dest);
    return src;
}

void getAllArgs(char cmdArgs[][ARG_CHARLEN],char *pch)
{
    int i = 0;
    do
    {
        pch = getFirstArg(cmdArgs[i], pch);
        printf("cmdArgs %d:%s\n", i, cmdArgs[i]);
        if(i == ARG_NUM)
        {
            printf("Args are too much,the max num is %d.\n",ARG_NUM);
            break;
        }
        i++;
    }while(*pch != 0);
}
void* add2branchWL(char **pch)
{
    MLFn pFn;
    EMLFnType fnType;
    int i;
    char tmpcmd[ARG_CHARLEN];
    struct list_head *rootfun = (struct list_head *)malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(rootfun);
    MLFnNode_tp pMLFN;
    void *tmp;
NEXT_BLOCK:
    *pch = getFirstArg(tmpcmd, *pch);
    if(!strcmp(tmpcmd,")"))
        goto END;
    //暂不支持树枝返回值为函数名，假定第一字符串是函数名
    pFn = getFnByName(tmpcmd, &fnType);
    pMLFN = (MLFnNode_tp)malloc(sizeof(MLFnNode_t));
    pMLFN->fn = pFn;
    pMLFN->type = fnType;
    pMLFN->arg = malloc(fnType*sizeof(void*));
    ///pMLFN->argFlag = argFlag;
    pMLFN->argFlag = 0;//0b1;
    //printf("fnType=%d\n",fnType);
    for(i = 0; i <= fnType /*&& *pch!=0*/;)
    {
        //printf("i=%d\n",i);
        *pch = getFirstArg(tmpcmd, *pch);
        if(!strcmp(tmpcmd,"("))//下一层
        {
            space(g_spaces);printf("(\n");
            pMLFN->argFlag |= 0b10 << i;
            g_spaces++;
            pMLFN->arg[i] = add2branchWL(pch);
            i++;
            continue;
        }
        else if(!strcmp(tmpcmd,"`("))//下一层
        {///引用
            space(g_spaces);printf("`(\n");
            pMLFN->argFlag &= ((~0b10) << i);///pMLFN->argFlag |= 0b10 << i;
            g_spaces++;
            pMLFN->arg[i] = add2branchWL(pch);
            space(g_spaces);printf("引用为%p\n",pMLFN->arg[i]);
            i++;
            continue;
        }
        else if((!strcmp(tmpcmd,"`,(")) || (!strcmp(tmpcmd,",`(")) )
        {///执行引用
            space(g_spaces);printf("``(\n");
            pMLFN->argFlag &= ((~0b10) << i);///pMLFN->argFlag |= 0b10 << i;
            g_spaces++;
            tmp = add2branchWL(pch);
            //pMLFN->arg[i] = (int*)malloc(sizeof(int));
            pMLFN->arg[i] = MLEngineRun(tmp);//*(int *)(pMLFN->arg[i]) = MLEngineRun(tmp);
            space(g_spaces);printf("=%d\n",*((int*)(pMLFN->arg[i])));
            i++;
            continue;
        }
        else if(!strcmp(tmpcmd,",("))//下一层
        {///执行
            space(g_spaces);printf("``(\n");
            pMLFN->argFlag |= 0b10 << i;
            g_spaces++;
            tmp = add2branchWL(pch);
            //pMLFN->arg[i] = (int*)malloc(sizeof(int));
            pMLFN->arg[i] = MLEngineRun(tmp);//*(int *)(pMLFN->arg[i]) = MLEngineRun(tmp);
            space(g_spaces);printf("=%d\n",*((int*)(pMLFN->arg[i])));
            i++;
            continue;
        }
        else if(!strcmp(tmpcmd,")"))//当前函数pFn的右括号
        {
            g_spaces--;
            space(g_spaces);printf(")\n");
            do
            {
                pMLFN->arg[i] = 0;
                i++;
            }while(i < fnType);
            goto ADD_FUN;//break;//跳出For循环
        }
        else if(0 == *tmpcmd)
        {
            g_spaces--;
            goto END;//break;
        }
        else if((tmpcmd[0] >= '0')&&(tmpcmd[0] <= '9'))
        {
            pMLFN->arg[i] = (int*)malloc(sizeof(int));
            *(int *)(pMLFN->arg[i]) = strtol(tmpcmd, 0, 0);//*(int *)(pMLFN->arg[i]) = atoi(tmpcmd);
            space(g_spaces);printf("%p %d\n",pMLFN->arg[i], *(int*)pMLFN->arg[i]);
            i++;
            continue;
        }
        else
        {//当作字符串
            pMLFN->arg[i] = (int*)malloc(strlen(tmpcmd));
            strcpy(pMLFN->arg[i], tmpcmd);
            space(g_spaces);printf("%p %s\n",pMLFN->arg[i], (char*)pMLFN->arg[i]);
            i++;
            continue;
        }
    }
ADD_FUN:
    list_add_tail(&(pMLFN->list),rootfun);
    //MLEngineRun(rootfun);
END:
    //free(rootfun);
    return rootfun;
}
void* add2rootWL(char **pch)
{
    MLFn pFn;
    EMLFnType fnType;
    int i;
    char tmpcmd[ARG_CHARLEN];
    struct list_head *rootfun = (struct list_head *)malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(rootfun);
    MLFnNode_tp pMLFN;
    void *tmp;
NEXT_BLOCK:
    printf("(\n");
    g_spaces = 1;
    *pch = getFirstArg(tmpcmd, *pch);
    if(!strcmp(tmpcmd,")"))
        goto END;
    //暂不支持树枝返回值为函数名，假定第一字符串是函数名
    pFn = getFnByName(tmpcmd, &fnType);
    pMLFN = (MLFnNode_tp)malloc(sizeof(MLFnNode_t));
    pMLFN->fn = pFn;
    pMLFN->type = fnType;
    pMLFN->arg = malloc(fnType*sizeof(void*));
    ///pMLFN->argFlag = argFlag;
    pMLFN->argFlag = 0;//0b1;
    //printf("fnType=%d\n",fnType);
    for(i = 0; i <= fnType /*&& *pch!=0*/;)
    {
        //printf("i=%d\n",i);
        *pch = getFirstArg(tmpcmd, *pch);
        if(!strcmp(tmpcmd,"("))//下一层
        {
            space(g_spaces);printf("(\n");
            pMLFN->argFlag |= 0b10 << i;
            g_spaces++;
            pMLFN->arg[i] = add2branchWL(pch);//add2rootWL(pch);//pMLFN->arg[i] = add2branchWL(*pch,rootfun);
            i++;
            continue;
        }
        else if(!strcmp(tmpcmd,"`("))//下一层
        {///引用
            space(g_spaces);printf("`(\n");
            pMLFN->argFlag &= ((~0b10) << i);///pMLFN->argFlag |= 0b10 << i;
            g_spaces++;
            pMLFN->arg[i] = add2branchWL(pch);
            space(g_spaces);printf("引用为%p\n",pMLFN->arg[i]);
            //MLEngineRun(pMLFN->arg[i]);
            //printf("引用 TEST finished\n",pMLFN->arg[i]);
            i++;
            continue;
        }
        else if((!strcmp(tmpcmd,"`,(")) || (!strcmp(tmpcmd,",`(")) )//下一层
        {///执行引用
            space(g_spaces);printf("`,(\n");
            pMLFN->argFlag &= ((~0b10) << i);///pMLFN->argFlag |= 0b10 << i;
            g_spaces++;
            tmp = add2branchWL(pch);
            //pMLFN->arg[i] = (int*)malloc(sizeof(int));
            pMLFN->arg[i] = MLEngineRun(tmp);//*(int *)(pMLFN->arg[i]) = MLEngineRun(tmp);
            space(g_spaces);printf("=%d\n",*((int*)(pMLFN->arg[i])));
            i++;
            continue;
        }
        else if(!strcmp(tmpcmd,",("))//下一层
        {///执行
            space(g_spaces);printf("``(\n");
            pMLFN->argFlag |= 0b10 << i;
            g_spaces++;
            tmp = add2branchWL(pch);
            //pMLFN->arg[i] = (int*)malloc(sizeof(int));
            pMLFN->arg[i] = MLEngineRun(tmp);//*(int *)(pMLFN->arg[i]) = MLEngineRun(tmp);
            space(g_spaces);printf("=%d\n",*((int*)(pMLFN->arg[i])));
            i++;
            continue;
        }
        else if(!strcmp(tmpcmd,")"))//当前函数pFn的右括号
        {
            g_spaces--;
            space(g_spaces);printf(")\n");
            do
            {
                pMLFN->arg[i] = 0;
                i++;
            }while(i < fnType);
            goto ADD_FUN;//break;//跳出For循环
        }
        else if(0 == *tmpcmd)
        {
            g_spaces--;
            goto END;//break;
        }
        else if((tmpcmd[0] >= '0')&&(tmpcmd[0] <= '9'))
        {
            pMLFN->arg[i] = (int*)malloc(sizeof(int));
            *(int *)(pMLFN->arg[i]) = strtol(tmpcmd, 0, 0);//*(int *)(pMLFN->arg[i]) = atoi(tmpcmd);
            space(g_spaces);printf("%p %d\n",pMLFN->arg[i], *(int*)pMLFN->arg[i]);
            i++;
            continue;
        }
        else
        {//当作字符串
            pMLFN->arg[i] = (int*)malloc(strlen(tmpcmd));
            strcpy(pMLFN->arg[i], tmpcmd);
            space(g_spaces);printf("%p %s\n",pMLFN->arg[i], (char*)pMLFN->arg[i]);
            i++;
            continue;
        }
    }
ADD_FUN:
    list_add_tail(&(pMLFN->list),rootfun);
//*
    if (*pch != 0)
    {
        *pch = getFirstArg(tmpcmd, *pch);
        if( (!strcmp(tmpcmd,"(")) || (!strcmp(tmpcmd,",(")) || (!strcmp(tmpcmd,"`,(")) || (!strcmp(tmpcmd,",`(")) )
        {
            goto NEXT_BLOCK;
        }
        if (*pch == 0)
            if (g_debug_on)
            {
                space(g_spaces);printf("<cmd end>");
            }
    }  //*/
    //MLEngineRun(rootfun);
END:
    //free(rootfun);
    return rootfun;
}




//函数名
char *str_var = "var";
char *str_malloc = "malloc";
char *str_var_show_all = "var_show_all";
char *str_plus = "+";
char *str_minus = "-";
char *str_multiply = "*";
char *str_divide = "/";
char *str_BC_plus = "BC_plus";
char *str_atoi = "atoi";
char *str_var_get = "varg";
char *str_var_set = "vars";
void MLLib_init()
{
    //将函数加入函数树
    MLFnTree_addFn(&str_var, (MLFn)var_create, MLFnT3);
    MLFnTree_addFn(&str_malloc, (MLFn)ML_malloc, MLFnT2);
    MLFnTree_addFn(&str_var_show_all, (MLFn)var_show_all, MLFnT0);
    MLFnTree_addFn(&str_plus, (MLFn)BC_plus, MLFnT3);
    MLFnTree_addFn(&str_minus, (MLFn)BC_minus, MLFnT3);
    MLFnTree_addFn(&str_multiply, (MLFn)BC_multiply, MLFnT3);
    MLFnTree_addFn(&str_divide, (MLFn)BC_divide, MLFnT3);
    MLFnTree_addFn(&str_BC_plus, (MLFn)BC_plus, MLFnT3);
    MLFnTree_addFn(&str_atoi, (MLFn)a2i, MLFnT2);
    MLFnTree_addFn(&str_var_get, (MLFn)var_get, MLFnT1);
    MLFnTree_addFn(&str_var_set, (MLFn)var_set, MLFnT2);
}




