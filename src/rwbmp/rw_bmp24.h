#ifndef _RW_BMP24_H_INCLUDED_
#define _RW_BMP24_H_INCLUDED_

#include <Windows.h>

typedef struct {
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
} Bmp24_Img_Info;

#ifdef __cplusplus
extern "C" {
#endif //_cplusplus

void* read_bitmap_24(Bmp24_Img_Info *img_info, char *img_path, BOOL convert32);
int save_bitmap_24(Bmp24_Img_Info *img_info, void *img_data, char *img_path);

#ifdef __cplusplus
}
#endif //_cplusplus

#endif