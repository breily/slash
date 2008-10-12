#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>

#include "init.h"
#include "options.h"
#include "eval.h"
#include "environ.h"
#include "aliases.h"
#include "input.h"
#include "config.h"

void init_settings() {
    set_env("@pwd",             "~");
    set_env("@oldpwd",          "~");
    set_env("@config",          CONFIG_FILE);
    set_env("@start",           START_MESSAGE);
    set_env("@exit",            EXIT_MESSAGE);
    set_env("@prompt",          PROMPT);
    set_env("@version",         VERSION);
    set_env("@author",          AUTHOR);
    set_env("@verbose?",        VERBOSE);
    set_env("@glob?",           GLOB);
    set_env("@config?",         CONFIG);
    set_env("@aliases?",        ALIASES);
    set_env("@ls_after_cd?",    LS_AFTER_CD);
    set_env("@ls_command",      LS_COMMAND);
    set_env("@no_hist_repeat?", NO_HISTORY_REPEAT);
    set_env("@history?",        HISTORY);
    set_env("@use_hist_file?",  USE_HISTORY_FILE);
    set_env("@hist_file",       HISTORY_FILE);
    set_env("@error_msgs?",     ERROR_MESSAGES);
    set_env("@execute?",        "0");
    set_env("@exit?",           "0");
    set_env("@script?",         "0");
    set_env("@background?",     "0");
}

void init_options(int argc, char **argv) {
    parse_opts(argc, argv);
}

void init_execute() {
    if (get_setting("@execute?")) {
        set_env("@history?", "0");
        eval(get_env("@command"));
        if (get_setting("@exit?")) {
            exit(0);
        }
        set_env("@history?", "1");
    }
}    

void init_script() {
    if (get_setting("@script?")) {
        set_env("@history?", "0");
        int ret = read_file(get_env("@script"));
        if (ret == 0) {
            f_error("script", get_env("@script"));
        }
        if (get_setting("@exit?")) {
            exit(0);
        }
        set_env("@history?", "1");
    }
}

void init_history() {
    using_history();
    if (get_setting("@history?") && get_setting("@use_hist_file?")) {
        if (read_history(get_env("@hist_file")) != 0) {
            error("history", "couldn't read history file");
        }
    }
}

void init_msg() {
    if (get_setting("@verbose?")) {
        printf("%s\n", get_env("@start"));
    }
}    

void init_config() {
    if (get_setting("@config?")) {
        int ret = read_file(get_env("@config"));
        if (ret == 0) {
            f_error("config", get_env("@config"));
        }
    }
}
