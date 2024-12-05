// 2drle.cpp : 定义控制台应用程序的入口点。
//

#include "../stdafx.h"

#include <assert.h>

#include "rwbmp/rw_bmp24.h"
#include "scroll/check_scroll.h"

#include "spacescan/spacescan.h"

#include "lz/lz.h"

int main()
{
    uint8_t in_data[8] = { 'A', 'A', 'B', 'C', 'B', 'C', 'A', 'B', };
    uint32_t in_data_len = 8;
    uint8_t out_data[64];
    uint32_t out_data_len = 64;

    int r = lz77_encode_print(in_data, in_data_len, out_data, &out_data_len);


    Bmp24_Img_Info img;
    char *path = "testimg\\test2.bmp";
    char *img_data = (char *)read_bitmap_24(&img, path);
    assert(img_data);

    

    free(img_data);

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

    getchar();
    return 0;
}

