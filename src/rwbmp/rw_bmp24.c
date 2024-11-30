
#include <stdio.h>
#include <malloc.h>

#include "../common.h"
#include "rw_bmp24.h"

void* read_bitmap_24(Bmp24_Img_Info *img_info, char *img_path)
{
    FILE* fp;
    fopen_s(&fp, img_path, "rb");
    if (fp == NULL) {
        log("打开文件失败:%s", img_path);
        return NULL;
    }

    fread(&img_info->bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&img_info->bi, sizeof(BITMAPINFOHEADER), 1, fp);

    if (img_info->bi.biBitCount != 24) {
        log("不支持该格式的BMP位图:%s", img_path);
        return NULL;
    }

    fseek(fp, img_info->bf.bfOffBits, 0);

    size_t data_size = img_info->bi.biWidth * img_info->bi.biHeight * 3;
    char *img_data = (char *)malloc(data_size);
    if (img_data == NULL) {
        log("申请内存失败:%zu", data_size);
        return NULL;
    }

    size_t ret = fread(img_data, 1, data_size, fp);
    if (ret != data_size) {
        log("读取数据失败:%zu, %zu", ret, data_size);
        return NULL;
    }

    fclose(fp);
    return img_data;
}

int save_bitmap_24(Bmp24_Img_Info *img_info, void *img_data, char *img_path)
{
    FILE* fpw;
    fopen_s(&fpw, img_path, "wb");
    fwrite(&img_info->bf, sizeof(BITMAPFILEHEADER), 1, fpw);
    fwrite(&img_info->bi, sizeof(BITMAPINFOHEADER), 1, fpw);

    size_t data_size = img_info->bi.biWidth * img_info->bi.biHeight * 3;
    size_t ret = fwrite(img_data, 1, data_size, fpw);
    if (ret != data_size) {
        log("写入数据失败:%zu, %zu", ret, data_size);
        return -1;
    }

    fclose(fpw);
    return 0;
}
