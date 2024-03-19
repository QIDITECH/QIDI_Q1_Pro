#ifndef MAKERBASE_SHELL_H
#define MAKERBASE_SHELL_H

#include <stdio.h>
#include <string.h>

#define MAX_FILE_LEN    1024*4

void execute_cmd(const char *cmd, char *result);

#endif
