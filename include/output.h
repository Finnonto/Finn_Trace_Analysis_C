#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>     /*------xiu------*/
#include "cal_method.h"
#include "trace_analysis.h"
#include "simulation.h"

#define ENT_MAX 3000
#define CHAR_LEN 200


char TraceName[CHAR_LEN];
char filename[CHAR_LEN];
char Trace_Path[CHAR_LEN];
char MAPE_filename[CHAR_LEN];
char Output_SrcIPName[CHAR_LEN];
char Output_DstIPName[CHAR_LEN];
char Output_SrcPortName[CHAR_LEN];
char Output_DstPortName[CHAR_LEN];
char Output_PktLenName[CHAR_LEN];
char Output_FolderName[CHAR_LEN];


//to count how many entries

int ent_cnt;

// for result storage
trace_info_t *entropy_info;
int    StreamDistinct[5][ENT_MAX];
int    StreamLength[5][ENT_MAX];
double MAPE[5][MAX_ALG];
double exact_entropy[5][ENT_MAX];
double Clifford_entropy[5][ENT_MAX];
double Clifford_cdf_entropy[5][ENT_MAX];
double Clifford_cdf_parallel_entropy[5][ENT_MAX];
double Clifford_cdf_stage50_entropy[5][ENT_MAX];
double Clifford_cdf_stage100_entropy[5][ENT_MAX];
double Clifford_cdf_opt_entropy[5][ENT_MAX];
double Clifford_HT_entropy[5][ENT_MAX];
double Clifford_HTo_entropy[5][ENT_MAX];
double Clifford_HTo_65536_entropy[5][ENT_MAX];
double Clifford_HTo_interpolation_entropy[5][ENT_MAX];
double Clifford_HTo_interpolation_65536_entropy[5][ENT_MAX];
double PingLi_entropy[5][ENT_MAX];


void create_folder();

void Output_write_file(char* Output_filename,int index);

void Output_Simulation();

void Output_Trace();

void Output_log();

void Output_MAPE();

#endif
