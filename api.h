#ifndef API_H
#define API_H

enum op_type
{
    UNARY,
    BINARY,
    TERTIARY,
    N_ARY
};

typedef struct
{
    const char* name;
    enum op_type type;

    union {
        double (*unary)(double);
        double (*binary)(double, double);
        double (*tertiary)(double, double, double);
        double (*n_ary)(int argc, const double argv[]);
    };
} operation_t;

typedef const operation_t* (*operation_getter)(void);
const operation_t* get_operation(void);

#endif
