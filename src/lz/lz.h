#ifndef _2DRLE_H_INCLUDED_
#define _2DRLE_H_INCLUDED_

#include "../rwbmp/rw_bmp24.h"

#ifdef __cplusplus
extern "C" {
#endif //_cplusplus

    int lz77_encode_print(uint8_t *in_data, uint32_t in_data_len, uint8_t *out_data, uint32_t *out_data_len);

#ifdef __cplusplus
}
#endif //_cplusplus

#endif