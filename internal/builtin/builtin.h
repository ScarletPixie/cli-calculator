#ifndef BUILTIN_H
#define BUILTIN_H

#include "api.h"

#define RPN_X_MAX_BUILTIN 128

typedef struct
{
    unsigned long size;
    const operation_t* builtins[RPN_X_MAX_BUILTIN];
}   builtin_operations_t;

const builtin_operations_t* get_builtins(void);

#endif
