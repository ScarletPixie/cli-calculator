#include "api.h"
#include "internal/cli.h"
#include "internal/internal.h"
#include "internal/config/config.h"

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

static int run(int c, char**args);
int main(int argc, char **argv)
{
    set_plugin_list_path("./plugins.txt");

    const operation_names_t* operations = loaded_operations();
    const plugin_paths_t* plugin_paths = get_plugin_paths();

    (void)operations; (void)plugin_paths;
    return run(argc, argv);
}

// TODO: use operations stack
static int run(int c, char** args)
{
    const operand_stack_t* operands = get_operand_stack();
    //const operation_stack_t* operations = get_operations_stack();

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
            else if (op->type == BINARY)
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
        }
        else if (*tokenEnd == '\0')
            operands->push(value);
        else
        {
            fprintf(stderr, "invalid value: %s\n", args[i]);
            return 1;
        }
    }
    const double *v = operands->pop();
    if (v == NULL)
    {
        fprintf(stderr, "invalid expression");
        return 3;
    }
    else if (v != NULL && operands->pop() != NULL)
    {
        fprintf(stderr, "invalid expression");
        return 3;
    }
    printf("%f", *v);
    return 0;
}
