#include "builtin.h"
#include <assert.h>
#include <math.h>
#include <string.h>

/* ------------------ UNARY ------------------ */

static double _neg(double x)            { return -x; }
static double _abs(double x)            { return fabs(x); }

static double _sin(double x)            { return sin(x); }
static double _cos(double x)            { return cos(x); }
static double _tan(double x)            { return tan(x); }

static double _asin(double x)           { return asin(x); }
static double _acos(double x)           { return acos(x); }
static double _atan(double x)           { return atan(x); }

static double _exp(double x)            { return exp(x); }
static double _ln(double x)             { return log(x); }
static double _sqrt(double x)           { return sqrt(x); }

/* ------------------ BINARY ------------------ */

static double _sum(double a, double b)  { return a + b; }
static double _sub(double a, double b)  { return a - b; }
static double _mul(double a, double b)  { return a * b; }
static double _div(double a, double b)  { return a / b; }

static double _pow(double a, double b)  { return pow(a, b); }
static double _mod(double a, double b)  { return fmod(a, b); }

static double _min(double a, double b)  { return a < b ? a : b; }
static double _max(double a, double b)  { return a > b ? a : b; }

/* ------------------ TERTIARY ------------------ */

static double _clamp(double x, double lo, double hi)
{
    return x < lo ? lo : (x > hi ? hi : x);
}

static double _lerp(double a, double b, double t)
{
    return a + (b - a) * t;
}

/* ------------------ N-ARY ------------------ */

static double _n_sum(int argc, const double args[])
{
    double r = 0.0;
    for (int i = 0; i < argc; i++) r += args[i];
    return r;
}

static double _n_product(int argc, const double args[])
{
    double r = 1.0;
    for (int i = 0; i < argc; i++) r *= args[i];
    return r;
}

static double _n_average(int argc, const double args[])
{
    if (argc == 0) return 0.0;
    double s = 0.0;
    for (int i = 0; i < argc; i++) s += args[i];
    return s / argc;
}

/* ------------------ OPERATIONS ------------------ */

/* Unary */
static const operation_t neg      = { .name="neg",  .type=UNARY, .unary=_neg };
static const operation_t abs_op   = { .name="abs",  .type=UNARY, .unary=_abs };
static const operation_t sin_op   = { .name="sin",  .type=UNARY, .unary=_sin };
static const operation_t cos_op   = { .name="cos",  .type=UNARY, .unary=_cos };
static const operation_t tan_op   = { .name="tan",  .type=UNARY, .unary=_tan };
static const operation_t asin_op  = { .name="asin", .type=UNARY, .unary=_asin };
static const operation_t acos_op  = { .name="acos", .type=UNARY, .unary=_acos };
static const operation_t atan_op  = { .name="atan", .type=UNARY, .unary=_atan };
static const operation_t exp_op   = { .name="exp",  .type=UNARY, .unary=_exp };
static const operation_t ln_op    = { .name="ln",   .type=UNARY, .unary=_ln };
static const operation_t sqrt_op  = { .name="sqrt", .type=UNARY, .unary=_sqrt };

/* Binary */
static const operation_t sum      = { .name="+",    .type=BINARY, .binary=_sum };
static const operation_t sub      = { .name="-",    .type=BINARY, .binary=_sub };
static const operation_t mul      = { .name="*",    .type=BINARY, .binary=_mul };
static const operation_t div      = { .name="/",    .type=BINARY, .binary=_div };
static const operation_t pow_op   = { .name="pow",  .type=BINARY, .binary=_pow };
static const operation_t mod      = { .name="mod",  .type=BINARY, .binary=_mod };
static const operation_t min_op   = { .name="min",  .type=BINARY, .binary=_min };
static const operation_t max_op   = { .name="max",  .type=BINARY, .binary=_max };

/* Tertiary */
static const operation_t clamp_op = { .name="clamp", .type=TERTIARY, .tertiary=_clamp };
static const operation_t lerp_op  = { .name="lerp",  .type=TERTIARY, .tertiary=_lerp };

/* N-ary */
static const operation_t n_sum    = { .name="sum",     .type=N_ARY, .n_ary=_n_sum };
static const operation_t n_prod   = { .name="product", .type=N_ARY, .n_ary=_n_product };
static const operation_t n_avg    = { .name="avg",     .type=N_ARY, .n_ary=_n_average };

builtin_operations_t builtins_;
static void insert_builtin(const operation_t* op);
const builtin_operations_t* get_builtins(void)
{
    static int initialized;
    if (!initialized)
    {
        initialized = 1;
        memset(&builtins_, 0, sizeof(builtins_));
        insert_builtin(&neg);
        insert_builtin(&abs_op);
        insert_builtin(&sin_op);
        insert_builtin(&cos_op);
        insert_builtin(&tan_op);
        insert_builtin(&asin_op);
        insert_builtin(&acos_op);
        insert_builtin(&atan_op);
        insert_builtin(&exp_op);
        insert_builtin(&ln_op);
        insert_builtin(&sqrt_op);
        insert_builtin(&sum);
        insert_builtin(&sub);
        insert_builtin(&mul);
        insert_builtin(&div);
        insert_builtin(&pow_op);
        insert_builtin(&mod);
        insert_builtin(&min_op);
        insert_builtin(&max_op);
        insert_builtin(&clamp_op);
        insert_builtin(&lerp_op);
        insert_builtin(&n_sum);
        insert_builtin(&n_prod);
        insert_builtin(&n_avg);
    }
    return &builtins_;
}
static void insert_builtin(const operation_t* op)
{
    assert(RPN_X_MAX_BUILTIN > 0);
    assert(op);
    assert(builtins_.size < RPN_X_MAX_BUILTIN);

    for (unsigned long i = 0; i < builtins_.size; ++i)
        assert(op != builtins_.builtins[i]);
    builtins_.builtins[builtins_.size] = op;
    builtins_.size++;
}
