#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

static plugin_paths_t plugins_;
static path_t path_;

static void load_file(void);
static void set_plugin_list_file(void);

const plugin_paths_t* get_plugin_paths(void)
{
    static int initialized;
    if (!initialized)
    {
        memset(&plugins_, 0, sizeof(plugins_));
        initialized = 1;
        set_plugin_list_file();
        load_file();
    }
    return &plugins_;
}

static void set_plugin_list_file(void)
{
    static const char* const ENV_NAME = "RPN_X_PLUGIN_PATH";
    static const char* PLUGIN_LIST_PATHS[] = {
        "./plugins.txt",
        NULL,
    };

    const char* env_plugin_path = getenv(ENV_NAME);
    if (env_plugin_path != NULL)
    {
        if (access(env_plugin_path, R_OK))
        {
            fprintf(stderr, "cannot read plugin path %s\n", env_plugin_path);
            return;
        }
    }
    for (int i = 0; PLUGIN_LIST_PATHS[i] != NULL; ++i)
    {
        if (access(PLUGIN_LIST_PATHS[i], R_OK) == 0)
        {
            strncpy(path_.data, PLUGIN_LIST_PATHS[i], PATH_MAX);
            return;
        }
    }
}

static void trim(char* s, unsigned long size);
static void load_file(void)
{
    if (path_.data[0] == '\0')
        return;

    unsigned long line = 1;

    FILE* fp = fopen(path_.data, "r");
    if (fp == NULL)
    {
        perror("failed to load plugin list file");
        return;
    }

    path_t buffer;
    while (fgets(buffer.data, sizeof(buffer.data) / sizeof(*buffer.data), fp) && line < 255)
    {
        const unsigned long size = strlen(buffer.data);
        trim(buffer.data, size);
        if (*buffer.data != '\0')
        {
            plugins_.plugin_paths[plugins_.path_c] = buffer;
            plugins_.path_c++;
        }
        ++line;
    }
}
static void trim(char* s, unsigned long size)
{
    assert(s != NULL);
    if (size == 0)
    {
        s[0] = '\0';
        return;
    }

    unsigned long i = 0;
    unsigned long j = size - 1;


    for (; i < size && isspace(s[i]); ++i);
    if (i == size)
    {
        s[0] = '\0';
        return;
    }

    while (!isspace(s[j]))
    {
        if (j == 0)
            break;
        --j;
    }
    
    const unsigned long tokenLength = j - i;
    memmove(s, &s[i], tokenLength * sizeof(s[0]));
    s[tokenLength] = '\0';
}
