// 2drle.cpp : 定义控制台应用程序的入口点。
//

#include "../stdafx.h"

#include <assert.h>

#include "rwbmp/rw_bmp24.h"

int main()
{
    Bmp24_Img_Info img;
    char *path = "testimg\\test1.bmp";
    char *save_path = "testimg\\test2.bmp";
    void *img_data = read_bitmap_24(&img, path);
    assert(img_data);

    int ret = save_bitmap_24(&img, img_data, save_path);
    assert(ret == 0);

    free(img_data);
    getchar();
    return 0;
}

