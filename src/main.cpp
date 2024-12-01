// 2drle.cpp : 定义控制台应用程序的入口点。
//

#include "../stdafx.h"

#include <assert.h>

#include "rwbmp/rw_bmp24.h"
#include "scroll/check_scroll.h"

#include "spacescan/spacescan.h"

int main()
{
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

