
# 分级内存池

示例代码：

```C
/* 以下为完整的示例代码 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "k_mem_pool.h"

static size_t count_malloc = 0;

static void *my_malloc(size_t size) {
    void *p = malloc(size);
    if (NULL != p) {
        printf("malloc( %zu ) -> 0x%p, count: %zu\n", size, p, ++count_malloc);
    } else {
        printf("malloc( %zu ) failed\n", size);
    }
    return p;
}

static void my_free(void *p) {
    if (NULL != p) {
        printf("free( 0x%p ), count: %zu\n", p, --count_malloc);
    }
    free(p);
}

int main(void) {
    int return_val = 0;

    /* 创建内存池 */
    struct k_mem_pool_config config;
    config.fn_malloc        = my_malloc;
    config.fn_free          = my_free;
    config.alloc_size_align = 8;
    config.block_size_max   = 128;
    config.alloc_chunk_size = 1024;
    struct k_mem_pool *pool = k_mem_pool_create(&config);
    if (NULL == pool) {
        printf("failed to create memory pool\n");
        return_val = -1;
        goto err;
    }

    /* 分配多个小内存块 */
    char *a = k_mem_pool_alloc(pool, 32);
    char *b = k_mem_pool_alloc(pool, 32);
    char *c = k_mem_pool_alloc(pool, 512);
    if (NULL == a || NULL == b || NULL == c) {
        printf("failed to alloc memory\n");
        return_val = -1;
        goto err;
    }

    int *num[50];
    int i;
    for (i = 0; i < 50; i++) {
        num[i] = k_mem_pool_alloc(pool, sizeof(int));
        if (NULL == num[i]) {
            printf("failed to alloc memory\n");
            return_val = -1;
            goto err;
        }
    }

    /* 使用这些内存块 */
    strcpy(a, "Hello");
    strcpy(b, "world");
    strcpy(c, "!");
    for (i = 0; i < 50; i++) {
        *(num[i]) = i;
    }

    printf("%s %s %s\n", a, b, c);
    for (i = 0; i < 50; i++) {
        printf("%d ", *(num[i]));
    }
    printf("\n");

    /* 归还内存块 */
    k_mem_pool_free(b);

err:
    /* 销毁内存池，所有未归还的块也会被一并释放 */
    k_mem_pool_destroy(pool);

    return return_val;
}
```

你可以把内存池的结构体嵌入到其他结构体中

```C
struct my_object {
    /* ... */

    struct k_mem_pool mem_pool; /* <- 把内存池的结构体嵌入到其他结构体中 */
};

struct my_object *my_object_create(void) {

    struct my_object *obj = NULL;
    struct k_mem_pool *pool = NULL;

    obj = malloc(sizeof(struct my_object));
    if (NULL == obj)
        goto err;
    
    {
        /* 构造内存池 */

        struct k_mem_pool_config config = ...;
        pool = k_mem_pool_construct(&obj->mem_pool, &config);
        if (NULL == pool)
            goto err;
    }
    
    /* ... */
    
    return obj;

err:
    /* ... */

    if (NULL != pool)
        k_mem_pool_destruct(pool);

    if (NULL != obj)
        free(obj);

    return NULL;
}
```
