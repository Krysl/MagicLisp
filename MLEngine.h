#ifndef MLENGINE_H_INCLUDED
#define MLENGINE_H_INCLUDED

#include "LinuxList.h"

#ifndef false
typedef enum __bool { false = 0, true = 1 } bool;
#endif
typedef enum MLFnType
{
    MLFnT0,
    MLFnT1,
    MLFnT2,
    MLFnT3,
    MLFnT4,
    MLFnT5,
    MLFnT6,
    MLFnT7,
    MLFnT8,
//    MLFnT9,
    MLFnTFnList,
    MLFnTVar,//表示值为变量
    MLFnT_char,
    MLFnT_uchar,
    MLFnT_pchar,
    MLFnT_int,
    MLFnT_uint,
    MLFnT_pint,
    MLFnT_pvoid
}EMLFnType;
typedef void* (*MLFn0)(void);
typedef void* (*MLFn1)(void*);
typedef void* (*MLFn2)(void*, void*);
typedef void* (*MLFn3)(void*, void*, void*);
typedef void* (*MLFn4)(void*, void*, void*, void*);

typedef MLFn1 MLFn;typedef struct MLFnNode
{
    struct list_head list;
    MLFn fn;
    EMLFnType type;
    void **arg;
    int argFlag;//位标识，1位标识一个参数，由低到高分别表示函数名、1~8个参数（9/32）。
                //0：变量（直接可用）、1：函数结点/链（需要求值）
}MLFnNode_t,*MLFnNode_tp;
#define MLWL_add_tail(pmlfn) \
    list_add_tail(&(pmlfn->list),&MLWorkList)
typedef struct MLFnTreeNode
{
    struct list_head list;
    ///struct list_head child_head;
    char *fnName;
    MLFn fn;
    EMLFnType type;
}MLFnTreeNode_t,*MLFnTreeNode_tp;
void MLEngine_init();
int MLWLAddFn(MLFn fn, EMLFnType type, void **arg, int argFlag);
int MLFLAddFn(MLFn fn, EMLFnType type, void **arg, int argFlag, struct list_head *pFL);//链接新函数到全局工作链
void* MLEngineRun(struct list_head *pMLWorkList);
void MLFnTree_addFn(char **name, MLFn fn, EMLFnType type);
void *getFnByName(char* name, EMLFnType *type);
void *getNameByFn(MLFn fn);
#endif // MLENGINE_H_INCLUDED
