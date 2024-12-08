
#include <stdio.h>
#include <malloc.h>

#include "../common.h"
#include "2drle.h"

typedef struct {
    int morton;//morton��
    int grey;//�Ҷ�ֵ
} DLR;

int rank2morton(int i, int j)//�������кż���morton��
{
    int morton = 0;
    for (int x = 0; x < sizeof(unsigned short) * CHAR_BIT; x++)	//���кż��� 
        morton |= (i & 1U << x) << (x + 1) | (j & 1U << x) << x;
    return morton;
}



int img_process(Bmp24_Img_Info *img_info, void *img_data, char *outpath)
{
    unsigned char *pImg = img_data;
    int width = img_info->bi.biWidth;
    int height = img_info->bi.biHeight;
    DLR *array = malloc(width*height*sizeof(DLR));//array����洢ÿ�����ص�morton�������ֵ
    DLR list[100000];//list���Ա��洢��ͬ��С�������
    list[0].morton = 0;
    list[0].grey = pImg[0];
    int n = 0, m = 1;//m��ʾ���Ա�ĳ���
    int morton;
    for (int i = 0; i<height; i++)
    {
        for (int j = 0; j<width; j++)
        {
            array[n].morton = rank2morton(i, j);
            array[n++].grey = pImg[i*width + j];
        }
    }
    for (int i = 0; i<n; i++)//ð�����򣬽����Ա�morton��Ĵ�С��С��������
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

    for (int i = 0; i<n - 1; i++)//������ͬ�ĺϲ�����ͬ�ļ�¼�����Ա���
    {
        if (array[i + 1].grey != array[i].grey)
        {
            list[m].grey = array[i + 1].grey>127 ? 1 : 0;
            list[m].morton = array[i + 1].morton;
            m++;
        }
    }
    list[m].grey = pImg[height*width - 1];//���һ�����ص�morton��Ҫд��
    list[m++].morton = rank2morton(height - 1, width - 1);

    printf("ѹ���ɹ���\n");

    return 0;

}