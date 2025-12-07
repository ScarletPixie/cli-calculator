#ifndef INTERNAL_H
#define INTERNAL_H

#include "api.h"

#ifndef MAX_OPS
    #define MAX_OPS 255
#endif

typedef struct
{
    unsigned int loaded_c;
    operation_t opers[MAX_OPS];
}   operation_names_t;

const operation_names_t* loaded_operations(void);

const operation_t* get_op(const char* name);
const operation_t* add_op(const operation_t op);

#endif
