#include "cli.h"

#include <assert.h>
#include <string.h>

static operand_stack_t operands_;
static operation_queue_t operations_;

void reset_(void);
const double* pop_(void);
const double* push_(double n);
void reverse_(void);
const operand_stack_t* get_operand_stack(void)
{
    static int initialized;
    if (!initialized)
    {
        initialized = 1;
        operands_.reset = reset_;
        operands_.pop = pop_;
        operands_.push = push_;
        operands_.reverse = reverse_;
    }
    return &operands_;
}


void reset_2(void);
const operation_t* pop_2(void);
const operation_t* push_2(const operation_t* op);
const operation_queue_t* get_operations_stack(void)
{
    static int initialized;
    if (!initialized)
    {
        initialized = 1;
        operations_.reset = reset_2;
        operations_.pop = pop_2;
        operations_.push = push_2;
    }
    return &operations_;
}


// OPERANDS
void reset_(void)
{
    operands_.size = 0;
}
const double* pop_(void)
{
    if (operands_.size == 0)
        return NULL;

    const double* n = &operands_.operands[operands_.size - 1];
    operands_.size--;
    return n;
}
const double* push_(double n)
{
    if (operands_.size >= MAX_OPERANDS -1)
        return NULL;

    operands_.operands[operands_.size] = n;
    operands_.size++;
    return &operands_.operands[operands_.size - 1];
}
void reverse_(void)
{
    for (unsigned int i = 0; i < (operands_.size / 2); ++i)
    {
        const unsigned int nextLast = (operands_.size - 1) - i;
        double tmp = operands_.operands[i];
        operands_.operands[i] = operands_.operands[nextLast];
        operands_.operands[nextLast] = tmp;
    }
}


// OPERATIONS
void reset_2(void)
{
    operations_.size = 0;
}
const operation_t* pop_2(void)
{
    if (operations_.size == 0)
        return NULL;

    const operation_t* op = operations_.operations[0];
    operations_.size--;
    memmove(operations_.operations,
            operations_.operations + 1,
            operations_.size * sizeof(*operations_.operations)
    );
    return op;
}
const operation_t* push_2(const operation_t* op)
{
    if (op == NULL || operations_.size >= MAX_OPERATIONS - 1)
        return NULL;

    operations_.operations[operations_.size] = op;
    operations_.size++;
    return op;
}
