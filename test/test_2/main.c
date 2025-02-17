#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "k_str_map.h"

struct data_node {
    struct k_str_map_node map_node;
    int data;
};

void add(struct k_str_map *map, const char *str, int data) {
    struct data_node *node = malloc(sizeof(struct data_node));

    const char *key = strdup(str);
    if (NULL == key) {
        printf("strdup failed\n");
        return;
    }

    node->data = data;

    if (0 == k_str_map_add(map, key, &node->map_node)) {
        printf("add ok : { key=\"%s\", data=%d }\n", key, data);
    }
    else {
        printf("failed\n");
        free((void*)key);
    }
}

void del(struct k_str_map *map, const char *str) {
    struct k_str_map_node *map_node = k_str_map_get(map, str);
    if (NULL != map_node) {
        struct data_node *node = (struct data_node *)map_node;
        k_str_map_del(map, &node->map_node);
        printf("del ok : { key=\"%s\", data=%d }\n", str, node->data);
        free((void*)node->map_node.key);
        free(node);
    } else
        printf("failed\n");
}

void get(struct k_str_map *map, const char *str) {
    struct k_str_map_node *map_node = k_str_map_get(map, str);
    if (NULL != map_node) {
        struct data_node *node = (struct data_node *)map_node;
        printf("get ok : { key=\"%s\", data=%d }\n", str, node->data);
    }
    else
        printf("null\n");
}

void test(void) {

    struct k_hash_list *hash_lists = malloc(sizeof(struct k_hash_list) * 8);
    struct k_str_map map;
    k_str_map_init(&map, hash_lists, 8);

    char input[128];
    while (1) {
        printf("input cmd ( add, del, get or quit ): ");
        fflush(stdin);
        scanf("%s", input);
        if (0 == strcmp("add", input)) {

            char key[128];
            printf("input str key: ");
            scanf("%s", key);

            int data;
            printf("input int data: ");
            scanf("%d", &data);

            add(&map, key, data);
        }
        else if (0 == strcmp("del", input)) {

            char key[128];
            printf("input str key: ");
            scanf("%s", key);

            del(&map, key);
        }
        else if (0 == strcmp("get", input)) {

            char key[128];
            printf("input str key: ");
            scanf("%s", key);

            get(&map, key);
        }
        else if (0 == strcmp("rehash", input)) {

            int new_lists_num;
            printf("input int new_lists_num: ");
            scanf("%d", &new_lists_num);

            struct k_hash_list *new_hash_lists = malloc(sizeof(struct k_hash_list) * new_lists_num);

            struct k_hash_list *old_lists = k_str_map_rehash(&map, new_hash_lists, new_lists_num);
            free(old_lists);
        }
        else if (0 == strcmp("quit", input)) {
            break;
        }
        else {
            printf("unknown cmd\n");
        }
    }

    free(map.lists);
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    test();

    return 0;
}

#endif
