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
	// To setup the container(tree)
    SrcIP_tree = tree_create();
    DesIP_tree = tree_create();
    SrcPort_tree = tree_create();
    DesPort_tree = tree_create();
    PktLen_tree = tree_create();
    // to initialize the interval time 
    next_report_time = ts.tv_sec +intervalTime;
    First_Time = ts.tv_sec;
}

void cal(trace_info_t* (*alg)(tree_t *))
{
	// we apply function pointer to replace the
	// entropy calculation,so that we can choose 
	// specific entropy calculation by arguments
	SrcIP_info = alg(SrcIP_tree);
	DesIP_info = alg(DesIP_tree);
	SrcPort_info = alg(SrcPort_tree);
	DesPort_info = alg(DesPort_tree);
	PktLen_info = alg(PktLen_tree);

	Info_list[0] =*SrcIP_info;
	Info_list[1] =*DesIP_info;
	Info_list[2] =*SrcPort_info;
	Info_list[3] =*DesPort_info;
	Info_list[4] =*PktLen_info;

	output();
}

void Items_processing()
{
	// flatten the container(tree) to linked list 
	tree_to_list(SrcIP_tree);
	tree_to_list(DesIP_tree);
	tree_to_list(SrcPort_tree);
	tree_to_list(DesPort_tree);
	tree_to_list(PktLen_tree);

	// choose algorithms
	if (EXACT)
	{
		algorithm=0;
		cal(exact);
	}
	if (CLIFFORD)
	{
		algorithm=1;
		cal(Clifford_est);
	}
	
	// clean up the container after we apply it
	// to entropy calculations
	tree_delete(SrcIP_tree);
	tree_delete(DesIP_tree);	
	tree_delete(SrcPort_tree);
	tree_delete(DesPort_tree);
	tree_delete(PktLen_tree);

	// create another tree for next time interval
	SrcIP_tree = tree_create();
	DesIP_tree = tree_create();
	SrcPort_tree = tree_create();
	DesPort_tree = tree_create();
	PktLen_tree = tree_create();

	while(ts.tv_sec>=next_report_time)
	{
		next_report_time += intervalTime;
	}
}

void Trace_processing(char* trace_path)
{
	libtrace_t *trace = NULL;
	libtrace_packet_t *packet = NULL;
	packet = trace_create_packet();
	if (packet == NULL) {
			perror("Creating libtrace packet");
			libtrace_cleanup(trace, packet);
			exit(0);
	}

	trace = trace_create(trace_path);
	strcpy(Trace_Path,trace_path);
	if (trace_is_err(trace)) {
			trace_perror(trace,"Opening trace file");
			libtrace_cleanup(trace, packet);
			exit(0);
	}

	if (trace_start(trace) == -1) {
			trace_perror(trace,"Starting trace");
			libtrace_cleanup(trace, packet);
			exit(0);
	}

	//Filename
	
	

	create_folder();

	while (trace_read_packet(trace,packet)>0) {
			file_cnt=0;
			per_packet(packet);
	}
	//Items_Processing();     
	Close_Output_File();


	if (trace_is_err(trace)) {
			trace_perror(trace,"Reading packets");
			libtrace_cleanup(trace, packet);
			exit(0);
	}

	libtrace_cleanup(trace, packet);	

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
		// when time interval is over ,we do Items processing
		Items_processing();
	}
	
	// we only apply two types of packet to entropy
	// calculation 1.IPv4 2.Raw IPv4(only IPv4 header)
	
	if(linktype == TRACE_TYPE_NONE)//condition  for raw packet
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
	if (linktype == TRACE_TYPE_ETH && analysis_is_ipv4(packet))// condition for IPv4 packet
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
