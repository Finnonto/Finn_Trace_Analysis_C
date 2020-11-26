
#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cal_method.h"
#include "trace_analysis.h"




char Ouput_FileName[100];
char CSV[5];
FILE *Output_File; 

void Output_Init();

void output_entropy_csv(trace_info_t *info ,uint32_t ct );

void Close_Output_File();



#endif
