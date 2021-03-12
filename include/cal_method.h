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

#define MAX_ALG 12

typedef struct Trace_info
{
    double entropy;
    uint32_t total_count;
    uint32_t distinct ;
}trace_info_t;


//flag
int ALG_flag[MAX_ALG];


/*

    for import table 

*/

uint16_t it;   //inverse table amount
int rand_tmp;

uint32_t K_Value;
// for invers cdf
uint32_t Table_Size; 
uint16_t Table_Amount;
uint32_t IC_Entry;
uint32_t CDF_Table_Entry_list[10];
//
typedef struct ICTable_list
{
    double Table[65536];
    uint32_t point[1000];
}ICTable_list;

ICTable_list Inverse_table[10];


uint32_t CDF_Stage50_Table_Entry_list[10];
ICTable_list Inverse_Stage50_table[10];
ICTable_list Inverse_Stage50_point[10];

uint32_t CDF_Stage100_Table_Entry_list[10];
ICTable_list Inverse_Stage100_table[10];
ICTable_list Inverse_Stage100_point[10];

typedef struct ICoptTable_list
{
    int Table[65536];
    uint32_t point[1000];
}ICoptTable_list;


uint32_t CDF_opt_Table_Entry_list[10];
ICoptTable_list Inverse_opt_table[10];



//for Clifford_est srand deviate
uint32_t Deviation;
uint32_t TableIndex;

//for head and tail 
int Head_Value[2000];
uint32_t Stage_Point[2000];
uint32_t HT_Table_Entry ;





void import_inverse_cdf_table(uint16_t table_amount);

void import_inverse_cdf_stage50_table(uint16_t table_amount,int num);

void import_inverse_cdf_stage100_table(uint16_t table_amount,int num);

void import_optimized_cdf_table(uint16_t table_amount);

void import_inverse_cdf_single_table(uint32_t index);

void import_HeadTail_table();

trace_info_t *exact(tree_t *item);

trace_info_t *Clifford_est(tree_t *item);

trace_info_t *Clifford_cdf_est(tree_t *item);

trace_info_t *Clifford_cdf_stage50_est(tree_t *item);

trace_info_t *Clifford_cdf_stage100_est(tree_t *item);

trace_info_t *Clifford_cdf_opt_est(tree_t *item);

trace_info_t *Clifford_HT_est(tree_t *item);

trace_info_t *Clifford_HTo_est(tree_t *item);

trace_info_t *Clifford_HTo_65536_est(tree_t *item);

trace_info_t *Clifford_HTo_interpolation_est(tree_t *item);

trace_info_t *Clifford_HTo_interpolation_65536_est(tree_t *item);

trace_info_t *PingLi_est(tree_t *item);


#endif