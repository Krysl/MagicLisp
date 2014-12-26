#ifndef MLDEBUG_H_INCLUDED
#define MLDEBUG_H_INCLUDED


int g_debug_on;
int g_spaces;

#define space(x) \
do\
{\
    int i;\
    for (i = 0; i < x; i++)\
        printf("____");\
}while(0);

#endif // MLDEBUG_H_INCLUDED
