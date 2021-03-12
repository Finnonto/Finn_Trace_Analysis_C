#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <signal.h>

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



// for simulation variables 

// info_t
trace_info_t *exact_info;
trace_info_t *Clifford_info;
trace_info_t *Clifford_cdf_info;
trace_info_t *Clifford_cdf_stage50_info;
trace_info_t *Clifford_cdf_stage100_info;
trace_info_t *Clifford_cdf_opt_info;
trace_info_t *Clifford_HT_info;
trace_info_t *Clifford_HTo_info;
trace_info_t *Clifford_HTo_65536_info;
trace_info_t *Clifford_HTo_interpolation_info;
trace_info_t *Clifford_HTo_interpolation_65536_info;
trace_info_t *PingLi_info;


// for result storage
int    Distinct[600];
double exact_entropy[600];
double Clifford_entropy[600];
double Clifford_cdf_entropy[600];
double Clifford_cdf_stage50_entropy[600];
double Clifford_cdf_stage100_entropy[600];
double Clifford_cdf_opt_entropy[600];
double Clifford_HT_entropy[600];
double Clifford_HTo_entropy[600];
double Clifford_HTo_65536_entropy[600];
double Clifford_HTo_interpolation_entropy[600];
double Clifford_HTo_interpolation_65536_entropy[600];
double PingLi_entropy[600];



void CreateStream(tree_t * container ,int length, float z,int range,int offset);

void Simulation_processing();

#endif