#include "stdio.h"
#include "MLDebug.h"
extern int g_debug_on;
void* BC_plus(int *a, int *b, int *ret)
{
    if (ret)
    {
        if(g_debug_on)
            printf("[BC_plus 1:%p+%p]", a, b);
        *ret = *a+*b;
        if(g_debug_on)
            printf("[BC_plus:%d+%d=%d]", *a, *b, *ret);
        return (void*)*ret;//返回值还是应该返回值的指针？须考虑如何改进
    }
    else
    {
        if(g_debug_on)
            printf("[BC_plus 2:%p+%p]", a, b);
        if(g_debug_on)
            printf("[BC_plus:%d+%d=%d]", *a, *b, *a+*b);
        return (void*)(*a+*b);
    }

}
void* BC_minus(int *a, int *b, int *ret)
{
    if (ret)
    {
        if(g_debug_on)
            printf("[BC_minus 1:%p-%p]", a, b);
        *ret = (*a)-(*b);
        if(g_debug_on)
            printf("[BC_minus:%d-%d=%d]", *a, *b, *ret);
        return (void*)*ret;//返回值还是应该返回值的指针？须考虑如何改进
    }
    else
    {
        if(g_debug_on)
            printf("[BC_minus 2:%p-%p]", a, b);
        if(g_debug_on)
            printf("[BC_minus:%d-%d=%d]", *a, *b, (*a)-(*b));
        return (void*)((*a)-(*b));
    }

}
void* BC_multiply(int *a, int *b, int *ret)
{
    if (ret)
    {
        if(g_debug_on)
            printf("[BC_multiply 1:%p*%p]", a, b);
        *ret = (*a) * (*b);
        if(g_debug_on)
            printf("[BC_multiply:%d*%d=%d]", *a, *b, *ret);
        return (void*)*ret;//返回值还是应该返回值的指针？须考虑如何改进
    }
    else
    {
        if(g_debug_on)
            printf("[BC_multiply 2:%p*%p]", a, b);
        if(g_debug_on)
            printf("[BC_multiply:%d*%d=%d]", *a, *b, (*a) * (*b));
        return (void*)((*a) * (*b));
    }

}
void* BC_divide(int *a, int *b, int *ret)
{
    if (ret)
    {
        if(g_debug_on)
            printf("[BC_divide 1:%p/%p]", a, b);
        *ret = *a / *b;
        if(g_debug_on)
            printf("[BC_divide:%d/%d=%d]", *a, *b, *ret);
        return (void*)*ret;//返回值还是应该返回值的指针？须考虑如何改进
    }
    else
    {
        if(g_debug_on)
            printf("[BC_divide 2:%p/%p]", a, b);
        if(g_debug_on)
            printf("[BC_divide:%d/%d=%d]", *a, *b, *a / *b);
        return (void*)(*a / *b);
    }

}
