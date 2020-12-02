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




tree_t* SrcIP_tree;
tree_t* DesIP_tree;
tree_t* SrcPort_tree;
tree_t* DesPort_tree;
tree_t* PktLen_tree;
tree_t* Protocol_tree;

//Main global param

//Info 
trace_info_t* SrcIP_info;
trace_info_t* DesIP_info;
trace_info_t* SrcPort_info;
trace_info_t* DesPort_info;
trace_info_t* PktLen_info;
trace_info_t Info_list[5];

//flag
uint8_t EXACT;
uint8_t CLIFFORD;
uint8_t INVERSE_CLI;
uint8_t ORIGIN;
uint8_t TOTAL;
uint8_t DISTINCT;


//Time
uint32_t next_report_time ;
uint32_t intervalTime;    //xiu
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

void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet);

bool analysis_is_ipv4( libtrace_packet_t *packet);

void trace_analysis_Init();

void cal(trace_info_t* (*alg)(tree_t *));

void Items_Processing();

void per_packet(libtrace_packet_t *packet);




#endif