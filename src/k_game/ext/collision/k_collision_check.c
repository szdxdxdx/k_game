#include "./_internal.h"

/* region [utils] */

/* 计算向量 `(x1, y1)` 与 `(x2, y2)` 的叉积 */
static float calc_vector_cross(float x1, float y1, float x2, float y2) {
    return x1 * y2 - x2 * y1;
}

/* 检测点与点是否发生碰撞。`(x1, y1)` 和 `(x2, y2)` 是第两个点的坐标 */
static int check_collision_point_point(float x1, float y1, float x2, float y2) {
    return (x1 == x2) && (y1 == y2);
}

/* 检测点与线段是否发生碰撞。`(x, y)` 是点的坐标，`(x1, y1)` 和 `(x2, y2)` 是线段的端点坐标 */
static int check_collision_point_line(float x, float y, float x1, float y1, float x2, float y2) {
    return (x1 <= x == x <= x2)
        && (y1 <= y == y <= y2)
        && calc_vector_cross(x2 - x1, y - y1, y2 - y1, x - x1);
}

/* 检测点与矩形是否发生碰撞。`(x, y)` 是点的坐标，`(x1, y1)` 和 `(x2, y2)` 是矩形的一组对角坐标 */
static int check_collision_point_rect(float x, float y, float x1, float y1, float x2, float y2) {
    return (x1 <= x == x <= x2) && (y1 <= y == y <= y2);
}

/* 检测点与圆是否发生碰撞。`(x, y)` 是点的坐标，`(cx, cy)` 和 `r` 是圆的圆心坐标和半径 */
static int check_collision_point_circle(float x, float y, float cx, float cy, float r) {
    return (x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r;
}

/* 检测两条线段是否发生碰撞。`(x1, y1)` 和 `(x2, y2)` 是第一条线段的端点坐标，`(x3, y3)` 和 `(x4, y4)` 是第二条线段的端点坐标 */
static int check_collision_line_line(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

    float max_x12, min_x12; if (x1 < x2) { max_x12 = x2; min_x12 = x1; } else { max_x12 = x1; min_x12 = x2; }
    float max_x34, min_x34; if (x3 < x4) { max_x34 = x4; min_x34 = x3; } else { max_x34 = x3; min_x34 = x3; }
    if (max_x12 < min_x34 || max_x34 < min_x12)
        return 0;

    float max_y12, min_y12; if (y1 < y2) { max_y12 = y2; min_y12 = y1; } else { max_y12 = y1; min_y12 = y2; }
    float max_y34, min_y34; if (y3 < y4) { max_y34 = y4; min_y34 = y3; } else { max_y34 = y3; min_y34 = y3; }
    if (max_y12 < min_y34 || max_y34 < min_y12)
        return 0;

    float c1 = calc_vector_cross(x3 - x1, y3 - y1, x2 - x1, y2 - y1);
    float c2 = calc_vector_cross(x4 - x1, y4 - y1, x2 - x1, y2 - y1);
    float c3 = calc_vector_cross(x1 - x3, y1 - y3, x4 - x3, y4 - y3);
    float c4 = calc_vector_cross(x2 - x3, y2 - y3, x4 - x3, y4 - y3);
    if (c1 * c2 < 0 && c3 * c4 < 0)
        return 1;

    if (c1 == 0 && min_x12 <= x3 && x3 <= max_x12 && min_y12 <= y3 && y3 <= max_y12) return 1;
    if (c2 == 0 && min_x12 <= x4 && x4 <= max_x12 && min_y12 <= y4 && y4 <= max_y12) return 1;
    if (c3 == 0 && min_x34 <= x1 && x1 <= max_x34 && min_y34 <= y1 && y1 <= max_y34) return 1;
    if (c4 == 0 && min_x34 <= x2 && x2 <= max_x34 && min_y34 <= y2 && y2 <= max_y34) return 1;

    return 0;
}

/* 检测线段与矩形是否发生碰撞。`(lx1, ly1)` 和 `(lx2, ly2)` 是条线段的端点坐标，`(rx1, ry1)` 和 `(rx2, ry2)` 是矩形的对角坐标 */
static int check_collision_line_rect(float lx1, float ly1, float lx2, float ly2, float rx1, float ry1, float rx2, float ry2) {
    return check_collision_point_rect(lx1, ly1, rx1, ry1, rx2, ry2)
        || check_collision_point_rect(lx2, ly1, rx1, ry1, rx2, ry2)
        || check_collision_line_line(lx1, ly1, lx2, ly2, rx1, ry1, rx2, ry1)
        || check_collision_line_line(lx1, ly1, lx2, ly2, rx2, ry1, rx2, ry2)
        || check_collision_line_line(lx1, ly1, lx2, ly2, rx2, ry2, rx1, ry2)
        || check_collision_line_line(lx1, ly1, lx2, ly2, rx1, ry2, rx1, ry1);
}

/* 检测线段与圆是否发生碰撞。`(x1, y1)` 和 `(x2, y2)` 是线段的端点坐标，`(cx, cy)` 和 `r` 是圆的圆心坐标和半径 */
static int check_collision_line_circle(float x1, float y1, float x2, float y2, float cx, float cy, float r) {

    if (check_collision_point_circle(x1, y1, cx, cy, r)) return 1;
    if (check_collision_point_circle(x2, y2, cx, cy, r)) return 1;

    float dx = x2 - x1;
    float dy = y2 - y1;
    float t = ((cx - x1) * dx + (cy - y1) * dy) / (dx * dx + dy * dy);
    float px = x1 + t * dx;
    float py = y1 + t * dy;

    return check_collision_point_rect(px, py, x1, y1, x2, y2)
        && check_collision_point_circle(px, py, cx, cy, r);
}

/* 检测两个矩形是否发生碰撞。`(x1, y1)` 和 `(x2, y2)` 是第一个矩形的对角坐标，`(x3, y3)` 和 `(x4, y4)` 是第二个矩形的对角坐标 */
static int check_collision_rect_rect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    return ( (x1 < x4) != (x2 < x3) && (y1 < y4) != (y2 < y3) )
        || ( (x1 < x3) != (x2 < x4) && (y1 < y3) != (y2 < y4) );
}

