#include "internal/internal.h"
#include "internal/config/config.h"

static void run(int c, char**args);
int main(int argc, char **argv)
{
    set_plugin_list_path("./plugina.txt");

    const operation_names_t* operations = loaded_operations();
    const plugin_paths_t* plugin_paths = get_plugin_paths();

    run(argc, argv);
    return 0;
}

static void run(int c, char** args)
{
    (void)c; (void)args;
    // TODO: parse CLI arguments
}
