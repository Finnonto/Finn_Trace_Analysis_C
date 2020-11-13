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


//Time
uint32_t next_report_time;
uint32_t invalTime;
uint32_t Current_time;
uint32_t First_Time ;
struct timeval ts;

//Packet
struct sockaddr_storage saddr,daddr;
struct sockaddr *saddr_ptr,*daddr_ptr;
uint16_t sport,dport;
size_t payload_len ;
struct in_addr ip_addr_tmp;

void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet);

bool analysis_is_ipv4(struct sockaddr *ip);

void trace_analysis_Init();

void Items_Processing();

void per_packet(libtrace_packet_t *packet);




#endif