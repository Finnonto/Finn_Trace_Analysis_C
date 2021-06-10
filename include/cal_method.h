#ifndef _CAL_METHOD_H
#define _CAL_METHOD_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "Tree.h"
#include "mpdecimal.h"


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
int alg_cnt;
// variable resolution
int resolution;

/*

    for import table 

*/

uint16_t it;   //inverse table amount
int rand_tmp;

uint32_t K_Value;
// for invers cdf
#define MAX_TABLE 2000
#define MAX_ENTRY 65536
uint32_t Table_Size; 
uint16_t Table_Amount;
uint32_t TableIndex;
int TableINT;

// define hash parameter array 
int Change_Hash;
uint32_t* para_a;
uint32_t* para_c;
int  hashseed;
// change origin clifford srand
int CCliffordSeed;
// change origin Pingli srand
int CPingLiSeed;


// CDF parameters
uint32_t IC_Entry;
uint32_t CDF_Table_Entry_list[MAX_TABLE];
//
typedef struct ICTable_list
{
    double Table[MAX_ENTRY];
    uint32_t point[1000];
}ICTable_list;

ICTable_list Inverse_table[MAX_TABLE];

// CDF parallel m hash parameter

int m_hash;


//CDF Stage 50 storage 
uint32_t CDF_Stage50_Table_Entry_list[MAX_TABLE];
ICTable_list Inverse_Stage50_table[MAX_TABLE];
ICTable_list Inverse_Stage50_point[MAX_TABLE];

uint32_t CDF_Stage100_Table_Entry_list[MAX_TABLE];
ICTable_list Inverse_Stage100_table[MAX_TABLE];
ICTable_list Inverse_Stage100_point[MAX_TABLE];

// CDF optimize storage 
typedef struct ICoptTable_list
{
    int Table[65536];
    uint32_t point[1000];
}ICoptTable_list;


uint32_t CDF_opt_Table_Entry_list[MAX_TABLE];
ICoptTable_list Inverse_opt_table[MAX_TABLE];




//for head and tail 
int Head_Value[2000];
uint32_t Stage_Point[2000];
uint32_t HT_Table_Entry ;

// pingli

mpd_context_t ctx;
double pingli_alpha;
//parallel interpolation
int interpolation_threshold; // bit num
int interpolation_threshold2; // bit num
int interpolation_span;
// entropy calculate storage 
void hash_para_gen(int seed,uint32_t* para_a,uint32_t* para_c,int size);

double cal_MAPE(double* exact,double* est,int index,int entry);

extern void import_inverse_cdf_table(uint16_t table_amount,uint16_t index,int integer);

void import_inverse_cdf_stage50_table(uint16_t table_amount,int num);

void import_inverse_cdf_stage100_table(uint16_t table_amount,int num);

void import_optimized_cdf_table(uint16_t table_amount);

void import_inverse_cdf_single_table(uint32_t index);

void import_HeadTail_table();



int exact(tree_t *item,trace_info_t *info);

int Clifford_est(tree_t *item,trace_info_t *info);

int Clifford_cdf_est(tree_t *item,trace_info_t *info);

int Clifford_cdf_parallel_est(tree_t *item,trace_info_t *info);

int Clifford_cdf_parallel_interpolation_est(tree_t *item,trace_info_t *info);

int Clifford_cdf_parallel_interpolation_2th_est(tree_t *item,trace_info_t *info);

int Clifford_cdf_parallel_mhash_est(tree_t *item,trace_info_t *info);

int Clifford_cdf_stage50_est(tree_t *item,trace_info_t *info);

int Clifford_cdf_stage100_est(tree_t *item,trace_info_t *info);

int Clifford_cdf_opt_est(tree_t *item,trace_info_t *info);

int Clifford_HT_est(tree_t *item,trace_info_t *info);

int Clifford_HTo_est(tree_t *item,trace_info_t *info);

int Clifford_HTo_65536_est(tree_t *item,trace_info_t *info);

int Clifford_HTo_interpolation_est(tree_t *item,trace_info_t *info);

int Clifford_HTo_interpolation_65536_est(tree_t *item,trace_info_t *info);

int PingLi_est(tree_t *item,trace_info_t *info);


#endif