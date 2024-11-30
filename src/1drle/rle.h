#ifndef _2DRLE_H_INCLUDED_
#define _2DRLE_H_INCLUDED_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif //_cplusplus

typedef struct {
    uint32_t data;
    uint32_t count;
} rle1d_data;

rle1d_data* rle1d_compress(uint32_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif //_cplusplus

#endif