/* 检测矩形和圆是否发生碰撞。`(x1, y1)` 和 `(x2, y2)` 是矩形的一组对角坐标，`(cx, cy)` 和 `r` 是圆形的圆心坐标和半径 */
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

/* 检测两个圆是否发生碰撞。`(x1, y1)` 和 `r1` 是第一个圆的圆心坐标和半径，`(x2, y2)` 和 `r2` 是第二个圆的圆心坐标和半径 */
static int check_collision_circle_circle(float x1, float y1, float r1, float x2, float y2, float r2) {
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) <= (r1 + r2) * (r1 + r2);
}

/* 检测点和碰撞盒是否发生碰撞。`(x, y)` 是点的坐标，`box` 是碰撞盒 */
static int check_collision_point_box(float x, float y, struct k_collision_box *box) {
    switch (box->type) {
        case K__COLLISION_POINT: {
            float x2 = *(box->x) + box->point.offset_x;
            float y2 = *(box->y) + box->point.offset_y;
            return check_collision_point_point(x, y, x2, y2);
        }
        case K__COLLISION_LINE: {
            float lx1 = *(box->x) + box->line.offset_x1;
            float ly1 = *(box->y) + box->line.offset_y1;
            float lx2 = *(box->x) + box->line.offset_x2;
            float ly2 = *(box->y) + box->line.offset_y2;
            return check_collision_point_line(x, y, lx1, ly1, lx2, ly2);
        }
        case K__COLLISION_RECTANGLE: {
            float rx1 = *(box->x) + box->rect.offset_x1;
            float ry1 = *(box->y) + box->rect.offset_y1;
            float rx2 = *(box->x) + box->rect.offset_x2;
            float ry2 = *(box->y) + box->rect.offset_y2;
            return check_collision_point_rect(x, y, rx1, ry1, rx2, ry2);
        }
        case K__COLLISION_CIRCLE: {
            float cx = *(box->x) + box->circle.offset_cx;
            float cy = *(box->y) + box->circle.offset_cy;
            float r  = box->circle.r;
            return check_collision_point_circle(x, y, cx, cy, r);
        }
        default: return 0;
    }
}

