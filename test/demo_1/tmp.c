#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static size_t next_prime(size_t n) {

    static const size_t primes[] = {
        53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,
        49157, 98317, 196613, 393241, 786433, 1572869, 3145739,
        6291469, 12582917, 25165843, 50331653, 100663319,
        201326611, 402653189, 805306457, 1610612741
    };

    size_t end = sizeof(primes) / sizeof(primes[0]);

    size_t left = 0;
    size_t right = end;

    while (left < right) {
        size_t mid = (left + right) / 2;
        if (primes[mid] <= n)
            left = mid + 1;
        else
            right = mid;
    }

    if (right < end)
        return primes[right];
    else
        return primes[end - 1];
}

static void test(void) {

    size_t nums[] = {(1610612742),
        (0), (6), 53, (54), 97, (98),
        193, 389, 769, 1543, 3079, 6151, 12289, 24593,
        49157, 98317, 196613, 393241, 786433, (786434), 1572869, 3145739,
        6291469, 12582917, 25165843, 50331653, 100663319,
        201326611, 402653189, 805306457, 1610612741, (1610612742)
    };

    size_t i;
    for (i = 0; i < sizeof(nums) / sizeof(nums[0]); ++i) {
        printf("i=%zu, next_prime=%zu\n", nums[i], next_prime(nums[i]));
    }
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
