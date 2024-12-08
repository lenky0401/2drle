
#include <stdio.h>
#include <malloc.h>

#include "../common.h"
#include "rw_bmp24.h"

void* read_bitmap_24(Bmp24_Img_Info *img_info, char *img_path, BOOL convert32)
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

    int stride = (img_info->bi.biWidth * img_info->bi.biBitCount + 31) / 32 * 4;
    //BMP的文件格式中规定每行的字节数必须是4的整数倍，不是4的整数倍的要补零
    assert(img_info->bi.biSizeImage == (stride * img_info->bi.biHeight));

    size_t data_size = img_info->bi.biSizeImage;
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
    if (!convert32) {
        return img_data;
    }

    int stride32 = img_info->bi.biWidth * 4;
    char *img_data32 = (char *)malloc(stride32 * img_info->bi.biHeight);
    if (img_data32 == NULL) {
        log("申请内存失败:%zu", stride32 * img_info->bi.biHeight);
        return NULL;
    }

    for (int i = 0; i < img_info->bi.biHeight; i++) {
        for (int j = 0; j < img_info->bi.biWidth; j++) {
            char *pix24 = i * stride + j * 3;
            char *pix32 = i * stride32 + j * 4;

            pix32[0] = pix24[0];
            pix32[1] = pix24[1];
            pix32[2] = pix24[2];
            pix32[3] = 255;
        }
    }

    free(img_data);
    return img_data32;
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
