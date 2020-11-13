#include <stdio.h>
#include "Tree.h"
#include "libtrace.h"
#include <inttypes.h>
#include <assert.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>


#ifndef _TRACE_ANALYSIS_H
#define _TRACE_ANALYSIS_H

tree_t* SrcIP_tree;
tree_t* DesIP_tree;
tree_t* SrcPort_tree;
tree_t* DesPort_tree;
tree_t* PktLen_tree;
tree_t* Protocol_tree;

uint32_t next_report_time ;
uint32_t invalTime ;

bool analysis_is_ipv4(struct sockaddr *ip);





#endif