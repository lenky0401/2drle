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

    uint8_t* read_bitmap24(Bmp24_Img_Info *img_info, char *img_path, BOOL convert32);
    uint32_t save_bitmap24(Bmp24_Img_Info *img_info, void *img_data, char *img_path);

    static uint32_t get_bitmap24_pix_value(uint8_t *ptr) {
        return (*(uint32_t *)ptr) & (0xffffff);
    }

    static uint32_t get_bitmap24_i_j_pix_value(uint8_t *img_data, uint32_t stride,
        uint32_t i, uint32_t j)
    {
        return get_bitmap24_pix_value(img_data + i * stride + j * 3);
    }

#ifdef __cplusplus
}
#endif //_cplusplus

#endif