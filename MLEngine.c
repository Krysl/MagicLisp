#include "MLEngine.h"
#include "LinuxList.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include "MLDebug.h"
int g_suspend = false;
int g_stop = false;
LIST_HEAD(MLWorkList);//全局工作链
LIST_HEAD(MLFnTree);  //全局函数树
void *g_arg[9];
int MLWLAddFn(MLFn fn, EMLFnType type, void **arg, int argFlag)//链接新函数到全局工作链
{
    int ret = 0;
    MLFnNode_tp pMLFN = (MLFnNode_tp)malloc(sizeof(MLFnNode_t));
    pMLFN->fn = fn;
    pMLFN->type = type;
    pMLFN->arg = arg;
    pMLFN->argFlag = argFlag;
    MLWL_add_tail(pMLFN);//list_add_tail(&(pmlfn->list),&MLWorkList);
    return ret;
}
int MLFLAddFn(MLFn fn, EMLFnType type, void **arg, int argFlag, struct list_head *pFL)//链接新函数到全局工作链
{
    int ret = 0;
    MLFnNode_tp pMLFN = (MLFnNode_tp)malloc(sizeof(MLFnNode_t));
    pMLFN->fn = fn;
    pMLFN->type = type;
    pMLFN->arg = arg;
    pMLFN->argFlag = argFlag;
    list_add_tail(&(pMLFN->list),pFL);
    return ret;
}
#if 0
#define getArgValue(pFn,n) \
(pFn->argFlag & (0x2 << (n))) ? MLEngineRun((struct list_head *)(pFn->arg[n])) : pFn->arg[n]
#else
inline void* getArgValue(MLFnNode_tp pFn, int n, void **args)
{
    if(pFn->argFlag & (0x2 << (n)))
    {
        args[n] = MLEngineRun((struct list_head *)(pFn->arg[n]));
        return &args[n];
    }
    else
    {
        //printf("<%p>",pFn->arg[n]);
        if(pFn->arg[n])
            printf(" %d", *(int*)pFn->arg[n]);
        return pFn->arg[n];
    }

}
#endif

void* MLEngineRun(struct list_head *pMLWorkList)//执行全局工作链   //递归用得太厉害了，什么时候研究尾递归什么的
{
    MLFnNode_tp pFn;
    void *ret,*args[9];
    MLFn pfun = NULL;
    struct list_head *i = pMLWorkList->next;
    if(g_debug_on)
        printf(" Run[%p %x]{",pMLWorkList,*pMLWorkList);
    while(1)
    {
        //if(list_empty(pMLWorkList) == false)
        if(i != pMLWorkList)
        {
            pFn = list_entry(i/*pMLWorkList->next*/, MLFnNode_t, list);
            if(pFn->argFlag & 0x1)//函数指针位置，保存的是树枝
            {
                pfun = MLEngineRun((struct list_head *)(pFn->fn));
            }
            else
            {
                pfun = pFn->fn;
                if(g_debug_on)
                    printf("%s",(char*)getNameByFn(pfun));
            }
            switch(pFn->type)
            {
            case MLFnT0:
                ret = (*((MLFn0)(pfun)))();
                break;
            case MLFnT1:
                //(*((MLFn1)(pfun)))(pFn->arg[0]);
                ret = (*((MLFn1)(pfun)))(getArgValue(pFn, 0, args));
                break;
            case MLFnT2:
                ret = (*((MLFn2)(pfun)))(getArgValue(pFn, 0, args), getArgValue(pFn, 1, args));
                break;
            case MLFnT3:
                ret = (*((MLFn3)(pfun)))(getArgValue(pFn, 0, args), getArgValue(pFn, 1, args), getArgValue(pFn, 2, args));
                break;
            default:
                ;
                break;
            }
            ///pMLWorkList->next = pMLWorkList->next->next;
            ///pMLWorkList->next->prev = pMLWorkList;
            ///free(pFn);
            i = i->next;
        }
        else
            goto END;
        if(g_stop)
            goto END;
        while(g_suspend)
            printf(".\b");
    }
END:
    if(g_debug_on)
#if 0
        printf(" =%d}",*(int*)ret);
#else
        printf(" =%d}\n",ret);
#endif
    return ret;
}

void MLFnTree_addFn(char **name, MLFn fn, EMLFnType type)//添加新函数到全局函数树
{
    MLFnTreeNode_tp pmlftn= (MLFnTreeNode_tp)malloc(sizeof(MLFnTreeNode_t));
    pmlftn->fn = fn;
    pmlftn->fnName = *name;
    pmlftn->type = type;
    list_add_tail(&(pmlftn->list),&MLFnTree);
}

void *getFnByName(char* name, EMLFnType *type)//以函数名在全局函数树中查找并获取函数
{
    struct list_head *i;
    MLFnTreeNode_tp pFn;
    if (g_debug_on)
    {
        space(g_spaces);
        printf("<%s>\n",name);//printf("getFnByName(%s)\n",name);
    }
    list_for_each(i, &MLFnTree)
    {
        pFn = list_entry(i, MLFnTreeNode_t, list);
        if(!strcmp(pFn->fnName,name))
            goto FIND_fn;
    }
    return NULL;
FIND_fn:
    *type = pFn->type;
    return (void*)(pFn->fn);
}
void *getNameByFn(MLFn fn)//以函数名在全局函数树中查找并获取函数
{
    struct list_head *i;
    MLFnTreeNode_tp pFn;
    list_for_each(i, &MLFnTree)
    {
        pFn = list_entry(i, MLFnTreeNode_t, list);
        if(pFn->fn == fn)
            goto FIND_Name;
    }
    return NULL;
FIND_Name:
    //if (g_debug_on)
     //   printf("getNameByFn(%s)\n",pFn->fnName);
    return (void*)(pFn->fnName);
}
void* eval(struct list_head **pMLWorkList)
{
    return MLEngineRun(*pMLWorkList);
}
char *str_getFnByName = "getFnByName";
char *str_eval = "eval";
void MLEngine_init()
{

    MLFnTree_addFn(&str_getFnByName, (MLFn)getFnByName, MLFnT2);
    MLFnTree_addFn(&str_eval, (MLFn)eval, MLFnT1);
}
