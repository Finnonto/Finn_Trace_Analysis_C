#ifndef _CAL_METHOD_H
#define _CAL_METHOD_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "Tree.h"


#ifndef PI
    #define PI acos(-1)
#endif


typedef struct Trace_info
{
    double entropy;
    uint32_t total_count;
    uint32_t distinct ;
}trace_info_t;


//uint32_t K_Value;   /*------xiu-----*/
uint16_t it;   //xiu

int rand_tmp;


trace_info_t *exact(tree_t *item);

trace_info_t *Clifford_est(tree_t *item);
#endif