/* 检测线段和碰撞盒是否发生碰撞。`(x1, y1)` 和 `(x2, y2)` 是线段的端点坐标，`box` 是碰撞盒 */
static int check_collision_line_box(float x1, float y1, float x2, float y2, struct k_collision_box *box) {
    switch (box->type) {
        case K__COLLISION_POINT: {
            float px = *(box->x) + box->point.offset_x;
            float py = *(box->y) + box->point.offset_y;
            return check_collision_point_line(px, py, x1, y1, x2, y2);
        }
        case K__COLLISION_LINE: {
            float x3 = *(box->x) + box->line.offset_x1;
            float y3 = *(box->y) + box->line.offset_y1;
            float x4 = *(box->x) + box->line.offset_x2;
            float y4 = *(box->y) + box->line.offset_y2;
            return check_collision_line_line(x1, y1, x2, y2, x3, y3, x4, y4);
        }
        case K__COLLISION_RECTANGLE: {
            float rx1 = *(box->x) + box->rect.offset_x1;
            float ry1 = *(box->y) + box->rect.offset_y1;
            float rx2 = *(box->x) + box->rect.offset_x2;
            float ry2 = *(box->y) + box->rect.offset_y2;
            return check_collision_line_rect(x1, y1, x2, y2, rx1, ry1, rx2, ry2);
        }
        case K__COLLISION_CIRCLE: {
            float cx = *(box->x) + box->circle.offset_cx;
            float cy = *(box->y) + box->circle.offset_cy;
            float r  = box->circle.r;
            return check_collision_line_circle(x1, y1, x2, y2, cx, cy, r);
        }
        default: return 0;
    }
}

/* 检测矩形和碰撞盒是否发生碰撞。`(x1, y1)` 和 `(x2, y2)` 是矩形的对角坐标，`box` 是碰撞盒 */
static int check_collision_rect_box(float x1, float y1, float x2, float y2, struct k_collision_box *box) {
    switch (box->type) {
        case K__COLLISION_POINT: {
            float px = *(box->x) + box->point.offset_x;
            float py = *(box->y) + box->point.offset_y;
            return check_collision_point_rect(px, py, x1, y1, x2, y2);
        }
        case K__COLLISION_LINE: {
            float lx1 = *(box->x) + box->line.offset_x1;
            float ly1 = *(box->y) + box->line.offset_y1;
            float lx2 = *(box->x) + box->line.offset_x2;
            float ly2 = *(box->y) + box->line.offset_y2;
            return check_collision_line_rect(lx1, ly1, lx2, ly2, x1, y1, x2, y2);
        }
        case K__COLLISION_RECTANGLE: {
            float x3 = *(box->x) + box->rect.offset_x1;
            float y3 = *(box->y) + box->rect.offset_y1;
            float x4 = *(box->x) + box->rect.offset_x2;
            float y4 = *(box->y) + box->rect.offset_y2;
            return check_collision_rect_rect(x1, y1, x2, y2, x3, y3, x4, y4);
        }
        case K__COLLISION_CIRCLE: {
            float cx = *(box->x) + box->circle.offset_cx;
            float cy = *(box->y) + box->circle.offset_cy;
            float r  = box->circle.r;
            return check_collision_rect_circle(x1, y1, x2, y2, cx, cy, r);
        }
        default: return 0;
    }
}

