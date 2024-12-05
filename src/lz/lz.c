
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#include "../common.h"
#include "lz.h"

//https://www.cnblogs.com/lab601/p/14206346.html
//https://people.freedesktop.org/~teuf/spice-doc/html/ch01s02.html
//https://blog.csdn.net/smartvxworks/article/details/132662149
//https://www.zhihu.com/question/30112322/answer/3358054159

#define SEARCH_WINDOW_MAX_SIZE 1024 // 窗口大小

static void search_match(uint8_t *in_data, uint32_t curt_pos, uint32_t in_data_len,
    uint32_t *offset, uint32_t *len)
{
    assert(curt_pos<in_data_len);

    int32_t search_left = 0;
    if (curt_pos > SEARCH_WINDOW_MAX_SIZE) {
        search_left = curt_pos - SEARCH_WINDOW_MAX_SIZE;
    }

    //转化问题：在str1中查找最长的str2前缀子串
    //str1：search_left ... curt_pos
    //str2: curt_pos ... in_data_len

    //暴力查找：其他优化方法，比如KMP？
    //https://blog.csdn.net/m0_63488627/article/details/130197908
    int curt_len;
    *offset = *len = 0;
    //在搜索窗口里从右边往左边移动查找，便于后续的哈夫曼编码
    for (int i = curt_pos - 1; i >= search_left; i--) {

        int j = curt_pos;
        int s = i;
        curt_len = 0;

        do {
            if (in_data[s] != in_data[j]) {
                break;
            }
            curt_len++;
            s++;
            j++;

        } while (s < curt_pos && j < in_data_len);

        if (curt_len > *len) {
            *len = curt_len;
            *offset = curt_pos - i;
        }
    }
}


int lz77_encode_print(uint8_t *in_data, uint32_t in_data_len, uint8_t *out_data, uint32_t *out_data_len)
{
    uint32_t offset = 0, len = 0;

    for (int i = 0; i < in_data_len;) {
        search_match(in_data, i, in_data_len, &offset, &len);
        assert(i + len <= in_data_len);
        if (i + len == in_data_len) {
            printf("offset: %d len: %d char: EOF\n", offset, len);
        }
        else {
            printf("offset: %d len: %d char: %c\n", offset, len, in_data[i + len]);
        }
        
        i += len + 1;
    }

    return 0;
}


void lz77_test()
{
    uint8_t in_data[8] = { 'A', 'A', 'B', 'C', 'B', 'C', 'A', 'B', };
    uint32_t in_data_len = 8;
    uint8_t out_data[64];
    uint32_t out_data_len = 64;

    int r = lz77_encode_print(in_data, in_data_len, out_data, &out_data_len);
}