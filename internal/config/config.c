#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static plugin_paths_t plugins_;
static path_t path_;

static void load_file(void);

const plugin_paths_t* get_plugin_paths(void)
{
    static int initialized;
    if (!initialized)
    {
        initialized = 1;
        load_file();
    }
    return &plugins_;
}

int set_plugin_list_path(const char* path)
{
    assert(path != NULL);

    if (access(path, F_OK | R_OK))
        return 0;

    strcpy(path_.data, path);
    return 1;
}

static void trim(char* s, unsigned long size);
static void load_file(void)
{
    unsigned long line = 1;
    plugin_paths_t tmp_;


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
            tmp_.plugin_paths[tmp_.path_c] = buffer;
            tmp_.path_c++;
        }
        ++line;
    }

    plugins_ = tmp_;
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

    for (; j >= 0 && isspace(s[j]); --j);
    
    const unsigned long tokenLength = j - i;
    memmove(s, &s[i], tokenLength);
    s[tokenLength] = '\0';
}
