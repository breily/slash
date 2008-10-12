#include "bg.h"
#include "parser.h"
#include "environ.h"

int check_bg(PARSED tokens) {
    if (!strcmp(tokens.storage[tokens.count - 1], "&")) {
        free(tokens.storage[tokens.count - 1]);
        set_setting("@background?", 1);
        return (tokens.count - 1);
    } else set_setting("@background?", 0);
    return tokens.count;
}
