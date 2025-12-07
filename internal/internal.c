#include "internal.h"
#include "builtin/builtin.h"

#include <assert.h>
#include <string.h>
#include <sys/types.h>

static void load_builtin(void);

static operation_names_t loaded_operations_;
const operation_names_t* loaded_operations(void)
{
    static int initialized;
    if (!initialized)
    {
        load_builtin();
        initialized = 1;
    }

    return &loaded_operations_;
}

const operation_t* get_op(const char* name)
{
    const operation_t* op = NULL;

    assert(name != NULL);

    for (unsigned int i = 0; i < loaded_operations_.loaded_c; ++i)
    {
        op = &loaded_operations_.opers[i];
        const char* op_name = op->name;

        assert(op_name != NULL);

        if (strcmp(op_name, name) == 0)
            return op;
    }

    return op;
}
const operation_t* add_op(const operation_t op)
{
    unsigned int i = 0;

    assert(op.name != NULL);
    assert(loaded_operations_.loaded_c < MAX_OPS - 1);

    for (; i < loaded_operations_.loaded_c; ++i)
    {
        const char* name = loaded_operations_.opers[i].name;
        assert(name != NULL);
        if (strcmp(op.name, name) == 0)
            return NULL;
    }
    loaded_operations_.opers[i] = op;
    loaded_operations_.loaded_c++;
    return &loaded_operations_.opers[i];
}

// HELPERS
static void load_builtin(void)
{
    const operation_t* sucess = NULL;
    sucess = add_op(sum);
    assert(sucess != NULL);
    sucess = add_op(sub);
    assert(sucess != NULL);
    sucess = add_op(mul);
    assert(sucess != NULL);
    sucess = add_op(div);
    assert(sucess != NULL);
    (void)sucess;
}
