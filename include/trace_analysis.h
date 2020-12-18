#ifndef _TRACE_ANALYSIS_H
#define _TRACE_ANALYSIS_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "libtrace.h"

#include "Tree.h"
#include "cal_method.h"
#include "output.h"
#include "prng.h"
#include "massdal.h"

//simulation mode parameters 
uint8_t SIMULATION;
int sim_times;
float sim_error;
int  zipf_slen;
int  zipf_range;
int  zipf_offset;
float zipf_par;



//trace analysis mode paramters 
uint8_t TRACE ;

uint32_t intervalTime ;
uint16_t it ;
uint16_t algorithm ;
uint16_t alg_cnt ;
uint16_t nor_cnt ;
uint16_t arg_tmp ;
uint16_t normalization ;
uint16_t output_num;
uint16_t file_cnt;
// simulation


// algorithm flags 
uint8_t EXACT ;
uint8_t CLIFFORD ;
uint8_t INVERSE_CLI ;
uint8_t ORIGIN ;
uint8_t TOTAL ;
uint8_t DISTINCT ;


// 5 tuple container for trace mode 
tree_t* SrcIP_tree;
tree_t* DesIP_tree;
tree_t* SrcPort_tree;
tree_t* DesPort_tree;
tree_t* PktLen_tree;
tree_t* Protocol_tree;



// Info wich store the result return by entropy
// calculation 
trace_info_t* SrcIP_info;
trace_info_t* DesIP_info;
trace_info_t* SrcPort_info;
trace_info_t* DesPort_info;
trace_info_t* PktLen_info;
trace_info_t Info_list[5];




// Time interval 
uint32_t next_report_time ;
uint32_t intervalTime;    
uint32_t Current_time;
uint32_t First_Time ;
struct timeval ts;

//Packet
struct sockaddr_storage saddr,daddr;
struct sockaddr *saddr_ptr,*daddr_ptr;
uint16_t sport,dport;
size_t payload_len ;
struct in_addr ip_addr_tmp;

uint16_t algorithm,alg_cnt,nor_cnt;

libtrace_linktype_t  linktype;
uint16_t eth_type;
uint32_t rem;

// dbn simulation 





void dbn_simulation(int len);

void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet);

bool analysis_is_ipv4( libtrace_packet_t *packet);

void trace_analysis_Init();

void cal(trace_info_t* (*alg)(tree_t *));

void Trace_processing(char* trace_path);

void Items_processing();

void per_packet(libtrace_packet_t *packet);

void classify(double* classify_container,double* stream,int length,int range,int part);

double cross_entropy(double* a_classify,double* b_classify,int length);

double cal_KLD(double* exact_stream ,double* base_stream,double* target_stream,int length);

unsigned long * CreateStream(int length, float z,int range,int offset);

void Simulation_processing();



#endif