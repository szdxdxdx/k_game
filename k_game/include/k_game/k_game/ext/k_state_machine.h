#ifndef K_STATE_MACHINE_H
#define K_STATE_MACHINE_H

#include "k_game/core/k_game_fwd.h"

/**
 * \brief 状态机
 *
 * 状态机是游戏开发中管理对象行为状态的经典工具。
 * 它将对象的不同状态看做独立的节点，同一时刻只有一个节点被激活。
 * 每个节点都有明确的进入、执行和退出逻辑，确保状态转换是明确可控的。
 *
 * 在游戏中，对象往往有许多的行为状态，例如站立、移动、攻击、死亡等。
 * 若状态数量过多，if-else、switch-case 将难以应对复杂的状态切换。
 * 而状态机的核心优势，就在于它强制开发者以模块化的方式组织代码，
 * 避免了复杂的条件判断和全局变量的滥用，让代码逻辑更清晰。
 *
 * k_state_machine 的本质是 k_component。
 */
struct k_state_machine;

/**
 * \brief 为对象挂载一个状态机
 *
 * 若成功，函数返回状态机的指针，否则返回 `NULL`。
 */
struct k_state_machine *k_object_add_state_machine(struct k_object *object);

/**
 * \brief 移除对象上挂载的状态机
 *
 * 若 `machine` 为 `NULL`，则函数立即返回。
 */
void k_object_del_state_machine(struct k_state_machine *machine);

/**
 * \brief 状态机的状态节点
 *
 * 切换状态机的状态节点时，先执行当前旧节点的 `on_exit()`，
 * 然后将当前节点设为新节点，并执行新节点的 `on_enter()`，
 * 之后每帧执行该节点的 `on_step()`。
 *
 * 状态节点的回调都是可选的，设为 `NULL` 则不执行回调。
 */
struct k_state_machine_state {

    /** \brief 进入该状态时触发的回调 */
    void (*on_enter)(struct k_object *object);

    /** \brief 该状态持续期间每帧触发的回调 */
    void (*on_step)(struct k_object *object);

    /** \brief 退出该状态时触发的回调 */
    void (*on_exit)(struct k_object *object);
};

/** \brief 切换状态机的状态节点 */
void k_state_machine_change_state(struct k_state_machine *machine, struct k_state_machine_state state);

#endif
