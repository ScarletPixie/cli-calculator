#ifndef CLI_H
#define CLI_H

#include "api.h"

#ifndef MAX_OPERANDS
    #define MAX_OPERANDS 255
#endif

#ifndef MAX_OPERATIONS
    #define MAX_OPERATIONS 255
#endif

typedef struct
{
    unsigned int size;
    double operands[MAX_OPERANDS];

    void (*reset)(void);
    const double* (*pop)(void);
    const double* (*push)(double n);
    void (*reverse)(void);
}   operand_stack_t;
const operand_stack_t* get_operand_stack(void);

typedef struct
{
    unsigned int size;
    const operation_t* operations[MAX_OPERATIONS];

    void (*reset)(void);
    const operation_t* (*pop)(void);
    const operation_t* (*push)(const operation_t* op);
}   operation_queue_t;
const operation_queue_t* get_operations_stack(void);


#endif
