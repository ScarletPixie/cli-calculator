#ifndef CLI_H
#define CLI_H

#include <stdarg.h>

typedef enum
{
    RUN_SUCESS,
    RUN_UNKNOWN_OPERATION,
    RUN_INVALID_EXPRESSION,
    RUN_NOT_ENOUGH_OPERANDS,
    RUN_TOO_MANY_OPERANDS,
    RUN_INVALID_TOKEN,
    RUN_NOT_IMPLEMENTED,
}   run_status;

run_status run(int argc, char** argv);

#endif
