#include <stddef.h>

#include "k_log.h"

#include "./k_game_init.h"

#include "../room/k_room_stack.h"
#include "../room/k_room_run.h"

static void run_game() {

    struct k_room *room = k__room_stack_get_top();
    if (NULL == room) {
        k_log_error("No room to run");
        return;
    }

    k__room_run(room);
}

int k_run_game(const struct k_game_config *config) {

    if (0 != k__init_game(config))
        return -1;

    run_game();

    k__deinit_game(config);
    return 0;
}
