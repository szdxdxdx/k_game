#include <stddef.h>

#define K_LOG_TAG "k_game"
#include "k_log.h"

#include "k_game/core/k_game_run.h"
#include "./k_game_init.h"
#include "./k_game_context.h"

#include "../room/k_room_stack.h"
#include "../room/k_room_run.h"

static void k__game_run() {

    struct k_room *room = k__room_stack_get_top();
    if (NULL == room) {
        k_log_error("no room to run");
        return;
    }

    while ( ! k__game.quit_game) {

        room = k__room_stack_get_top();
        if (NULL == room)
            break;

        k__room_run(room);
    }
}

void k_game_run(const struct k_game_config *config) {

    if (0 != k__game_init(config)) {
        k_log_error("failed to init game");
        goto err;
    }

    k__game_run();

    k__game_deinit(config);

    return;
err:;
}
