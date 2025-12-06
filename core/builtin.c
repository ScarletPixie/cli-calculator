#include "api.h"
#include "builtin.h"

#include <stdlib.h>

operation_t get_operation(void)
{
    return (operation_t){UNARY, NULL};
}
