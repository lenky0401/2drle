// 2drle.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "../stdafx.h"

#include <assert.h>

#include "rwbmp/rw_bmp24.h"
#include "scroll/check_scroll.h"

int main()
{
    Bmp24_Img_Info img;
    char *path = "testimg\\test2.bmp";
    char *img_data = (char *)read_bitmap_24(&img, path);
    assert(img_data);

    

    free(img_data);

    getchar();
    return 0;
}

