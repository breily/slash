#include <stdlib.h>
#include <stdio.h>

#include "environ.h"
#include "eval.h"
#include "init.h"

int main(int argc, char **argv) {
    init_settings();
    init_options(argc, argv);
    init_execute();
    init_script();
    init_history();
    init_msg();
    init_config();

    while (1) {
        char *prompt = (char *) print_prompt(get_env("@prompt"));
        char *command = calloc(1, 1024);
        command = (char *) read_stdin(prompt);
        eval(command);
        free(command);
        free(prompt);
    }
}
