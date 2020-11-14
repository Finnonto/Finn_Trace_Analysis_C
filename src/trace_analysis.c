#include "trace_analysis.h"


void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet)
{

        if (trace)trace_destroy(trace);
        if (packet)trace_destroy_packet(packet);

}

bool analysis_is_ipv4(struct sockaddr *ip)
{
	if (ip->sa_family == AF_INET)return true;
	return false;
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
	printf("Processing");
	tree_to_list(SrcIP_tree);
	tree_to_list(DesIP_tree);
	tree_to_list(SrcPort_tree);
	tree_to_list(DesPort_tree);
	tree_to_list(PktLen_tree);
	
	SrcIP_info = exact(SrcIP_tree,0);
	DesIP_info = exact(DesIP_tree,0);
	SrcPort_info = exact(SrcPort_tree,0);
	DesPort_info = exact(DesPort_tree,0);
	PktLen_info = exact(PktLen_tree,0);

	
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
	static uint32_t runtime;
	runtime+=1;
	
	

	// get the packet information
	ts = trace_get_timeval(packet);
	saddr_ptr = trace_get_source_address(packet, (struct sockaddr *)&saddr);
	daddr_ptr = trace_get_destination_address(packet, (struct sockaddr *)&daddr);
	sport = trace_get_source_port(packet);
	dport = trace_get_destination_port(packet);
	payload_len = trace_get_payload_length(packet);




	//init the all counter and time 
	if(!next_report_time)
	{
			//initialization
			trace_analysis_Init();
	}

	Current_time = ts.tv_sec -First_Time;

	while(ts.tv_sec >= next_report_time)
	{   
			Items_Processing();
	}
	// if IPv4 then do counting 
	if(analysis_is_ipv4(saddr_ptr))
	{       
		struct sockaddr_in *v4 = (struct sockaddr_in *)saddr_ptr;
		ip_addr_tmp=v4->sin_addr;
		tree_insert(SrcIP_tree,ntohl(ip_addr_tmp.s_addr));
		if(sport)tree_insert(SrcPort_tree,sport);
		if(dport)tree_insert(DesPort_tree,dport);
		if(payload_len)tree_insert(PktLen_tree,payload_len);
	}
	
	if(analysis_is_ipv4(daddr_ptr))
	{
		struct sockaddr_in *v4 = (struct sockaddr_in *)daddr_ptr;
		ip_addr_tmp=v4->sin_addr;
		tree_insert(DesIP_tree,ntohl(ip_addr_tmp.s_addr));
	}       
		
}

