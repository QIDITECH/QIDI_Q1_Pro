#include "../include/MakerbaseShell.h"

void execute_cmd(const char *cmd, char *result) {
    char buf_ps[MAX_FILE_LEN];
    char ps[MAX_FILE_LEN];
    FILE *ptr;
    strcpy(ps, cmd);
    if ((ptr = popen(ps, "r")) != NULL) {
        while (fgets(buf_ps, MAX_FILE_LEN, ptr) != NULL)
        {
            strcat(result, buf_ps);
            if (strlen(result) > MAX_FILE_LEN) 
                break;
        }
        pclose(ptr);
    } else {
        printf("popen %s error\n", ps);
    }
}
