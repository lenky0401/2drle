
#include <stdio.h>
#include <malloc.h>

#include "rle.h"

/**
参考：https://blog.csdn.net/zimuzi2019/article/details/106583064/

压缩前：[data] [data] [data] [data] [data] ...
压缩后：[data count] [data count] [data count] ...

*/

/*
IsRepetitionStart() 函数判断从 src 开始的数据是否是连续重复数据。根据算法要求，
只有数裾重复出现两次以上才算作连续重复数据，因此 IsRepetitionStart() 函数检査
连续的3字节是否是相同的数据，如果是则判定为出现连续重复数据。之所以要求至少要 
3 字节的重复数据才判定为连续重复数据，是为了尽量优化对短重复数据间隔出现时的压缩效率。
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
如果是连续重复数据，则调用 GetRepetitionCount() 函数计算出连续重复数据的长度，
将长度属性字节的最高位罝 1 并向输出缓冲区写入一个字节的重复数据。
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
如果不是连续重复数据，则调用 GetNonRepetitionCount() 函数计算连续非重复数据的长度，
将长度属性字节的极高位罝 0 并向输出缓冲区复制连续的多个非重复数据。
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
    unsigned char *src = inbuf; //src为输入指针
    int i;
    int encSize = 0;
    int srcLeft = inSize;       //srcleft为剩余数组元素
    while (srcLeft>0) {         //srcleft=0时代表输入结束
        int count = 0;
        if (IsrepetitionStart(src, srcLeft)) { //是否连续三个字节数据相同？
            if ((encSize + 2)>onuBufSize) {      //缓存数组太小时
                return -1;
            }
            count = GetRepetitionCount(src, srcLeft); //得到重复的个数
            outbuf[encSize++] = count | 0x80;          //写入输出数组
            outbuf[encSize++] = *src;
            src += count;                            //移动输入数组指针
            srcLeft -= count;
        }
        else {
            count = GetNonRepetitionCount(src, srcLeft); //得到不重复的个数
            if ((encSize + count + 1)>onuBufSize) { //缓存数组设置太小了
                return -1;
            }
            outbuf[encSize++] = count;
            for (i = 0; i<count; i++) {            //逐个写入这些数据
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
        if ((decSize + count)>onuBufSize) { //缓存数组设置太小了
            return -1;
        }
        if ((sign & 0x80) == 0x80) {          //连续重复数据标志
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