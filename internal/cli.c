#include "cli.h"
#include "api.h"
#include "operation_loader.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MAX_OPERANDS
    #define MAX_OPERANDS 1024
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

static operand_stack_t operands_;
static const operand_stack_t* get_operand_stack(void);


static run_status parse_error(run_status st, const char *fmt, ...);
static run_status parse_op(const operand_stack_t* operands, const operation_t* op, const char* token);
run_status run(int argc, char** argv)
{
    assert(argc > 0);
    if (argc > MAX_OPERANDS)
        return parse_error(RUN_TOO_MANY_OPERANDS, "too many operands, max is %d\n", MAX_OPERANDS);

    const operand_stack_t* operands = get_operand_stack();
    for (int i = 1; i < argc; ++i)
    {
        char* tokenEnd;
        double value = strtod(argv[i], &tokenEnd);
        if (*tokenEnd == argv[i][0])
        {
            const operation_t* op = get_op(argv[i]);
            run_status st = parse_op(operands, op, argv[i]);
            if (st != RUN_SUCESS)
                return st;
        }
        else if (*tokenEnd == '\0')
            operands->push(value);
        else
            return parse_error(RUN_INVALID_TOKEN, "invalid token: '%s'\n", argv[i]);
    }
    if (operands->size != 1)
        return parse_error(RUN_INVALID_EXPRESSION, NULL);

    printf("%g\n", *operands->pop());
    return RUN_SUCESS;
}

static run_status parse_op(const operand_stack_t* operands, const operation_t* op, const char* token)
{
    if (operands->size == 0)
        return parse_error(RUN_NOT_ENOUGH_OPERANDS, "not enough operands for: '%s'\n", token);
    else if (!op)
        return parse_error(RUN_UNKNOWN_OPERATION, "unkown operation: '%s'\n", token);
    else if (!op->unary)
        return parse_error(RUN_NOT_IMPLEMENTED, "operation not implemented: '%s'\n", token);


    if (op->type == UNARY)
    {
        const double* v = operands->pop();
        if (v == NULL)
            return parse_error(RUN_NOT_ENOUGH_OPERANDS, "not enough operands for: '%s'\n", token);
        operands->push(op->unary(*v));
    }
    else if (op->type == BINARY)
    {
        const double* rhs = operands->pop();
        const double* lhs = operands->pop();
        if (lhs == NULL || rhs == NULL)
            return parse_error(RUN_NOT_ENOUGH_OPERANDS, "not enough operands for: '%s'\n", token);
        operands->push(op->binary(*lhs, *rhs));
    }
    else if (op->type == TERTIARY)
    {
        const double* rhs = operands->pop();
        const double* mid = operands->pop();
        const double* lhs = operands->pop();
        if (lhs == NULL || mid == NULL || rhs == NULL)
            return parse_error(RUN_NOT_ENOUGH_OPERANDS, "not enough operands for: '%s'\n", token);
        operands->push(op->tertiary(*lhs, *mid, *rhs));
    }
    else if (op->type == N_ARY)
    {
        operands->reverse();
        double val = op->n_ary(operands->size, operands->operands);
        operands->reset();
        operands->push(val);
    }

    return RUN_SUCESS;
}
static run_status parse_error(run_status st, const char *fmt, ...)
{
    if (st == RUN_SUCESS)
        return st;

    va_list va;
    va_start(va, fmt);

    if (st == RUN_INVALID_EXPRESSION)
        vfprintf(stderr, fmt ? fmt : "invalid expression\n", va);
    else if (st == RUN_NOT_ENOUGH_OPERANDS)
        vfprintf(stderr, fmt ? fmt : "not enough operands\n", va);
    else if (st == RUN_UNKNOWN_OPERATION)
        vfprintf(stderr, fmt ? fmt : "unknown operation\n", va);
    else if (st == RUN_INVALID_TOKEN)
        vfprintf(stderr, fmt ? fmt : "invalid token\n", va);
    else if (st == RUN_NOT_IMPLEMENTED)
        vfprintf(stderr, fmt ? fmt : "not implemented\n", va);
    else if (st == RUN_TOO_MANY_OPERANDS)
        vfprintf(stderr, fmt ? fmt : "too many operands\n", va);
    else
        vfprintf(stderr, fmt ? fmt : "unknown error\n", va);

    va_end(va);
    return st;
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
static void reverse_(void)
{
    for (unsigned int i = 0; i < (operands_.size / 2); ++i)
    {
        const unsigned int nextLast = (operands_.size - 1) - i;
        double tmp = operands_.operands[i];
        operands_.operands[i] = operands_.operands[nextLast];
        operands_.operands[nextLast] = tmp;
    }
}
static const operand_stack_t* get_operand_stack(void)
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
