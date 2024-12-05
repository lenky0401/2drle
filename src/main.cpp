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

    lz77_test();

    Bmp24_Img_Info img;
    char *path = "testimg\\test2.bmp";
    char *img_data = (char *)read_bitmap_24(&img, path);
    assert(img_data);

    

    free(img_data);

    morton_code_test();

    getchar();
    return 0;
}

