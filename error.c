#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

void error(char *fail, char *msg) {
    if (get_setting("@error_msgs?")) {
        fprintf(stderr, "error - '%s' failed: %s;\n", fail, msg);
    }
}

void f_error(char *fail, char *file) {
    char *msg = calloc(1, strlen(file) + 18);
    sprintf(msg, "could not open '%s'", file);
    error(fail, msg);
    free(msg);
}
