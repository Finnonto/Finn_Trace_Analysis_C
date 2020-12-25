#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "Tree.h"
#include "cal_method.h"
#include "output.h"
#include "prng.h"
#include "massdal.h"



//simulation mode parameters 
uint8_t SIMULATION;
int m_KLD;
int sim_times;
float sim_error;
int  zipf_slen;
int  zipf_range;
int  zipf_offset;
float zipf_par;



void classify(double* classify_container,double* stream,int length,int range,int part);

double cross_entropy(double* a_classify,double* b_classify,int length);

double cal_KLD(double* exact_stream ,double* base_stream,double* target_stream,int length);

void CreateStream(tree_t * container ,int length, float z,int range,int offset);

void Simulation_processing();

#endif