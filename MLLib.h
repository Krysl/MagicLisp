#ifndef MLLIB_H_INCLUDED
#define MLLIB_H_INCLUDED
#include "MLEngine.h"

#define VARLIST_LEN 100

#define ARG_NUM 20 //一次命令中参数的个数
#define ARG_CHARLEN 20 //参数的最大字符数

typedef enum VarType
{
    VarType_char,
    VarType_uchar,
    VarType_pchar,
    VarType_int,
    VarType_uint,
    VarType_pint,
    VarType_pvoid,
    VarType_fun,//º¯ÊýÁ´ struct list_head*
    VarType_branch
}EVarType;
typedef struct VarNode
{
    char name[20];
    EVarType type;
    void *pvar;
}VarNode_t, *VarNode_tp;

void var_create(char *str,EVarType *type, void *ptr);
void var_show_all();
void* ML_malloc(void *ptr, void *len);
void* a2i(void *str, void *ret);
void MLLib_init();
char* getFirstArg(char *dest, char *src);
void getAllArgs(char cmdArgs[][ARG_CHARLEN],char *pch);
void* add2rootWL(char **pch);
int add2rootWL2(char **pch);
#endif // MLLIB_H_INCLUDED
