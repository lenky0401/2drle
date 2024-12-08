
#include <stdio.h>
#include <malloc.h>

#include "../common.h"
#include "2drle.h"

//����תʮ����Ī����
int ij_2_morton_code(int i, int j)
{
    int morton_code = 0;
    //ʮ����Mortonֵ��i��j�еĶ��������ֽ����ϵó�
    for (int t = 0; i || j; t += 2)
    {
        //ȡ��i��j�Ķ��������һλ���֣��ֱ�λ����t+1λ��tλ����λ��
        morton_code |= (i & 1) << (t + 1) | (j & 1) << t;
        //i��j��λ����һλ
        i >>= 1; j >>= 1;
    }
    return morton_code;
}

//ʮ����Ī����ת����
void morton_code_2_ij(int morton_code, int *i, int *j)
{
    // i��j��Morton�����ƽ���ȡ���õ�
    *i = *j = 0;
    for (int t = 0; morton_code; t++)
    {
        //ȡ��Md�����Ƶ�����2λ���֣���λ����1λ���ٰ�λ����tλ����λ��
        *i |= (morton_code & 2) >> 1 << t;
        //ȡ��Md�����Ƶ�����1λ���֣���λ����tλ����λ��
        *j |= (morton_code & 1) << t;
        //morton_code��λ����2λ
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
        log("���ļ�ʧ��:%s", outpath);
        return;
    }

    //��¼����������
    fwrite(&rows, sizeof(rows), 1, outFile);
    fwrite(&cols, sizeof(cols), 1, outFile);

    uint32_t index = 0;
    char value = pData[0];

    //��¼��0��ֵ
    fwrite(&index, sizeof(index), 1, outFile);
    fwrite(&value, sizeof(value), 1, outFile);

    //��1��min2PowSq����ȡ���Ҷ�ֵ����ǰһ�����бȽ�
    int i, j;
    for (int k = 1; k <= ij_2_morton_code(rows - 1, cols - 1); k++)
    {
        morton_code_2_ij(k, &i, &j);

        //���кŲ���ͼ��Χ��
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
        log("���ļ�ʧ��:%s", inpath);
    }

    //��ȡͼ������������
    int rows, cols;
    fread(&rows, sizeof(rows), 1, fp);
    fread(&cols, sizeof(cols), 1, fp);

    //��ȡ��λ�г̱�����е�һ��
    unsigned int index = 0;
    char value = 0;
    fread(&index, sizeof(index), 1, fp);
    fread(&value, sizeof(value), 1, fp);

    //��ȡ��λ�г̱��������һ��
    unsigned int newIndex;
    char newValue;
    int i, j;
    while (fread(&newIndex, sizeof(newIndex), 1, fp) && fread(&newValue, sizeof(newValue), 1, fp))
    {

        // ��Mdֵ��[index,newIndex)��Χ�ڵ����ػҶ�ֵ��Ϊvalue*/
        for (int k = index; k < (int)newIndex; k++)
        {
            morton_code_2_ij(k, &i, &j);
            if (i >= rows || j >= cols) continue;
            //pData[i*cols + j] = value;
        }
        index = newIndex; value = newValue;
    } while (TRUE);

    /*��Mdֵ��[index,min2PowSq)��Χ�ڵ����ػҶ�ֵ��Ϊvalue*/
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