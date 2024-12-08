
#include <stdio.h>
#include <malloc.h>

#include "../common.h"
#include "2drle.h"

typedef struct {
    int morton;//morton码
    int grey;//灰度值
} DLR;

int rank2morton(int i, int j)//根据行列号计算morton码
{
    int morton = 0;
    for (int x = 0; x < sizeof(unsigned short) * CHAR_BIT; x++)	//行列号计算 
        morton |= (i & 1U << x) << (x + 1) | (j & 1U << x) << x;
    return morton;
}



int img_process(Bmp24_Img_Info *img_info, void *img_data, char *outpath)
{
    unsigned char *pImg = img_data;
    int width = img_info->bi.biWidth;
    int height = img_info->bi.biHeight;
    DLR *array = malloc(width*height*sizeof(DLR));//array数组存储每个像素的morton码和像素值
    DLR list[100000];//list线性表即存储不同大小块的属性
    list[0].morton = 0;
    list[0].grey = pImg[0];
    int n = 0, m = 1;//m表示线性表的长度
    int morton;
    for (int i = 0; i<height; i++)
    {
        for (int j = 0; j<width; j++)
        {
            array[n].morton = rank2morton(i, j);
            array[n++].grey = pImg[i*width + j];
        }
    }
    for (int i = 0; i<n; i++)//冒泡排序，将线性表按morton码的大小从小到大排序
    {
        for (int j = i + 1; j<n; j++)
        {
            if (array[j].morton<array[i].morton)
            {
                DLR w = array[i];
                array[i] = array[j];
                array[j] = w;
            }
        }
    }

    for (int i = 0; i<n - 1; i++)//属性相同的合并，不同的记录在线性表中
    {
        if (array[i + 1].grey != array[i].grey)
        {
            list[m].grey = array[i + 1].grey>127 ? 1 : 0;
            list[m].morton = array[i + 1].morton;
            m++;
        }
    }
    list[m].grey = pImg[height*width - 1];//最后一个像素的morton码要写入
    list[m++].morton = rank2morton(height - 1, width - 1);

    printf("压缩成功！\n");

    return 0;

}