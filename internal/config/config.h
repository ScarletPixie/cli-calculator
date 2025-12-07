#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

typedef struct
{
    char data[PATH_MAX];
}   path_t;

typedef struct
{
    unsigned int path_c;
    path_t plugin_paths[255];
}   plugin_paths_t;


int set_plugin_list_path(const char* path);

const plugin_paths_t* get_plugin_paths(void);

#endif
