#include "api.h"
#include "internal/cli.h"
#include "internal/internal.h"
#include "internal/config/config.h"

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int run(void);
static int init_stacks(int c, char**args);
int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Usage: ./rpn-x <operands> <operation>\n");
        return 0;
    }
    set_plugin_list_path("./plugins.txt");

    (void)loaded_operations();;

    const int status = init_stacks(argc, argv);
    if (status)
        return status;

    return run();
}

static int run(void)
{
    const operand_stack_t* operands = get_operand_stack();
    const operation_queue_t* operations = get_operations_stack();

    if (operands->size == 0)
    {
        fprintf(stderr, "no operands\n");
        return 4;
    }
    for (unsigned int i = 0; operands->size != 0 && i < operands->size; ++i)
    {
        const operation_t* op = operations->pop();
        if (op == NULL)
            break;

        if (op->type == UNARY)
        {
            const double* v = operands->pop();
            if (v == NULL)
            {
                fprintf(stderr, "insuficient operands for %s\n", op->name);
                return 2;
            }
            operands->push(op->unary(*v));
        }
        else if (op->type == BINARY)
        {
            const double* v1 = operands->pop();
            const double* v2 = operands->pop();
            if (v1 == NULL || v2 == NULL)
            {
                fprintf(stderr, "insuficient operands for %s\n", op->name);
                return 2;
            }
            operands->push(op->binary(*v1, *v2));
        }
        else if (op->type == TERTIARY)
        {
            const double* v1 = operands->pop();
            const double* v2 = operands->pop();
            const double* v3 = operands->pop();
            if (v1 == NULL || v2 == NULL || v3 == NULL)
            {
                fprintf(stderr, "insuficient operands for %s\n", op->name);
                return 2;
            }
            operands->push(op->tertiary(*v1, *v2, *v3));
        }
        else if (op->type == N_ARY)
        {
            double val = op->n_ary(operands->size, operands->operands);
            operands->reset();
            operands->push(val);
        }
        else
        {
            fprintf(stderr, "insuficient operands for %s\n", op->name);
            return 6;
        }
    }
    if (operands->size != 1)
    {
        fprintf(stderr, "invalid expression\n");
        return 5;
    }
    printf("%f\n", *operands->pop());
    return 0;
}

static int init_stacks(int c, char** args)
{
    const operand_stack_t* operands = get_operand_stack();
    const operation_queue_t* operations = get_operations_stack();

    for (int i = 1; i < c; ++i)
    {
        char* tokenEnd;
        double value = strtod(args[i], &tokenEnd);
        if (*tokenEnd == args[i][0])
        {
            const operation_t* op = get_op(args[i]);
            if (op ==  NULL)
            {
                fprintf(stderr, "unkown operation: %s\n", args[i]);
                return 1;
            }
            operations->push(op);
        }
        else if (*tokenEnd == '\0')
            operands->push(value);
        else
        {
            fprintf(stderr, "invalid value: %s\n", args[i]);
            return 1;
        }
    }
    return 0;
}
