#include "api.h"
#include "config/config.h"
#include "builtin/builtin.h"
#include "operation_loader.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dlfcn.h>

typedef union
{
    void* obj;
    operation_getter f;
}   _lib_operation_getter;

static void do_load_plugins(void);
static void load_builtin(void);

static operation_names_t loaded_operations_;

void load_plugins(void)
{
    static int initialized;
    if (!initialized)
    {
        for (int i = 0; i < MAX_OPS; ++i)
            loaded_operations_.handles[i] = NULL;

        load_builtin();
        do_load_plugins();
        initialized = 1;
    }
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

    return NULL;
}
const operation_t* add_op(const operation_t op)
{
    unsigned int i = 0;

    assert(op.name != NULL);
    if (loaded_operations_.loaded_c >= MAX_OPS)
    {
        fprintf(stderr, "cannot add more plugins\n");
        return NULL;
    }

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

void unload_plugins(void)
{
    for (unsigned int i = 0; i < loaded_operations_.loaded_c; ++i)
    {
        void *handler = loaded_operations_.handles[loaded_operations_.loaded_c];
        if (handler != NULL)
            dlclose(handler);
    }
}

// HELPERS
static void do_load_plugins(void)
{
    const plugin_paths_t* plugins = get_plugin_paths();

    if (plugins->path_c > MAX_OPS)
        fprintf(stderr, "too many plugins listed, ignoring the last %d plugins\n", plugins->path_c - MAX_OPS);
    
    const unsigned long size = plugins->path_c > MAX_OPS ? MAX_OPS : plugins->path_c;
    for (unsigned long i = 0; i < size; ++i)
    {
        const char* path = plugins->plugin_paths[i].data;
        void *handler = dlopen(path, RTLD_LAZY);
        if (handler == NULL)
        {
            fprintf(stderr, "failed to load plugin: %s\n", path);
            continue;
        }

        _lib_operation_getter getter = {.obj=dlsym(handler, "get_operation")};
        const char* error = dlerror();
        if (error != NULL)
        {
            fprintf(stderr, "failed to load plugin: %s\n", path);
            continue;
        }
        else if (getter.obj == NULL)
        {
            fprintf(stderr, "failed to load plugin (NULL callback): %s\n", path);
            continue;
        }
        loaded_operations_.handles[loaded_operations_.loaded_c] = handler;
        add_op(*(getter.f()));
    }
}
static void load_builtin(void)
{
    const builtin_operations_t* builtins = get_builtins();
    for (unsigned long i = 0; i < builtins->size; ++i)
    {
        const operation_t* op = builtins->builtins[i];
        assert(op);
        assert(op->unary);
        const operation_t* success = add_op(*op);
        assert(success);
        (void)success;
    }
}
