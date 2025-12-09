#include "builtin.h"

double _sum(double lhs, double rhs)
{
    return lhs + rhs;
}
double _sub(double lhs, double rhs)
{
    return lhs - rhs;
}
double _mul(double lhs, double rhs)
{
    return lhs * rhs;
}
double _div(double lhs, double rhs)
{
    return lhs / rhs;
}

const operation_t sum = {"+", BINARY, .binary=_sum};
const operation_t sub = {"-", BINARY, .binary=_sub};
const operation_t mul = {"*", BINARY, .binary=_mul};
const operation_t div = {"/", BINARY, .binary=_div};
