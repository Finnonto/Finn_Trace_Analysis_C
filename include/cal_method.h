#ifndef _CAL_METHOD_H
#define _CAL_METHOD_H

#include <stdio.h>
#include <Tree.h>

typedef struct Trace_info
{
    float entropy;
    uint32_t total_count;
    uint32_t distinct ;
}trace_info_t;


uint8_t K_Value;


trace_info_t *exact(tree_t *item,uint32_t k_value);

#endif