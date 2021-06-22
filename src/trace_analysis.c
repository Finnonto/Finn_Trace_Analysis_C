#include "trace_analysis.h"
#define TIMESCALE 1000000


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
	initTime = ts.tv_usec;
    next_report_time = ts.tv_sec +intervalTime;
	ent_cnt = 0;
}

void invoke_algorithms(tree_t *Stream,int index)
{
	// we apply function pointer to replace the
	// entropy calculation,so that we can choose 
	// specific entropy calculation by arguments
	trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));
	exact(Stream,info);
	exact_entropy[index][ent_cnt] = info->entropy;
	StreamDistinct[index][ent_cnt] =info->distinct;
	StreamLength[index][ent_cnt] =info->total_count;
	//choosing algorithm
	for(int j=1;j<MAX_ALG;j++)
		{
			if(ALG_flag[j]==1)
			{
				switch (j){
					case 1:
						if(Clifford_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 2:
						if(Clifford_cdf_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 3:
						if(Clifford_cdf_parallel_interpolation_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_parallel_interpolation_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 4:
						if(Clifford_cdf_parallel_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_parallel_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 5:
						if(Clifford_cdf_parallel_mhash_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_parallel_mhash_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 6:
						if(Clifford_HT_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_HT_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 7:
						if(Clifford_cdf_parallel_interpolation_2th_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_parallel_interpolation_2th_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 8:
						if(Clifford_HTo_65536_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_HTo_65536_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 9:
						if(Clifford_HTo_interpolation_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_HTo_interpolation_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 10:
						if(Clifford_HTo_interpolation_65536_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_HTo_interpolation_65536_entropy[index][ent_cnt] = info->entropy;
						break;
					
					case 11:
						if(PingLi_est(Stream,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						PingLi_entropy[index][ent_cnt] = info->entropy;
						break;

					default:
						fprintf(stderr,"invoke algorthm error!\n");
						exit(0);
				}
			}
		}
		free(info);
}

void Items_processing()
{
	
	
	// flatten the container(tree) to linked list 
	tree_to_list(SrcIP_tree);
	tree_to_list(DesIP_tree);
	tree_to_list(SrcPort_tree);
	tree_to_list(DesPort_tree);
	tree_to_list(PktLen_tree);
	
	invoke_algorithms(SrcIP_tree,0);
	invoke_algorithms(DesIP_tree,1);
	invoke_algorithms(SrcPort_tree,2);
	invoke_algorithms(DesPort_tree,3);
	invoke_algorithms(PktLen_tree,4);
	


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

	
}

void Trace_processing(char* trace_path)
{
	mpd_init(&ctx, 38);


	if(resolution == 4096 || resolution == 16384||resolution == 65536||resolution == (pow(2,31)-1)){		//import_optimized_cdf_table(it);
		//import_inverse_cdf_stage50_table(it,50);
		//import_inverse_cdf_parallel_table(it,100);
		import_inverse_cdf_table(it,TableIndex,TableINT);
		//import_HeadTail_table();
	}

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
	
	

	next_report_time = 0;
	
	trace_packet_cnt = 0;
	
	while (trace_read_packet(trace,packet)>0) {
			
			
			per_packet(packet);
	}
	
	Items_processing();
	//to make sure that next_report_time is over ts.tv_sec

	
	printf("Progress %d timeinterval\r",ent_cnt);
	fflush(stdout);
	ent_cnt ++;


	printf("\n");
	printf("analysis done\n");
	Output_Trace();     
	


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
	long timecheck;
	if(ts.tv_usec-initTime <0)timecheck = ts.tv_sec-1;
	if(ts.tv_usec-initTime >=0)timecheck = ts.tv_sec;

	payload_len = trace_get_payload_length(packet);
	trace_get_layer2(packet,&linktype,&rem);
	

	if(next_report_time == 0)
	{
		trace_analysis_Init();
	}

	while(timecheck >=next_report_time)
	{   
		
		// when time interval is over ,we do Items processing
		Items_processing();
		//to make sure that next_report_time is over ts.tv_sec
		while(timecheck>=next_report_time)
		{
			next_report_time += intervalTime;
		}
		printf("Progress %d timeinterval\r",ent_cnt);
		fflush(stdout);
		ent_cnt ++;
		trace_packet_cnt= 0;
	}
	
	// we only apply two types of packet to entropy
	// calculation 1.IPv4 2.Raw IPv4(only IPv4 header)
	
	if(linktype == TRACE_TYPE_NONE)//condition  for raw packet
	{
		trace_packet_cnt++;
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
		trace_packet_cnt++;
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
