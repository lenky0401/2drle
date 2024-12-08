
#include <stdio.h>
#include <malloc.h>

#include "../common.h"
#include "2drle.h"

//行列转十进制莫顿码
int ij_2_morton_code(int i, int j)
{
    int morton_code = 0;
    //十进制Morton值由i和j中的二进制数字交叉结合得出
    for (int t = 0; i || j; t += 2)
    {
        //取出i和j的二进制最后一位数字，分别按位左移t+1位、t位，按位或
        morton_code |= (i & 1) << (t + 1) | (j & 1) << t;
        //i和j按位右移一位
        i >>= 1; j >>= 1;
    }
    return morton_code;
}

//十进制莫顿码转行列
void morton_code_2_ij(int morton_code, int *i, int *j)
{
    // i和j由Morton二进制交叉取出得到
    *i = *j = 0;
    for (int t = 0; morton_code; t++)
    {
        //取出Md二进制倒数第2位数字，按位右移1位，再按位左移t位，按位或
        *i |= (morton_code & 2) >> 1 << t;
        //取出Md二进制倒数第1位数字，按位左移t位，按位或
        *j |= (morton_code & 1) << t;
        //morton_code按位右移2位
        morton_code >>= 2;
    }
}

void morton_2drle_encode(Bmp24_Img_Info *img_info, void *img_data, char *outpath)
{
    int rows = img_info->bi.biHeight;
    int cols = img_info->bi.biWidth;
    assert(rows > 0);
    assert(cols > 0);

    char *pData = (char *)img_data;

    FILE *outFile;
    fopen_s(&outFile, outpath, "wb");
    if (outFile == NULL) {
        log("打开文件失败:%s", outpath);
        return;
    }

    //记录行数和列数
    fwrite(&rows, sizeof(rows), 1, outFile);
    fwrite(&cols, sizeof(cols), 1, outFile);

    uint32_t index = 0;
    char value = pData[0];

    //记录第0个值
    fwrite(&index, sizeof(index), 1, outFile);
    fwrite(&value, sizeof(value), 1, outFile);

    //从1到min2PowSq依次取出灰度值并与前一个进行比较
    int i, j;
    for (int k = 1; k <= ij_2_morton_code(rows - 1, cols - 1); k++)
    {
        morton_code_2_ij(k, &i, &j);

        //行列号不在图像范围内
        if (i >= rows || j >= cols) {
            continue;
        }

        char newValue = pData[i*cols + j];
        if (newValue == value) {
            continue;
        }

        index = k;
        value = newValue;

        fwrite(&index, sizeof(index), 1, outFile);
        fwrite(&value, sizeof(value), 1, outFile);
    }

    fclose(outFile);
}

void morton_2drle_decode(Bmp24_Img_Info *img_info, char *inpath, char *outpath)
{
    FILE* fp;
    fopen_s(&fp, inpath, "rb");
    if (fp == NULL) {
        log("打开文件失败:%s", inpath);
    }

    //读取图像行数和列数
    int rows, cols;
    fread(&rows, sizeof(rows), 1, fp);
    fread(&cols, sizeof(cols), 1, fp);

    //读取二位行程编码表中第一行
    unsigned int index = 0;
    char value = 0;
    fread(&index, sizeof(index), 1, fp);
    fread(&value, sizeof(value), 1, fp);

    //读取二位行程编码表中下一行
    unsigned int newIndex;
    char newValue;
    int i, j;
    while (fread(&newIndex, sizeof(newIndex), 1, fp) && fread(&newValue, sizeof(newValue), 1, fp))
    {

        // 将Md值在[index,newIndex)范围内的像素灰度值设为value*/
        for (int k = index; k < (int)newIndex; k++)
        {
            morton_code_2_ij(k, &i, &j);
            if (i >= rows || j >= cols) continue;
            //pData[i*cols + j] = value;
        }
        index = newIndex; value = newValue;
    } while (TRUE);

    /*将Md值在[index,min2PowSq)范围内的像素灰度值设为value*/
    for (int k = index; k <= ij_2_morton_code(rows - 1, cols - 1); k++)
    {
        morton_code_2_ij(k, &i, &j);
        if (i >= rows || j >= cols) continue;
        //pData[i*cols + j] = value;
    }

    fclose(fp);
}

void morton_2drle_test()
{
    Bmp24_Img_Info img;
    char *path = "testimg\\test3.bmp";
    char *img_data = (char *)read_bitmap_24(&img, path, FALSE);
    assert(img_data);

    char *outpath = "testimg\\test3_2drle_encode.data";
    morton_2drle_encode(&img, img_data, outpath);


    free(img_data);
}