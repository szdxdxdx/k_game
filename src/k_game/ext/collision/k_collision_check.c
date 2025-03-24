#include "./_internal.h"

/* region [utils] */

/* 检测一个点是否在矩形内
 *
 * `(x, y)` 是点的坐标，`(x1, y1)` 和 `(x2, y2)` 是矩形的一组对角坐标。
 */
static int is_point_in_rect(float x, float y, float x1, float y1, float x2, float y2) {
    return (x1 <= x == x <= x2) && (y1 <= y == y <= y2);
}

/* 检测一个点是否在圆形内
 *
 * `(x, y)` 是点的坐标，`(cx, cy)` 和 `r` 是圆形的圆心坐标和半径。
 */
static int is_point_in_circle(float x, float y, float cx, float cy, float r) {
    return (x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r;
}

/* 检测两个矩形是否发生碰撞
 *
 * `(x1, y1)` 和 `(x2, y2)` 是第一个矩形的一组对角坐标，
 * `(x3, y3)` 和 `(x4, y4)` 是第二个矩形的一组对角坐标。
 */
static int check_collision_rect_rect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    return ( (x1 < x4) != (x2 < x3) && (y1 < y4) != (y2 < y3) )
        || ( (x1 < x3) != (x2 < x4) && (y1 < y3) != (y2 < y4) );
}

/* 检测两个圆形是否发生碰撞
 *
 * `(x1, y1)` 和 `r1` 是第一个圆形的圆心坐标和半径，
 * `(x2, y2)` 和 `r2` 是第二个圆形的圆心坐标和半径。
 */
static int check_collision_circle_circle(float x1, float y1, float r1, float x2, float y2, float r2) {
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) <= (r1 + r2) * (r1 + r2);
}

/* 检测矩形和圆形是否发生碰撞
 *
 * `(x1, y1)` 和 `(x2, y2)` 是矩形的一组对角坐标，
 * `(cx, cy)` 和 `r` 是圆形的圆心坐标和半径。
 */
static int check_collision_rect_circle(float x1, float y1, float x2, float y2, float cx, float cy, float r) {
    return is_point_in_rect(cx - r, cy, x1, y1, x2, y2)
        || is_point_in_rect(cx + r, cy, x1, y1, x2, y2)
        || is_point_in_rect(cx, cy - r, x1, y1, x2, y2)
        || is_point_in_rect(cx, cy + r, x1, y1, x2, y2)
        || is_point_in_circle(x1, y1, cx, cy, r)
        || is_point_in_circle(x1, y2, cx, cy, r)
        || is_point_in_circle(x2, y1, cx, cy, r)
        || is_point_in_circle(x2, y2, cx, cy, r);
}

/* endregion */

/* region [collision_check] */

static struct k_collision_group *k__collision_manager_find_group(int group_id) {

    struct k_component_manager *component_manager = k_room_get_component_manager(k__component_type_collision);
    if (NULL == component_manager)
        return NULL;

    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);
    struct k_int_map_node *map_node = k_int_map_get(&manager->group_map, group_id);
    if (NULL == map_node)
        return NULL;

    struct k_collision_group *group = container_of(map_node, struct k_collision_group, map_node);
    return group;
}

struct k_collision_box *k_collision_check_rectangle(int group_id, float x1, float y1, float x2, float y2) {

    struct k_collision_group *group = k__collision_manager_find_group(group_id);
    if (NULL == group)
        return NULL;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        switch (box->type) {
            case K_COLLISION_RECTANGLE: {
                struct k_collision_rectangle *rect = &box->rect;

                float x3 = *(rect->x) + rect->offset_x1;
                float y3 = *(rect->y) + rect->offset_y1;
                float x4 = *(rect->x) + rect->offset_x2;
                float y4 = *(rect->y) + rect->offset_y2;
                if (check_collision_rect_rect(x1, y1, x2, y2, x3, y3, x4, y4))
                    return box;

                break;
            }
            case K_COLLISION_CIRCLE: {
                struct k_collision_circle *circle = &box->circle;

                float cx = *(circle->x) + circle->offset_cx;
                float cy = *(circle->y) + circle->offset_cy;
                float r  = circle->r;
                if (check_collision_rect_circle(x1, y1, x2, y2, cx, cy, r))
                    return box;

                break;
            }
        }
    }

    return NULL;
}

struct k_collision_box *k_collision_check_circle(int group_id, float cx, float cy, float r) {

    struct k_collision_group *group = k__collision_manager_find_group(group_id);
    if (NULL == group)
        return NULL;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        switch (box->type) {
            case K_COLLISION_RECTANGLE: {
                struct k_collision_rectangle *rect = &box->rect;

                float x1 = *(rect->x) + rect->offset_x1;
                float y1 = *(rect->y) + rect->offset_y1;
                float x2 = *(rect->x) + rect->offset_x2;
                float y2 = *(rect->y) + rect->offset_y2;
                if (check_collision_rect_circle(x1, y1, x2, y2, cx, cy, r))
                    return box;

                break;
            }
            case K_COLLISION_CIRCLE: {
                struct k_collision_circle *circle = &box->circle;

                float x2 = *(circle->x) + circle->offset_cx;
                float y2 = *(circle->y) + circle->offset_cy;
                float r2  = circle->r;
                if (check_collision_circle_circle(cx, cy, r, x2, y2, r2))
                    return box;

                break;
            }
        }
    }

    return NULL;
}

struct k_collision_box *k_collision_check_line(int group_id, float x1, float y1, float x2, float y2) {


    return NULL;
}

/* endregion */
