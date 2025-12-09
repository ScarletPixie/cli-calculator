#ifndef INTERNAL_H
#define INTERNAL_H

#include "api.h"

#ifndef MAX_OPS
    #define MAX_OPS 255
#endif

typedef struct
{
    unsigned int loaded_c;
    void* handles[MAX_OPS];
    operation_t opers[MAX_OPS];
}   operation_names_t;

void load_plugins(void);
void unload_plugins(void);

const operation_t* get_op(const char* name);
const operation_t* add_op(const operation_t op);

#endif
