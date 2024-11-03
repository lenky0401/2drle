
#include <stdio.h>
#include <malloc.h>

#include "../common.h"
#include "check_scroll.h"

#define def_image_id_block_w (128)
#define def_image_id_block_h (64)

uint32_t g_last_image_row_id[SCREE_HEIGHT / 2] = { 0 };
uint32_t g_last_image_block_id[SCREE_HEIGHT / 2] = { 0 };

static uint32_t calc_image_row_id(uint32_t *data, int len)
{
    uint32_t mul_x = PRIME_X;
    uint32_t ret_value = 0;

    for (int x = 0; x < len; x++) {
        ret_value += data[x] * mul_x;
        mul_x *= PRIME_X;
    }

    return ret_value;
}

static void print_last_image_id(int offset)
{
    for (int y = offset; y < SCREE_HEIGHT / 2; y++) {
        log("%u\n", g_last_image_block_id[y]);
    }

    log("=================\n");
}

void calc_image_col_id(Bmp24_Img_Info *img_info, void *img_data)
{
    int start = (SCREE_HEIGHT / 4);
    int end = start + (SCREE_HEIGHT / 2);

    for (int y = 0; y < (end - start); y++)
    {
        uint32_t *data = (uint32_t *)img_data + 
            (start + y) * img_info->bi.biWidth + SCREE_WIDTH / 3;

        g_last_image_row_id[y] = calc_image_row_id(data, def_image_id_block_w);
    }

#if 0
    uint32_t mul_y = PRIME_Y;
    uint32_t ret_value = 0;

    assert(end - start >= def_image_id_block_h);

    for (int y = 0; y < def_image_id_block_h; y++)
    {
        ret_value += g_last_image_row_id[y] * mul_y;
        mul_y *= PRIME_Y;
    }

    for (int y = def_image_id_block_h; y < (end - start); y++)
    {
        ret_value += g_last_image_row_id[y] * mul_y;


        g_last_image_block_id[y] = ret_value;
        mul_y *= PRIME_Y;
    }

#else

    


    for (int y = 0; y < (end - start) - def_image_id_block_h; y++)
    {

        uint32_t mul_y = PRIME_Y;
        uint32_t ret_value = 0;

        for (int y1 = y; y1 < y + def_image_id_block_h; y1++)
        {
            ret_value += g_last_image_row_id[y1] * mul_y;
            mul_y *= PRIME_Y;

        }

        g_last_image_block_id[y] = ret_value;

    }


#endif

    print_last_image_id(0);
}