#include <cstdio>
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
