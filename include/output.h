#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>     /*------xiu------*/
#include "cal_method.h"
#include "trace_analysis.h"



char Trace_Path[200];
char Output_FileName[100];

char CSV[5];
FILE *Output_File; 
FILE *filelist[20];
uint16_t file_cnt;


void create_folder();

void NORfolder( char *Output_FolderName);

void Output_Init(char *Output_FolderName);

void output();

void output_entropy_csv_origin();

void output_entropy_csv_distinct();

void output_entropy_csv_total();

void Close_Output_File();




#endif
