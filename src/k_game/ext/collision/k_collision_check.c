#include <assert.h>

#include "./_internal.h"

/* region [utils] */

/* 检测点与点是否发生碰撞
 *
 * `(x1, y1)` 是第一个点的坐标，`(x2, y2)` 是第二个点的坐标。
 */
static int check_collision_point_point(float x1, float y1, float x2, float y2) {
    return (x1 == x2) && (y1 == y2);
}

/* 检测点与线段是否发生碰撞
 *
 * `(x, y)` 是点的坐标，`(x1, y1)` 和 `(x2, y2)` 是线段的端点坐标。
 */
static int check_collision_point_line(float x, float y, float x1, float y1, float x2, float y2) {
    assert(0);
    return 0;
}

/* 检测点与矩形是否发生碰撞
 *
 * `(x, y)` 是点的坐标，`(x1, y1)` 和 `(x2, y2)` 是矩形的一组对角坐标。
 */
static int check_collision_point_rect(float x, float y, float x1, float y1, float x2, float y2) {
    return (x1 <= x == x <= x2) && (y1 <= y == y <= y2);
}

/* 检测点与圆是否发生碰撞
 *
 * `(x, y)` 是点的坐标，`(cx, cy)` 和 `r` 是圆的圆心坐标和半径。
 */
static int check_collision_point_circle(float x, float y, float cx, float cy, float r) {
    return (x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r;
}

/* 检测两条线段是否发生碰撞
 *
 * `(x1, y1)` 和 `(x2, y2)` 是第一条线段的端点坐标，
 * `(x3, y3)` 和 `(x4, y4)` 是第二条线段的端点坐标。
 */
static int check_collision_line_line(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    assert(0);
    return 0;
}

/* 检测线段与矩形是否发生碰撞
 *
 * `(lx1, ly1)` 和 `(lx2, ly2)` 是条线段的端点坐标，
 * `(rx1, ry1)` 和 `(rx2, ry2)` 是矩形的对角坐标。
 */
static int check_collision_line_rect(float lx1, float ly1, float lx2, float ly2, float rx1, float ry1, float rx2, float ry2) {
    assert(0);
    return 0;
}

/* 检测线段与圆是否发生碰撞
 *
 * `(x1, y1)` 和 `(x2, y2)` 是线段的端点坐标，
 * `(cx, cy)` 和 `r` 是圆的圆心坐标和半径。
 */
static int check_collision_line_circle(float x1, float y1, float x2, float y2, float cx, float cy, float r) {
    assert(0);
    return 0;
}

/* 检测两个矩形是否发生碰撞
 *
 * `(x1, y1)` 和 `(x2, y2)` 是第一个矩形的对角坐标，
 * `(x3, y3)` 和 `(x4, y4)` 是第二个矩形的对角坐标。
 */
static int check_collision_rect_rect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    return ( (x1 < x4) != (x2 < x3) && (y1 < y4) != (y2 < y3) )
        || ( (x1 < x3) != (x2 < x4) && (y1 < y3) != (y2 < y4) );
}

/* 检测矩形和圆形是否发生碰撞
 *
 * `(x1, y1)` 和 `(x2, y2)` 是矩形的一组对角坐标，
 * `(cx, cy)` 和 `r` 是圆形的圆心坐标和半径。
 */
static int check_collision_rect_circle(float x1, float y1, float x2, float y2, float cx, float cy, float r) {
    return check_collision_point_rect(cx - r, cy, x1, y1, x2, y2)
        || check_collision_point_rect(cx + r, cy, x1, y1, x2, y2)
        || check_collision_point_rect(cx, cy - r, x1, y1, x2, y2)
        || check_collision_point_rect(cx, cy + r, x1, y1, x2, y2)
        || check_collision_point_circle(x1, y1, cx, cy, r)
        || check_collision_point_circle(x1, y2, cx, cy, r)
        || check_collision_point_circle(x2, y1, cx, cy, r)
        || check_collision_point_circle(x2, y2, cx, cy, r);
}

/* 检测两个圆是否发生碰撞
 *
 * `(x1, y1)` 和 `r1` 是第一个圆的圆心坐标和半径，
 * `(x2, y2)` 和 `r2` 是第二个圆的圆心坐标和半径。
 */
static int check_collision_circle_circle(float x1, float y1, float r1, float x2, float y2, float r2) {
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) <= (r1 + r2) * (r1 + r2);
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

    struct k_collision_group *group = container_of(map_node, struct k_collision_group, group_map_node);
    return group;
}

struct k_collision_box *k_collision_check_point(int group_id, float x, float y) {

