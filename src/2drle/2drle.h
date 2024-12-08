#ifndef _2DRLE_H_INCLUDED_
#define _2DRLE_H_INCLUDED_

#include "../rwbmp/rw_bmp24.h"

#ifdef __cplusplus
extern "C" {
#endif //_cplusplus

    void morton_2drle_test();

    void morton_2drle_encode(Bmp24_Img_Info *img_info, void *img_data, char *outpath);

#ifdef __cplusplus
}
#endif //_cplusplus

#endif