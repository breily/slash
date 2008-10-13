#include <string.h>
#include <stdlib.h>

#include <stdio.h>

#include "parser.h"

PARSED split(char *line, char splitter) {
    if (strchr(line, splitter) != NULL) {
        // Find out how much space is needed, and allocate it.
        int ct = _count_char(line, splitter) + 1;
        char **ret = calloc(ct, sizeof *ret);
        int r = 0;  // Number of items in array.

        // Buffer to hold the current command.
        char *buf = calloc(1, strlen(line));
        int quote_flag = 0;
        int i = 0;
        while (i < strlen(line)) {
            if (line[i] == splitter && (line[i + 1] == ' ' || line[i + 1] == '\t')) {
                if (quote_flag) {
                    buf[strlen(buf)] = line[i];
                } else {
                    r = _store(buf, ret, r);
                    free(buf);
                    buf = calloc(1, strlen(line) - strlen(buf) + 1);
                }
            } else {
                if(line[i] == '\'' || line[i] == '\"') {
                    if (quote_flag) quote_flag = 0;
                    else quote_flag = 1;
                }
                buf[strlen(buf)] = line[i];
            }
            i++;
        }
        if (strlen(buf)) {
            r = _store(buf, ret, r);
        }
        free(buf);
        if (r < 2) {
            PARSED p = { NULL, 0 };
            return p;
        } else {
            PARSED p = { ret, r };
            return p;
        }
    }
    PARSED p = { NULL, 0 };
    return p;
}

PARSED get_tokens(char *line) {
    int ct = _count_char(line, ' ') + 1;
    char **ret = calloc(ct, sizeof *ret);
    int r = 0;

    int sq_flag = 0;
    int dq_flag = 0;

    char *buf = calloc(1, strlen(line));

    int i = 0;
    while (i < strlen(line)) {
        if (line[i] == '\"' && (i == 0 || line[i - 1] != '\\')) {
            if (dq_flag) {
                dq_flag = 0;
                if (sq_flag) {
                    strncat(buf, &line[i], 1);
                } else {
                    r = _store(buf, ret, r);
                    free(buf);
                    buf = calloc(1, strlen(line) - strlen(buf) + 1);
                }
            } else {
                dq_flag = 1;
                if (sq_flag) {
                    strncat(buf, &line[i], 1);
                }
            }
        // End double quote parsing    
        } else if (line[i] == '\'' && (i == 0 || line[i - 1] != '\\')) {
            if (sq_flag) {
                sq_flag = 0;
                if (dq_flag) {
                    strncat(buf, &line[i], 1);
                } else {
                    r = _store(buf, ret, r);
                    free(buf);
                    buf = calloc(1, strlen(line) - strlen(buf) + 1);
                }
            } else {
                sq_flag = 1;
                if (dq_flag) {
                    strncat(buf, &line[i], 1);
                }
            }
        // End single quote parsing
        } else if (line[i] == ' ' && (i == 0 || line[i - 1] != '\\')) {
            if (sq_flag || dq_flag) {
                strncat(buf, &line[i], 1);
            } else if (strlen(buf) != 0) {
                r = _store(buf, ret, r);
                free(buf);
                buf = calloc(1, strlen(line) - strlen(buf) + 1);
            }
        // End space parsing
        } else if (line[i] == '\t') {
            if (sq_flag || dq_flag) {
                strncat(buf, &line[i], 1);
            } else if (strlen(buf) != 0) {
                r = _store(buf, ret, r);
                free(buf);
                buf = calloc(1, strlen(line) - strlen(buf) + 1);
            }
        // End tab parsing    
        } else {
            if (line[i] == '\\' && (line[i + 1] == '\"' || line[i + 1] == '\'')) {
            } else {
                strncat(buf, &line[i], 1);
            }
        }
        i++;
    }
    if (strlen(buf)) {
        r = _store(buf, ret, r);
    }
    free(buf);
    PARSED p = { ret, r };
    return p;
}

int _count_char(char *line, char search) {
    int ret = 0;
    int i = 0;
    while (i < strlen(line)) {
        if (line[i] == search) {
            ret++;
        }
        i++;
    }
    return ret;
}

int _store(char *buf, char **array, int r) {
    array[r] = calloc(1, strlen(buf) + 1);
    strcpy(array[r], buf);
    return (r + 1);
}

void free_parsed(PARSED p) {
    while (p.count > 0) {
        free(p.storage[p.count - 1]);
        p.count--;
    }
}

char* strip_comments(char *line) {
    if (strchr(line, '#') != NULL) {
        char *new_line = calloc(1, strlen(line));
        strcpy(new_line, "");
        int i = 0;
        while (i < strlen(line)) {
            if (line[i] == '#') break;
            new_line[i] = line[i];
            i++;
        }
        //printf("new line: '%s'\n", new_line);
        int ct = strlen(new_line);
        while(new_line[ct] == ' ' || new_line[ct] == '\t') {
            ct--;
        }
        //printf("command length: %d\n", ct);
        //free(line);
        char *line = calloc(1, ct + 2);
        strncpy(line, new_line, ct + 1);
        //printf("line: '%s'\n", line);
        free(new_line);
    }
    return line;
}

void replace_tilde(char *line) {
    if (strchr(line, '~') != NULL) {
        char *new_line = calloc(1, strlen(line) + strlen(getenv("HOME")));
        int i = 0;
        while (i < strlen(line)) {
            if (line[i] == '~') break;
            new_line[i] = line[i];
            i++;
        }
        strcat(new_line, getenv("HOME"));
        strcat(new_line, &line[i + 1]);
        line = realloc(line, strlen(new_line) + 1);
        strcpy(line, new_line);
        free(new_line);
    }
}
