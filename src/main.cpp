// 2drle.cpp : 定义控制台应用程序的入口点。
//

#include "../stdafx.h"

#include <assert.h>

#include "rwbmp/rw_bmp24.h"
#include "scroll/check_scroll.h"

int main()
{
    Bmp24_Img_Info img;
    char *path = "testimg\\test1.bmp";
    char *save_path = "testimg\\test2.bmp";
    void *img_data = read_bitmap_24(&img, path);
    assert(img_data);

    //int ret = save_bitmap_24(&img, img_data, save_path);
    //assert(ret == 0);

    calc_image_col_id(&img, img_data);

    Bmp24_Img_Info img2;
    char *path2 = "testimg\\test1a.bmp";
    void *img_data2 = read_bitmap_24(&img2, path2);
    assert(img_data2);

    calc_image_col_id(&img2, img_data2);

    free(img_data);
    free(img_data2);

    getchar();
    return 0;
}

