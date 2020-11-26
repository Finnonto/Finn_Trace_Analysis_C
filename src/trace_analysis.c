#include "trace_analysis.h"


void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet)
{

        if (trace)trace_destroy(trace);
        if (packet)trace_destroy_packet(packet);

}

bool analysis_is_ipv4(libtrace_packet_t *packet)
{
	if(!trace_get_layer3(packet,&eth_type,&rem))return false;
	if (eth_type == TRACE_ETHERTYPE_IP)return true;
	else return false;
}

void trace_analysis_Init()
{
    SrcIP_tree = tree_create();
    DesIP_tree = tree_create();
    SrcPort_tree = tree_create();
    DesPort_tree = tree_create();
    PktLen_tree = tree_create();
    
    next_report_time = ts.tv_sec +invalTime;
    First_Time = ts.tv_sec;
}

void Items_Processing()
{
	tree_to_list(SrcIP_tree);
	tree_to_list(DesIP_tree);
	tree_to_list(SrcPort_tree);
	tree_to_list(DesPort_tree);
	tree_to_list(PktLen_tree);
	
	SrcIP_info = Clifford_cdf_stage_est(SrcIP_tree);
	DesIP_info = Clifford_cdf_stage_est(DesIP_tree);
	SrcPort_info = Clifford_cdf_stage_est(SrcPort_tree);
	DesPort_info = Clifford_cdf_stage_est(DesPort_tree);
	PktLen_info = Clifford_cdf_stage_est(PktLen_tree);

	
	Info_list[0] =*SrcIP_info;
	Info_list[1] =*DesIP_info;
	Info_list[2] =*SrcPort_info;
	Info_list[3] =*DesPort_info;
	Info_list[4] =*PktLen_info;
	
	output_entropy_csv(Info_list,Current_time);
	
	tree_delete(SrcIP_tree);
	tree_delete(DesIP_tree);	
	tree_delete(SrcPort_tree);
	tree_delete(DesPort_tree);
	tree_delete(PktLen_tree);


	SrcIP_tree = tree_create();
	DesIP_tree = tree_create();
	SrcPort_tree = tree_create();
	DesPort_tree = tree_create();
	PktLen_tree = tree_create();

	while(ts.tv_sec>=next_report_time)
	{
		next_report_time += invalTime;
	}
}

void per_packet(libtrace_packet_t *packet)
{
	
	
	
	

	// get the packet information
	ts = trace_get_timeval(packet);
	payload_len = trace_get_payload_length(packet);
	linktype = trace_get_link_type(packet);

	//init the all counter and time 
	if(next_report_time == 0)
	{
		//initialization
		trace_analysis_Init();
	}

	Current_time = ts.tv_sec -First_Time;

	while(ts.tv_sec >= next_report_time)
	{   
		Items_Processing();
	}
	
	//check link type and if ipv4 or not
	
	if(linktype == TRACE_TYPE_NONE)
	{
		
		saddr_ptr = trace_get_source_address(packet, (struct sockaddr *)&saddr);
		daddr_ptr = trace_get_destination_address(packet, (struct sockaddr *)&daddr);
		
		struct sockaddr_in *v4 = (struct sockaddr_in *)saddr_ptr;
		ip_addr_tmp=v4->sin_addr;
		tree_insert(SrcIP_tree,ntohl(ip_addr_tmp.s_addr));
		
		struct sockaddr_in *v3 = (struct sockaddr_in *)daddr_ptr;
		ip_addr_tmp=v3->sin_addr;
		tree_insert(DesIP_tree,ntohl(ip_addr_tmp.s_addr));

		tree_insert(PktLen_tree,payload_len);

	}
	if (linktype == TRACE_TYPE_ETH && analysis_is_ipv4(packet))
	{
		
		saddr_ptr = trace_get_source_address(packet, (struct sockaddr *)&saddr);
		daddr_ptr = trace_get_destination_address(packet, (struct sockaddr *)&daddr);
		sport = trace_get_source_port(packet);
		dport = trace_get_destination_port(packet);



		tree_insert(SrcPort_tree,sport);
		tree_insert(DesPort_tree,dport);
		tree_insert(PktLen_tree,payload_len);

		struct sockaddr_in *v4 = (struct sockaddr_in *)saddr_ptr;
		ip_addr_tmp=v4->sin_addr;
		tree_insert(SrcIP_tree,ntohl(ip_addr_tmp.s_addr));
		
		struct sockaddr_in *v3 = (struct sockaddr_in *)daddr_ptr;
		ip_addr_tmp=v3->sin_addr;
		tree_insert(DesIP_tree,ntohl(ip_addr_tmp.s_addr));


		}
	
	
		    
		
}

