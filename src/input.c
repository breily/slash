#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>

#include "eval.h"
#include "input.h"

int read_file(char *file_name) {
    int in_home_dir = 0;
    if (strchr(file_name, '~') != NULL) {
        in_home_dir = 1;
        char *buf = calloc(1, strlen(getenv("HOME")) + strlen(file_name));
        strcpy(buf, getenv("HOME"));
        strcat(buf, ++file_name);
        file_name = calloc(1, strlen(buf) + 1);
        strcpy(file_name, buf);
        free(buf);
    }

    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        return 0;
    }

    char *command = calloc(1, 1024);
    strcpy(command, "");
    int len = 0;
    int c;
    while ((c = getc(fp)) != EOF) {
        if (c == '\n') {
            if (len) eval(command);
            free(command);
            char *command = calloc(1, 1024);
            strcpy(command, "");
            len = 0;
        } else {
            command[len] = c;
            len++;
        }
    }

    fclose(fp);
    if (in_home_dir) free(file_name);
    return 1;
}

char * read_stdin(char *prompt) {
    /*
    strcpy(ret, "");
    int len = 0;
    int cur;
    while ((cur = getchar()) != EOF) {
        if (cur == '\n') break;
        ret[len] = cur;
        len++;
    }
    if (cur == EOF && len == 0) return;
    ret[len] = '\0';
    */
    char *ret = readline(prompt);
    return ret;
}
