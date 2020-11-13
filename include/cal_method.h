#include <stdio.h>
#include "Tree.h"
#ifndef _CAL_METHOD_H
#define _CAL_METHOD_H

uint8_t K_Value;

typedef struct Trace_info
{
    float entropy;
    uint32_t total_count;
    uint32_t distinct ;
}trace_info_t;

trace_info_t *exact(tree_t *item,uint32_t k_value);

#endif