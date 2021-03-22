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




//trace analysis mode paramters 
uint8_t TRACE ;



uint32_t intervalTime ;

// simulatio

// 5 tuple container for trace mode 
tree_t* SrcIP_tree;
tree_t* DesIP_tree;
tree_t* SrcPort_tree;
tree_t* DesPort_tree;
tree_t* PktLen_tree;
tree_t* Protocol_tree;


// Time interval 
uint32_t next_report_time ;
uint32_t intervalTime;    
struct timeval ts;
//Packet
struct sockaddr_storage saddr,daddr;
struct sockaddr *saddr_ptr,*daddr_ptr;
uint16_t sport,dport;
size_t payload_len ;
struct in_addr ip_addr_tmp;
int packet_cnt = 0;



libtrace_linktype_t  linktype;
uint16_t eth_type;
uint32_t rem;

// dbn simulation 





void dbn_simulation(int len);

void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet);

bool analysis_is_ipv4( libtrace_packet_t *packet);

void trace_analysis_Init();

void invoke_algorithms(tree_t *Stream,int index);

void Trace_processing(char* trace_path);

void Items_processing();

void per_packet(libtrace_packet_t *packet);





#endif