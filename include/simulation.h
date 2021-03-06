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

//#define RES_ALLDIFF


//simulation mode parameters 
uint8_t SIMULATION;


int sim_times;
int sim_start_idx;
float sim_error;
int  zipf_slen;
int  zipf_range;
int  zipf_offset;
float zipf_par;

extern


// for simulation variables 

// info_t

void import_inverse_cdf_table(uint16_t table_amount,uint16_t index,int integer);

void CreateStream(tree_t * container ,int length, float z,int range,int offset);

void Simulation_processing();

#endif