    struct k_collision_group *group = k__collision_manager_find_group(group_id);
    if (NULL == group)
        return NULL;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        switch (box->type) {
            case K_COLLISION_POINT: {
                float x2 = *(box->x) + box->point.offset_x;
                float y2 = *(box->y) + box->point.offset_y;
                if (check_collision_point_point(x, y, x2, y2))
                    return box;

                break;
            }
            case K_COLLISION_LINE: {
                float lx1 = *(box->x) + box->line.offset_x1;
                float ly1 = *(box->y) + box->line.offset_y1;
                float lx2 = *(box->x) + box->line.offset_x2;
                float ly2 = *(box->y) + box->line.offset_y2;
                if (check_collision_point_line(x, y, lx1, ly1, lx2, ly2))
                    return box;

                break;
            }
            case K_COLLISION_RECTANGLE: {
                float rx1 = *(box->x) + box->rect.offset_x1;
                float ry1 = *(box->y) + box->rect.offset_y1;
                float rx2 = *(box->x) + box->rect.offset_x2;
                float ry2 = *(box->y) + box->rect.offset_y2;
                if (check_collision_point_rect(x, y, rx1, ry1, rx2, ry2))
                    return box;

                break;
            }
            case K_COLLISION_CIRCLE: {
                float cx = *(box->x) + box->circle.offset_cx;
                float cy = *(box->y) + box->circle.offset_cy;
                float r  = box->circle.r;
                if (check_collision_point_circle(x, y, cx, cy, r))
                    return box;

                break;
            }
        }
    }

    return NULL;
}

struct k_collision_box *k_collision_check_line(int group_id, float x1, float y1, float x2, float y2) {

    struct k_collision_group *group = k__collision_manager_find_group(group_id);
    if (NULL == group)
        return NULL;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        switch (box->type) {
            case K_COLLISION_POINT: {
                float px = *(box->x) + box->point.offset_x;
                float py = *(box->y) + box->point.offset_y;
                if (check_collision_point_line(px, py, x1, y1, x2, y2))
                    return box;

                break;
            }
            case K_COLLISION_LINE: {
                float x3 = *(box->x) + box->line.offset_x1;
                float y3 = *(box->y) + box->line.offset_y1;
                float x4 = *(box->x) + box->line.offset_x2;
                float y4 = *(box->y) + box->line.offset_y2;
                if (check_collision_line_line(x1, y1, x2, y2, x3, y3, x4, y4))
                    return box;

                break;
            }
            case K_COLLISION_RECTANGLE: {
                float rx1 = *(box->x) + box->rect.offset_x1;
                float ry1 = *(box->y) + box->rect.offset_y1;
                float rx2 = *(box->x) + box->rect.offset_x2;
                float ry2 = *(box->y) + box->rect.offset_y2;
                if (check_collision_line_rect(x1, y1, x2, y2, rx1, ry1, rx2, ry2))
                    return box;

                break;
            }
            case K_COLLISION_CIRCLE: {
                float cx = *(box->x) + box->circle.offset_cx;
                float cy = *(box->y) + box->circle.offset_cy;
                float r  = box->circle.r;
                if (check_collision_line_circle(x1, y1, x2, y2, cx, cy, r))
                    return box;

                break;
            }
        }
    }

    return NULL;
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
            case K_COLLISION_POINT: {
                float px = *(box->x) + box->point.offset_x;
                float py = *(box->y) + box->point.offset_y;
                if (check_collision_point_rect(px, py, x1, y1, x2, y2))
                    return box;

                break;
            }
            case K_COLLISION_LINE: {
                float lx1 = *(box->x) + box->line.offset_x1;
                float ly1 = *(box->y) + box->line.offset_y1;
                float lx2 = *(box->x) + box->line.offset_x2;
                float ly2 = *(box->y) + box->line.offset_y2;
                if (check_collision_line_rect(lx1, ly1, lx2, ly2, x1, y1, x2, y2))
                    return box;

                break;
            }
            case K_COLLISION_RECTANGLE: {
                float x3 = *(box->x) + box->rect.offset_x1;
                float y3 = *(box->y) + box->rect.offset_y1;
                float x4 = *(box->x) + box->rect.offset_x2;
                float y4 = *(box->y) + box->rect.offset_y2;
                if (check_collision_rect_rect(x1, y1, x2, y2, x3, y3, x4, y4))
                    return box;

                break;
            }
            case K_COLLISION_CIRCLE: {
                float cx = *(box->x) + box->circle.offset_cx;
                float cy = *(box->y) + box->circle.offset_cy;
                float r  = box->circle.r;
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
            case K_COLLISION_POINT: {
                float px = *(box->x) + box->point.offset_x;
                float py = *(box->y) + box->point.offset_y;
                if (check_collision_point_circle(px, py, cx, cy, r))
                    return box;

                break;
            }
            case K_COLLISION_LINE: {
                float lx1 = *(box->x) + box->line.offset_x1;
                float ly1 = *(box->y) + box->line.offset_y1;
                float lx2 = *(box->x) + box->line.offset_x2;
                float ly2 = *(box->y) + box->line.offset_y2;
                if (check_collision_line_circle(lx1, ly1, lx2, ly2, cx, cy, r))
                    return box;

                break;
            }
            case K_COLLISION_RECTANGLE: {
                float rx1 = *(box->x) + box->rect.offset_x1;
                float ry1 = *(box->y) + box->rect.offset_y1;
                float rx2 = *(box->x) + box->rect.offset_x2;
                float ry2 = *(box->y) + box->rect.offset_y2;
                if (check_collision_rect_circle(rx1, ry1, rx2, ry2, cx, cy, r))
                    return box;

                break;
            }
            case K_COLLISION_CIRCLE: {
                float x2 = *(box->x) + box->circle.offset_cx;
                float y2 = *(box->y) + box->circle.offset_cy;
                float r2 =box->circle.r;
                if (check_collision_circle_circle(cx, cy, r, x2, y2, r2))
                    return box;

                break;
            }
        }
    }

    return NULL;
}

/* endregion */
