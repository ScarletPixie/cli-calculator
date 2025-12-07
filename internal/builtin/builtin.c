#include "builtin.h"

static double _sum(double lhs, double rhs)
{
    return lhs + rhs;
}
static double _sub(double lhs, double rhs)
{
    return lhs - rhs;
}
static double _mul(double lhs, double rhs)
{
    return lhs * rhs;
}
static double _div(double lhs, double rhs)
{
    return lhs / rhs;
}

const operation_t sum = {"sum", BINARY, .binary=_sum};
const operation_t sub = {"sum", BINARY, .binary=_sub};
const operation_t mul = {"sum", BINARY, .binary=_mul};
const operation_t div = {"sum", BINARY, .binary=_div};
