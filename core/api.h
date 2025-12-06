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
    enum op_type type;
    void* tmp;
} operation_t;

operation_t get_operation(void);

#endif
