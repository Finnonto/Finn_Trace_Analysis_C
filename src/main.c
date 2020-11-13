#include "libtrace.h"
#include "trace_analysis.h"
#include "Tree.h"
#include "cal_method.h"
#include <stdio.h>


uint32_t runtime_cnt =1;


void per_packet(libtrace_packet_t *packet)
{
        //printf("runtime = %d\n",runtime_cnt);
	// initi all param
        struct sockaddr_storage saddr,daddr;
	struct sockaddr *saddr_ptr,*daddr_ptr;
	uint16_t sport,dport;
        size_t payload_len ;
        struct in_addr ip_addr_tmp;
        struct timeval ts;


        // get the packet information
        ts = trace_get_timeval(packet);
	saddr_ptr = trace_get_source_address(packet, (struct sockaddr *)&saddr);
        daddr_ptr = trace_get_destination_address(packet, (struct sockaddr *)&daddr);
	sport = trace_get_source_port(packet);
        dport = trace_get_destination_port(packet);
        payload_len = trace_get_payload_length(packet);
        //printf("check 2\n");
        //init cal info
        trace_info_t* SrcIP_info;
        trace_info_t* DesIP_info;
        trace_info_t* SrcPort_info;
        trace_info_t* DesPort_info;
        trace_info_t* PktLen_info;


        //init the all counter and time 
        if(!next_report_time)
        {
                //initialization
                //printf("check init\n");
                SrcIP_tree = tree_create();
                DesIP_tree = tree_create();
                SrcPort_tree = tree_create();
                DesPort_tree = tree_create();
                PktLen_tree = tree_create();
                
                next_report_time = ts.tv_sec +invalTime;
        }
        // time's up to do what I want and clean all counter
        while(ts.tv_sec > next_report_time)
        {       
                tree_to_list(SrcIP_tree);
                tree_to_list(DesIP_tree);
                tree_to_list(SrcPort_tree);
                tree_to_list(DesPort_tree);
                tree_to_list(PktLen_tree);
                
                printf("check 1\n");
                SrcIP_info = exact(SrcIP_tree,0);
                printf("check 2\n");
                DesIP_info = exact(DesIP_tree,0);
                printf("check 3\n");
                SrcPort_info = exact(SrcPort_tree,0);
                printf("check 4\n");
                DesPort_info = exact(DesPort_tree,0);
                printf("check 5\n");
                PktLen_info = exact(PktLen_tree,0);

                
                
                printf("SrcIP_entropy = %f\n",SrcIP_info->entropy);
                printf("DesIP_entropy = %f\n",DesIP_info->entropy);
                printf("SrcPort_entropy = %f\n",SrcPort_info->entropy);
                printf("DesPort_entropy = %f\n",DesPort_info->entropy);
                printf("PktLen_entropy = %f\n",PktLen_info->entropy);

        
                SrcIP_tree = tree_create();
                DesIP_tree = tree_create();
                SrcPort_tree = tree_create();
                DesPort_tree = tree_create();
                PktLen_tree = tree_create();
                next_report_time =  ts.tv_sec +invalTime;

        }
        // if IPv4 then do counting 
        if(analysis_is_ipv4(saddr_ptr))
        {       
                //printf("check sip\n");
                struct sockaddr_in *v4 = (struct sockaddr_in *)saddr_ptr;
                ip_addr_tmp=v4->sin_addr;
                SrcIP_tree->root = tree_insert(SrcIP_tree,ntohl(ip_addr_tmp.s_addr));
                //printf("check sip end\n");
        }
        
        if(analysis_is_ipv4(daddr_ptr))
        {
                //printf("check dip\n");
                struct sockaddr_in *v4 = (struct sockaddr_in *)daddr_ptr;
                //printf("check dip1\n");
                ip_addr_tmp=v4->sin_addr;
                //printf("check dip2\n");
                DesIP_tree->root = tree_insert(DesIP_tree,ntohl(ip_addr_tmp.s_addr));
                //printf("check dip end\n");
        }       
        //printf("Dip info  =%d,%d\n",DesIP_tree->root->data,DesIP_tree->root->height); 
        //printf("check port\n");
        if(sport)SrcPort_tree->root = tree_insert(SrcPort_tree,sport);
        if(dport)DesPort_tree->root = tree_insert(DesPort_tree,dport);
        //printf("check por end\n");
        //printf("check pktlen\n");
        if(payload_len)PktLen_tree->root = tree_insert(PktLen_tree,payload_len);
        //printf("check pktlen end\n");
        
        runtime_cnt +=1 ;

}

void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet) {

        if (trace)trace_destroy(trace);
        if (packet)trace_destroy_packet(packet);

}

int main(int argc, char *argv[])
{
        

        libtrace_t *trace = NULL;
        libtrace_packet_t *packet = NULL;
        

        if (argc < 3) {
                //printf(stderr, "Usage: %s inputURI\n", argv[0]);
                return 1;
        }

        packet = trace_create_packet();

        if (packet == NULL) {
                perror("Creating libtrace packet");
                libtrace_cleanup(trace, packet);
                return 1;
        }

        trace = trace_create(argv[1]);

        if (trace_is_err(trace)) {
                trace_perror(trace,"Opening trace file");
                libtrace_cleanup(trace, packet);
                return 1;
        }

        if (trace_start(trace) == -1) {
                trace_perror(trace,"Starting trace");
                libtrace_cleanup(trace, packet);
                return 1;
        }

        invalTime = strtoul(argv[2],NULL,10);

        if(invalTime<=0)
        {
                invalTime = 1;
        }
        //printf("check 1\n");
        while (trace_read_packet(trace,packet)>0) {
                per_packet(packet);
        }


        if (trace_is_err(trace)) {
                trace_perror(trace,"Reading packets");
                libtrace_cleanup(trace, packet);
                return 1;
        }

        libtrace_cleanup(trace, packet);
        return 0;
}

