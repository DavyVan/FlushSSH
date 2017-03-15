#include <cstdio>
#include <cstring>
#include <ctype.h>
#include "utils.h"

void display_help()
{
    puts("Usage: FlushSSH hosts_file cmd_file");
    puts("       Ctrl+C will terminate elegantly");
    puts("hosts_file:");
    puts("    IPaddr username passwd");
    puts("cmd_file:");
    puts("    One command per line.");
}

char *trim(char *str)
{
    char *p = str;
    char *pe;
    if(p != NULL)
    {
        pe = p + strlen(str) - 1;   //last char
        if(pe < p)
            return p;
        while(isspace(*p))
            p++;
        while(pe > p && isspace(*pe))
            *(pe--) = '\0';
    }
    return p;
}