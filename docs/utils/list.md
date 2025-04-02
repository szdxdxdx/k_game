
## 结点自环

```C
/* 以下为完整的示例代码 */

#include <stdio.h>

#include "k_list.h"

struct my_data {

    struct k_list_node node;

    int data;
};

static void print_list(struct k_list *list) {

    struct my_data *data;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        data = container_of(iter, struct my_data, node);

        printf("%d ", data->data);
    }
    printf("\n");
}

#if 1

int main(void) {

    struct my_data nums[10] = {
        { .data = 0 }, { .data = 1 },
        { .data = 2 }, { .data = 3 },
        { .data = 4 }, { .data = 5 },
        { .data = 6 }, { .data = 7 },
        { .data = 8 }, { .data = 9 },
    };

    struct k_list list;
    k_list_init(&list);

    size_t i = 0;
    for (; i < 10; i++) {
        k_list_add_head(&list, &nums[i].node);
    }

    /* list = [ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 ] */
    print_list(&list);

    /* 遍历链表并删除部分结点 */
    struct my_data *data;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(&list, iter, next)) {
        data = container_of(iter, struct my_data, node);

        if (data->data % 3 != 0) {
            k_list_del(iter);

            /* 设置结点【自环】 */
            k_list_node_loop(iter);
        }
    }

    /* list = [ 9, 6, 3, 0 ] */
    print_list(&list);


    /* 注意：nums[5] 结点已从容器中删除，这里对该结点【再次执行删除】操作 */
    k_list_del(&nums[5].node);

    /* list = [ 9, 6, 3, 0 ] */
    print_list(&list);

    /* 对自环的结点多次执行删除操作不会影响容器。
     *
     * 若删去了上述对结点【自环】的代码，会发现第二次删除影响了容器。
     * list 内容将变为 [ 9, 6, 4, 3, 0 ]
     */

    return 0;
}

#endif
```
