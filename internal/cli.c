#include "cli.h"

#include <assert.h>
#include <string.h>

static operand_stack_t operands_;
static operation_stack_t operations_;

static void reset_(void);
static const double* pop_(void);
static const double* push_(double n);
const operand_stack_t* get_operand_stack(void)
{
    static int initialized;
    if (!initialized)
    {
        initialized = 1;
        operands_.reset = reset_;
        operands_.pop = pop_;
        operands_.push = push_;
    }
    return &operands_;
}


static void reset_2(void);
static const operation_t* pop_2(void);
static const operation_t* push_2(const operation_t* op);
const operation_stack_t* get_operations_stack(void)
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
static void reset_(void)
{
    operands_.size = 0;
}
static const double* pop_(void)
{
    if (operands_.size == 0)
        return NULL;

    const double* n = &operands_.operands[operands_.size - 1];
    operands_.size--;
    return n;
}
static const double* push_(double n)
{
    if (operands_.size >= MAX_OPERANDS -1)
        return NULL;

    operands_.operands[operands_.size] = n;
    operands_.size++;
    return &operands_.operands[operands_.size - 1];
}


// OPERATIONS
static void reset_2(void)
{
    operations_.size = 0;
}
static const operation_t* pop_2(void)
{
    if (operations_.size == 0)
        return NULL;

    const operation_t* op = operations_.operations[operations_.size - 1];
    operations_.size--;
    return op;
}
static const operation_t* push_2(const operation_t* op)
{
    if (op == NULL || operations_.size >= MAX_OPERATIONS - 1)
        return NULL;

    operations_.operations[operations_.size] = op;
    operations_.size++;
    return op;
}
