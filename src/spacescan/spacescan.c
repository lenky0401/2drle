
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "spacescan.h"

/**
学习：https://cloud.tencent.com/developer/article/2215595

*/


static uint32_t get_max_bit_num(uint32_t a)
{
    uint32_t s = a * a;
    uint32_t n = 1;
    while (s = s / 10) {
        n++;
    }
    return n;
}

static int32_t is_power(int n) {
    if (n < 1)
        return -1;
    return ((n & (n - 1)) == 0) ? 0 : -1;
}

/**
参考：https://blog.csdn.net/cheche012345/article/details/127729403
*/
#define MORTON_CODE_MAX_BIT (16)
void morton_code_print(uint32_t a)
{
    if (a >= (1 << MORTON_CODE_MAX_BIT)) {
        printf("错误：超位数限制(%d)\n", a);
        return;
    }
    if (is_power(a) != 0) {
        printf("错误：非2的幂(%d)\n", a);
        return;
    }

    uint32_t n = get_max_bit_num(a) + 1;
    char fmt[16];
    memset(fmt, 0, sizeof(fmt));
    snprintf(fmt, sizeof(fmt), "%%%dd", n);

    for (int i = 0; i < a; i++) {
        for (int j = 0; j < a; j++) {

            int code = 0;
            for (int k = 0; k < MORTON_CODE_MAX_BIT; k++) {
                code |= ((i >> k) & 1) << (2 * k + 1);
                code |= ((j >> k) & 1) << (2 * k);
            }
            printf(fmt, code);

        }
        printf("\n");
    }
    printf("\n");
}

void morton_code_test()
{
    morton_code_print(0);
    morton_code_print(1);
    morton_code_print(2);
    morton_code_print(3);
    morton_code_print(4);
    morton_code_print(5);
    morton_code_print(6);
    morton_code_print(8);
    morton_code_print(10);
    morton_code_print(16);
}