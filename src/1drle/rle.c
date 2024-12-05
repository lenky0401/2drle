
#include <stdio.h>
#include <malloc.h>

#include "rle.h"

/**
�ο���https://blog.csdn.net/zimuzi2019/article/details/106583064/

ѹ��ǰ��[data] [data] [data] [data] [data] ...
ѹ����[data count] [data count] [data count] ...

*/

/*
IsRepetitionStart() �����жϴ� src ��ʼ�������Ƿ��������ظ����ݡ������㷨Ҫ��
ֻ�������ظ������������ϲ����������ظ����ݣ���� IsRepetitionStart() �������
������3�ֽ��Ƿ�����ͬ�����ݣ���������ж�Ϊ���������ظ����ݡ�֮����Ҫ������Ҫ 
3 �ֽڵ��ظ����ݲ��ж�Ϊ�����ظ����ݣ���Ϊ�˾����Ż��Զ��ظ����ݼ������ʱ��ѹ��Ч�ʡ�
*/
int IsrepetitionStart(unsigned char *src, int srcLeft) {
    if (srcLeft<3) {
        return 0;
    }
    if ((src[0] == src[1]) && (src[1] == src[2])) {
        return 1;
    }
    return 0;
}

/*
����������ظ����ݣ������ GetRepetitionCount() ��������������ظ����ݵĳ��ȣ�
�����������ֽڵ����λ�D 1 �������������д��һ���ֽڵ��ظ����ݡ�
*/
int  GetRepetitionCount(unsigned char *src, int srcLeft) {
    int repeatedbuf = src[0];
    int length = 1;
    while (length<srcLeft&&length<0x7f && src[length] == repeatedbuf) {
        length++;
    }
    return length;
}

/*
������������ظ����ݣ������ GetNonRepetitionCount() ���������������ظ����ݵĳ��ȣ�
�����������ֽڵļ���λ�D 0 ����������������������Ķ�����ظ����ݡ�
*/
int GetNonRepetitionCount(unsigned char *src, int srcLeft) {
    if (srcLeft<3) {
        return srcLeft;
    }
    int length = 2;
    int a = src[0], b = src[1];
    while (length<srcLeft&&length<0x7f && ((a != b) || (b != src[length]))) {
        a = b;
        b = src[length];
        length++;
    }
    return length;
}

int Rle_Encode(unsigned char *inbuf, int inSize, unsigned char *outbuf, int onuBufSize)
{
    unsigned char *src = inbuf; //srcΪ����ָ��
    int i;
    int encSize = 0;
    int srcLeft = inSize;       //srcleftΪʣ������Ԫ��
    while (srcLeft>0) {         //srcleft=0ʱ�����������
        int count = 0;
        if (IsrepetitionStart(src, srcLeft)) { //�Ƿ����������ֽ�������ͬ��
            if ((encSize + 2)>onuBufSize) {      //��������̫Сʱ
                return -1;
            }
            count = GetRepetitionCount(src, srcLeft); //�õ��ظ��ĸ���
            outbuf[encSize++] = count | 0x80;          //д���������
            outbuf[encSize++] = *src;
            src += count;                            //�ƶ���������ָ��
            srcLeft -= count;
        }
        else {
            count = GetNonRepetitionCount(src, srcLeft); //�õ����ظ��ĸ���
            if ((encSize + count + 1)>onuBufSize) { //������������̫С��
                return -1;
            }
            outbuf[encSize++] = count;
            for (i = 0; i<count; i++) {            //���д����Щ����
                outbuf[encSize++] = *src++;
            }
            srcLeft -= count;
        }
    }
    return encSize;
}

int Rle_Decode(unsigned char *inbuf, int inSize, unsigned char *outbuf, int onuBufSize) {
    unsigned char *src = inbuf;
    int i;
    int decSize = 0;
    int count = 0;
    while (src<(inbuf + inSize)) {
        unsigned char sign = *src++;
        int count = sign & 0x7F;
        if ((decSize + count)>onuBufSize) { //������������̫С��
            return -1;
        }
        if ((sign & 0x80) == 0x80) {          //�����ظ����ݱ�־
            for (i = 0; i<count; i++) {
                outbuf[decSize++] = *src;
            }
            src++;
        }
        else {
            for (i = 0; i<count; i++) {
                outbuf[decSize++] = *src++;
            }
        }
    }
    return decSize;
}


rle1d_data* rle1d_compress(uint32_t *data, uint32_t len)
{
    return NULL;
}