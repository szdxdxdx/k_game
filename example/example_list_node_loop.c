#include <stdio.h>

#include "k_hash_list.h"

struct my_data {
    struct k_hash_list_node node;
    int data;
};

static void print_list(struct k_hash_list *list) {
    struct k_hash_list_node *iter, *next;
    for (k_hash_list_for_each_s(list, iter, next)) {
        struct my_data *data = container_of(iter, struct my_data, node);
        printf("%d ", data->data);
    }
    printf("\n");
}

#if 1

int main(int argc, char **argv) {

    struct my_data nums[10] = {
        { .data = 0 }, { .data = 1 },
        { .data = 2 }, { .data = 3 },
        { .data = 4 }, { .data = 5 },
        { .data = 6 }, { .data = 7 },
        { .data = 8 }, { .data = 9 },
    };

    struct k_hash_list list;
    k_hash_list_init(&list);
    k_hash_list_add(&list, &nums[0].node);
    k_hash_list_add(&list, &nums[1].node);
    k_hash_list_add(&list, &nums[2].node);
    k_hash_list_add(&list, &nums[3].node);
    k_hash_list_add(&list, &nums[4].node);
    k_hash_list_add(&list, &nums[5].node);
    k_hash_list_add(&list, &nums[6].node);
    k_hash_list_add(&list, &nums[7].node);
    k_hash_list_add(&list, &nums[8].node);
    k_hash_list_add(&list, &nums[9].node);

    print_list(&list);

    /* ------------------------------------------------------------------------ */

    /* 遍历链表并删除部分结点 */
    struct k_hash_list_node *iter, *next;
    for (k_hash_list_for_each_s(&list, iter, next)) {
        struct my_data *data = container_of(iter, struct my_data, node);

        if (data->data % 3 != 0) {
            k_hash_list_del(iter);

            /* 若之后还可能对以删除结点再次执行删除操作，
             * 需要在第一次删除结点后设置结点自环。
             */
            k_hash_list_node_loop(iter);
        }
    }

    print_list(&list);

    /* ------------------------------------------------------------------------ */

    struct k_hash_list_node *nth_5 = &nums[5].node;

    /* nth_5 指向的结点已从容器中删除。这里对一个以删除的结点再次执行删除操作。
     *
     * 对自环的结点多次执行删除操作不会影响容器。
     * 如果你删去了上述设置结点自环的代码，会发现第二次删除影响了容器。
     */
    k_hash_list_del(nth_5);

    print_list(&list);

    return 0;
}

#endif
