#include "limits.h"

#define K_LOG_TAG "yx:behavior_tree:example"
#include "k_log.h"

#include "k_game.h"

#include "yx_bt_example.h"

static enum k_behavior_tree_status action_inc(void *data) {
  *(int *)data += 1; return K_BT_SUCCESS; }
static enum k_behavior_tree_status action_log_if_even(void *data) {
  k_log_info("%d 是偶数", *(int *)data); return K_BT_SUCCESS; }
static enum k_behavior_tree_status action_log_if_odd(void *data) {
  k_log_info("%d 是奇数", *(int *)data); return K_BT_SUCCESS; }
static enum k_behavior_tree_status condition_is_even(void *data) {
  return (*(int *)data % 2 == 0) ? K_BT_SUCCESS : K_BT_FAILURE; }

static struct k_behavior_tree *tree = NULL; /* 行为树实例 */
static int num = 0; /* 全局变量，表示行为树操控的数据 */
/* 创建行为树 */
static void create_tree(void) {
  struct k_behavior_tree_builder *b; /* 通过 builder 来创建行为树 */
  k_bt_builder(&tree, b) {
    k_bt_delay(b, 500) { /* 延时节点，每 500 毫秒执行一次 */
      k_bt_sequence(b) {
        k_bt_action(b, &num, action_inc, NULL); /* 给 num 自增 */
        k_bt_sequence(b) { /* 选择 */
          k_bt_inverter(b) { /* 反转结果 */
            k_bt_sequence(b) {
              /* 条件节点，判断是不是偶数 */
              k_bt_condition(b, &num, condition_is_even);
              k_bt_action(b, &num, action_log_if_even, NULL); /* 输出 */
            }
          }
          k_bt_action(b, &num, action_log_if_odd, NULL); /* 输出 */
        }
      }
    }
  }
}
/* 每帧主动更新行为树 */
static void on_step(void *data) { k_behavior_tree_tick(tree, k_time_get_step_delta_ms()); }

void yx_bt_run_example(void) {
    create_tree();
    k_room_add_step_callback(NULL, on_step);
}