/* 检测圆和碰撞盒是否发生碰撞。`(cx, cy)` 是圆的圆心坐标，`r` 是圆的半径，`box` 是碰撞盒 */
static int check_collision_circle_box(float cx, float cy, float r, struct k_collision_box *box) {
    switch (box->type) {
        case K__COLLISION_POINT: {
            float px = *(box->x) + box->point.offset_x;
            float py = *(box->y) + box->point.offset_y;
            return check_collision_point_circle(px, py, cx, cy, r);
        }
        case K__COLLISION_LINE: {
            float lx1 = *(box->x) + box->line.offset_x1;
            float ly1 = *(box->y) + box->line.offset_y1;
            float lx2 = *(box->x) + box->line.offset_x2;
            float ly2 = *(box->y) + box->line.offset_y2;
            return check_collision_line_circle(lx1, ly1, lx2, ly2, cx, cy, r);
        }
        case K__COLLISION_RECTANGLE: {
            float rx1 = *(box->x) + box->rect.offset_x1;
            float ry1 = *(box->y) + box->rect.offset_y1;
            float rx2 = *(box->x) + box->rect.offset_x2;
            float ry2 = *(box->y) + box->rect.offset_y2;
            return check_collision_rect_circle(rx1, ry1, rx2, ry2, cx, cy, r);
        }
        case K__COLLISION_CIRCLE: {
            float x2 = *(box->x) + box->circle.offset_cx;
            float y2 = *(box->y) + box->circle.offset_cy;
            float r2 =box->circle.r;
            return check_collision_circle_circle(cx, cy, r, x2, y2, r2);
        }
        default: return 0;
    }
}

/* endregion */

/* region [collision_check] */

struct k_collision_box *k_collision_check_point(int group_id, float x, float y) {

    struct k_collision_group *group = k__collision_find_group(group_id);
    if (NULL == group)
        return NULL;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        if (check_collision_point_box(x, y, box))
            return box;
    }

    return NULL;
}

struct k_collision_box *k_collision_check_line(int group_id, float x1, float y1, float x2, float y2) {

    struct k_collision_group *group = k__collision_find_group(group_id);
    if (NULL == group)
        return NULL;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        if (check_collision_line_box(x1, y1, x2, y2, box))
            return box;
    }

    return NULL;
}

struct k_collision_box *k_collision_check_rect(int group_id, float x1, float y1, float x2, float y2) {

    struct k_collision_group *group = k__collision_find_group(group_id);
    if (NULL == group)
        return NULL;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        if (check_collision_rect_box(x1, y1, x2, y2, box))
            return box;
    }

    return NULL;
}

struct k_collision_box *k_collision_check_circle(int group_id, float cx, float cy, float r) {

    if (r <= 0.0f)
        return k_collision_check_point(group_id, cx, cy);

    struct k_collision_group *group = k__collision_find_group(group_id);
    if (NULL == group)
        return NULL;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        if (check_collision_circle_box(cx, cy, r, box))
            return box;
    }

    return NULL;
}

/* endregion */

/* region [collision_query] */

size_t k_collision_query_point(struct k_collision_box **result, size_t n, int group_id, float x, float y) {

    if (NULL == result || 0 == n)
        return 0;

    struct k_collision_group *group = k__collision_find_group(group_id);
    if (NULL == group)
        return 0;

    size_t count = 0;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        if (check_collision_point_box(x, y, box)) {
            result[count] = box;

            count += 1;
            if (n <= count)
                break;
        }
    }

    return count;
}

size_t k_collision_query_line(struct k_collision_box **result, size_t n, int group_id, float x1, float y1, float x2, float y2) {

    if (NULL == result || 0 == n)
        return 0;

    struct k_collision_group *group = k__collision_find_group(group_id);
    if (NULL == group)
        return 0;

    size_t count = 0;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        if (check_collision_line_box(x1, y1, x2, y2, box)) {
            result[count] = box;

            count += 1;
            if (n <= count)
                break;
        }
    }

    return count;
}

size_t k_collision_query_rect(struct k_collision_box **result, size_t n, int group_id, float x1, float y1, float x2, float y2) {

    if (NULL == result || 0 == n)
        return 0;

    struct k_collision_group *group = k__collision_find_group(group_id);
    if (NULL == group)
        return 0;

    size_t count = 0;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        if (check_collision_rect_box(x1, y1, x2, y2, box)) {
            result[count] = box;

            count += 1;
            if (n <= count)
                break;
        }
    }

    return count;
}

size_t k_collision_query_circle(struct k_collision_box **result, size_t n, int group_id, float cx, float cy, float r) {

    if (NULL == result || 0 == n)
        return 0;

    struct k_collision_group *group = k__collision_find_group(group_id);
    if (NULL == group)
        return 0;

    size_t count = 0;

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        if (check_collision_circle_box(cx, cy, r, box)) {
            result[count] = box;

            count += 1;
            if (n <= count)
                break;
        }
    }

    return count;
}

/* endregion */
