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


//flag
uint8_t EXACT;  //algorithm flag
uint8_t CLIFFORD;  //algorithm flag
uint8_t INVERSE_CLI;  //algorithm flag

uint8_t ORIGIN;  //normalization flag
uint8_t TOTAL;  //normalization flag
uint8_t DISTINCT;  //normalization flag

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
uint32_t Table_Entry_list[10];
//
typedef struct ICTable_list
{
    double Table[65536];
    uint32_t point[1000];
}ICTable_list;

ICTable_list Inverse_table[10];

ICTable_list Inverse_Stage_table[10];
ICTable_list Inverse_Stage_point[10];

//for Clifford_est srand deviate
uint32_t Deviation;
uint32_t TableIndex;

//for head and tail 
double Head_Value[2000];
uint32_t Stage_Point[2000];
uint32_t HT_Table_Entry ;


void import_inverse_cdf_table(uint16_t table_amount);

void import_inverse_cdf_stage_table(uint16_t table_amount);

void import_inverse_cdf_single_table(uint32_t index);

void import_HeadTail_table();

trace_info_t *exact(tree_t *item);

trace_info_t *Clifford_est(tree_t *item);

trace_info_t *Clifford_cdf_est(tree_t *item);

trace_info_t *Clifford_cdf_stage_est(tree_t *item);

trace_info_t *Clifford_HT_est(tree_t *item);

trace_info_t *Clifford_HTo_est(tree_t *item);

#endif