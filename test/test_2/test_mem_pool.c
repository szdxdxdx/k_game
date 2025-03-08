#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "k_mem_pool.h"


static size_t count_line_num = 0;

#define print_h(fmt, ...) printf("     | \n"\
                                 "-----+ " fmt " :\n"\
                                 "     | \n", ##__VA_ARGS__)
#define print_l(fmt, ...) printf("     | " fmt "\n", ##__VA_ARGS__)
#define print_o(fmt, ...) printf("  <> | " fmt "\n", ##__VA_ARGS__)
#define print_e(fmt, ...) printf(" ERROR " fmt "\n", ##__VA_ARGS__)
#define print_c(fmt, ...) printf("%4zu | " fmt "\n", (count_line_num++, count_line_num%=10000), ##__VA_ARGS__)

#define scanf_o(fmt, ...) printf("input> "); scanf(fmt, ##__VA_ARGS__)

static size_t count_malloc   = 0;

static void *my_malloc(size_t size) {

#ifdef K_MALLOC_RANDOM_FAIL
    if (0 == rand() % (K_MALLOC_RANDOM_FAIL)) {
        print_e("malloc( %zu ) failed", size);
        return NULL;
    }
#endif

    void *p = malloc(size);
    if (NULL != p)
        print_o("malloc( %zu ) -> 0x%p, count: %zu", size, p, ++count_malloc);
    else
        print_e("malloc( %zu ) failed", size);

    return p;
}

static void my_free(void *ptr) {

    if (NULL != ptr)
        print_o("free( 0x%p ), count: %zu", ptr, --count_malloc);

    free(ptr);
}

static void test_1(void) {

    static const char s[] =
        "___4___8__12__16__20__24__28__32__36__40__44__48__52__56__60__64"
        "__68__72__76__80__84__88__92__96_100_104_108_112_116_120_124_128"
        "_132_136_140_144_148_152_156_160_164_168_172_176_180_184_188_192"
        "_196_200_204_208_212_216_220_224_228_232_236_240_244_248_252_256"
        "_260_264_268_272_276_280_284_288_292_296_300_304_308_312_316_320"
        "_324_328_332_336_340_344_348_352_356_360_364_368_372_376_380_384"
        "_388_392_396_400_404_408_412_416_420_424_428_432_436_440_444_448"
        "_452_456_460_464_468_472_476_480_484_488_492_496_500_504_508_512"
        "_516_520_524_528_532_536_540_544_548_552_556_560_564_568_572_576"
        "_580_584_588_592_596_600_604_608_612_616_620_624_628_632_636_640";

    void *p[50] = { NULL };

    srand(time(NULL));

    print_h("create mem_pool");

    struct k_mem_pool_config config = {
        .fn_malloc        = my_malloc,
        .fn_free          = my_free,
        .alloc_size_align = 32,
        .block_size_max   = 512,
        .alloc_chunk_size = 4096
    };
    struct k_mem_pool *pool = k_mem_pool_create(&config);
    if (NULL == pool)
        return;

    print_h("pool_alloc() and pool_free()");

    size_t loop = 1;
    for (; loop < 2000; loop++) {

        size_t i = rand() % (sizeof(p) / sizeof(p[0]));

        if (NULL != p[i]) {
            print_c("pool_free( %p )", p[i]);

            k_mem_pool_free(pool, p[i]);
            p[i] = NULL;
        }
        else {
            size_t str_len = rand() % sizeof(s);
            print_c("pool_alloc( %zu )", str_len + 1);

            p[i] = k_mem_pool_alloc(pool, str_len + 1);
            if (NULL == p[i])
                goto next_loop;

            strncpy(p[i], s, str_len);
            ((char *)(p[i]))[str_len] = '\0';

            print_l("pool_alloc -> 0x%p", p[i]);
            print_l("str = \"%s\"", (char *)p[i]);
        }

    next_loop:
        print_l("");
    }

    print_h("free all blocks");

    size_t i = 0;
    for (; i < sizeof(p) / sizeof(p[0]); i++)
        k_mem_pool_free(pool, p[i]);

    print_h("destroy mem_pool");

    k_mem_pool_destroy(pool);
}

static void test_2(void) {

    static const char s[] =
        "___4___8__12__16__20__24__28__32__36__40__44__48__52__56__60__64"
        "__68__72__76__80__84__88__92__96_100_104_108_112_116_120_124_128"
        "_132_136_140_144_148_152_156_160_164_168_172_176_180_184_188_192"
        "_196_200_204_208_212_216_220_224_228_232_236_240_244_248_252_256"
        "_260_264_268_272_276_280_284_288_292_296_300_304_308_312_316_320"
        "_324_328_332_336_340_344_348_352_356_360_364_368_372_376_380_384"
        "_388_392_396_400_404_408_412_416_420_424_428_432_436_440_444_448"
        "_452_456_460_464_468_472_476_480_484_488_492_496_500_504_508_512"
        "_516_520_524_528_532_536_540_544_548_552_556_560_564_568_572_576"
        "_580_584_588_592_596_600_604_608_612_616_620_624_628_632_636_640";

    void *p[50] = { NULL };

    srand(time(NULL));

    print_h("create mem_pool");

    struct k_mem_pool pool_in_place;
    struct k_mem_pool_config config = {
        .fn_malloc        = my_malloc,
        .fn_free          = my_free,
        .alloc_size_align = 32,
        .block_size_max   = 512,
        .alloc_chunk_size = 4096
    };
    struct k_mem_pool *pool = k_mem_pool_construct(&pool_in_place, &config);
    if (NULL == pool)
        return;

    print_h("pool_alloc() and pool_free()");

    size_t loop = 1;
    for (; loop < 2000; loop++) {

        size_t i = rand() % (sizeof(p) / sizeof(p[0]));

        if (NULL != p[i]) {
            print_c("pool_free( %p )", p[i]);

            k_mem_pool_free(pool, p[i]);
            p[i] = NULL;
        }
        else {
            size_t str_len = rand() % sizeof(s);
            print_c("pool_alloc( %zu )", str_len + 1);

            p[i] = k_mem_pool_alloc(pool, str_len + 1);
            if (NULL == p[i])
                goto next_loop;

            strncpy(p[i], s, str_len);
            ((char *)(p[i]))[str_len] = '\0';

            print_l("pool_alloc -> 0x%p", p[i]);
            print_l("str = \"%s\"", (char *)p[i]);
        }

    next_loop:
        print_l("");
    }

    print_h("free all blocks");

    size_t i = 0;
    for (; i < sizeof(p) / sizeof(p[0]); i++)
        k_mem_pool_free(pool, p[i]);

    print_h("destroy mem_pool");

    k_mem_pool_destroy(pool);
}

#if 1

int main(int argc, char **argv) {

    test_2();

    return 0;
}

#endif
