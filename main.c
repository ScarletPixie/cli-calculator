#include "internal/cli.h"
#include "internal/operation_loader.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {

        printf("Usage: rpn-x <operand/operator>...\n\n"
            "Example:\n"
            "  rpn-x 3 4 + 2 * 5 -\n"
        );
        return 0;
    }

    load_plugins();

    run_status status = run(argc, argv);

    unload_plugins();
    return status;
}
