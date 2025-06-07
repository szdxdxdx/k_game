#ifndef K_GAME_H
#define K_GAME_H

#include "./k_game/core/k_game_fwd.h"
#include "./k_game/core/k_window.h"
#include "./k_game/core/k_image.h"
#include "./k_game/core/k_sound.h"
#include "./k_game/core/k_font.h"
#include "./k_game/core/k_time.h"
#include "./k_game/core/k_sprite.h"
#include "./k_game/core/k_canvas.h"
#include "./k_game/core/k_keyboard.h"
#include "./k_game/core/k_mouse.h"
#include "./k_game/core/k_room.h"
#include "./k_game/core/k_view.h"
#include "./k_game/core/k_object.h"
#include "./k_game/core/k_component.h"
#include "./k_game/core/k_callback.h"
#include "./k_game/core/k_named_lookup.h"
#include "./k_game/core/k_game_run.h"

/* 内建组件 */
#include "./k_game/ext/k_sprite_renderer.h"
#include "./k_game/ext/k_collision_box.h"
#include "./k_game/ext/k_position.h"
#include "./k_game/ext/k_state_machine.h"
#include "./k_game/ext/k_camera.h"
#include "./k_game/ext/k_room_blackboard.h"

/* 其他工具 */
#include "./k_game/ext/k_behavior_tree.h"

#endif
