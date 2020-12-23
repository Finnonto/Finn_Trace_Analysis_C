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

void classify(double* classify_container,double* stream,int length,int range,int part)
{
	
	
	

	for(int i=0;i<601;i++)
	{
		//printf("%lf\n",classify_container[i]);
	}


	for(int i =0;i<length;i++)
	{
		double item;
		
		if(stream[i]<= -(range))
		{
			item = (double)-range;	
		}
		else if (stream[i]>=range)
		{
			item = (double)range;
		}
		else 
		{
			item = stream[i];
		}	
		
		int key = (int)(item*part) + range*part;
		
		//printf("%d\n",key);
		classify_container[key] +=  (double)1/length;
	}


	

}

double cross_entropy(double* a_classify,double* b_classify,int length)
{
	
	double S = 0;

    for(int i =0;i<length;i++)
	{
		if(a_classify[i]!=0)		
		{
			if (b_classify[i]/a_classify[i]!=0)
			{
				S-=a_classify[i]*(log(b_classify[i]/a_classify[i]));
				
			}
		}
	}    	
	printf("%lf\n",S);
    return S;

}

double cal_KLD(double* exact_stream ,double* base_stream,double* target_stream,int length)
{

	double base_dev[length];
	double target_dev[length];
	

	

	// to calculate the base_deviation with exact stream 
	for(int i =0;i<length;i++)
	{
		base_dev[i] = exact_stream[i]-base_stream[i];
		target_dev[i] = exact_stream[i]-target_stream[i];
	}
	
	//classification 
	int range = 2;
	int part = 100;
	int classify_len = 2*(range*part)+1;
	
	double* base_classify = (double*)calloc(classify_len,sizeof(double));
	double* target_classify = (double*)calloc(classify_len,sizeof(double));

	for(int i =0;i<(classify_len);i++)
	{
		base_classify[i]= 0.0 ;
		
		//printf("%d:%lf\n",i,base_classify[i]);
	
		target_classify[i]= 0.0 ;
	}	

	
	classify(base_classify,base_dev,length,range,part);
	classify(target_classify,target_dev,length,range,part);
	for(int i =0;i<(classify_len);i++)
	{
	
		//printf("%d:%lf\n",i,base_classify[i]);
		
	}	
	

	double a_b;
	double b_a;
    
	a_b = cross_entropy(base_classify,target_classify,classify_len);
	b_a = cross_entropy(target_classify,base_classify,classify_len);
	

	printf("%lf\n",(a_b+b_a)/2);
	
	printf("KLD done!\n");
	return (a_b+b_a)/2;


}

void CreateStream(tree_t * container ,int length, float z,int range,int offset)
{
  // generate a stream based of values drawn from a zip distribution

	long a,b;//for universal hash
	float zet;
	int i; 
	unsigned long value, *stream;
	prng_type * prng;// random number generator 

	//container = tree_create();
	// allocate new memory to stream
	stream=(unsigned long *) calloc(length+1,sizeof(unsigned long));
		
	prng=prng_Init(44545+offset*zipf_offset,2);//use offset to create different stream
	a = (long long) (prng_int(prng)% MOD);
  	b = (long long) (prng_int(prng)% MOD);
	zet=zeta(length,z);// to speed up the creation
	for (i=1;i<=length;i++) 
	{
		// get a value from the zipf dbn, and hash it to a new place
    	// use offset to mix things up a bit 
		// use offset to mix things up a bit 
		value = ( hash31(a,b,(int) floor( fastzipf(zipf_par,zipf_range,zet,prng) ) ) )&1048575; 
		
		tree_insert(container,value);
	}

	stream[0]=length;// makes first of stream is length
	tree_to_list(container);
	prng_Destroy(prng);
	

}

void Simulation_processing()
{
	//import tables 


	import_inverse_cdf_table(it);
	printf("import table done!\n");


    
  	tree_t* Sim_tree;
	trace_info_t *exact_info,*base_info,*target_info;
	
	double base_deviation[sim_times];
	double target_deviation[sim_times];

	//for absolute error measurement 
	double error_porb_arg = 0;
	double error_base_porb1 = 0;
	double error_base_porb2 = 0;
	double error_base_porb3 = 0;
	double error_base_porb4 = 0;
	double error_base_porb5 = 0;
	double error_base_porb6 = 0;
	double error_base_porb7 = 0;
	double error_base_porb8 = 0;
	double error_base_porb9 = 0;

	
	double error_target_porb1 = 0;
	double error_target_porb2 = 0;
	double error_target_porb3 = 0;
	double error_target_porb4 = 0;
	double error_target_porb5 = 0;
	double error_target_porb6 = 0;
	double error_target_porb7 = 0;
	double error_target_porb8 = 0;
	double error_target_porb9 = 0;

	// for relative measurement
	double entropy_exact_sum = 0; 
	double epsilon_base_sum = 0;
	double epsilon_target_sum = 0;
	double entropy_base_sum = 0;
	double entropy_target_sum = 0;


	// for stream measurement 
	double distinct[sim_times];
	double distinct_avg = 0;

	double exact_entropy[sim_times];//store exact entropies
	double base_entropy[sim_times];// store base entropies of algorithm
	double target_entropy[sim_times];// store target entropies of algorithm

	// create  streams and use binary tree to classify
	// if we only need to calculate the sketch entropy 
	// just treat stream as a turnstile data stream is
	// fine .classification is only for exact entropy.
	// We use for loop to calculate the error probability 
	//of these streams
	// 
	fprintf(stderr,"simulate %d sim_times\n",sim_times);
	for(int i =0;i<sim_times;i++)
	{
		
		Sim_tree = tree_create();
		CreateStream(Sim_tree,zipf_slen,zipf_par,zipf_range,i);
		
		
		// flatten the tree to linked list 
		
		
		//in this simulation we calculate the exact entropy
		// and clifford entropy
		
		// exact entropy
		exact_info = exact(Sim_tree);
		exact_entropy[i] = exact_info->entropy;
		distinct[i] = exact_info->distinct;
		

		// estimate entropy
		base_info = Clifford_est(Sim_tree);
		base_entropy[i] = base_info->entropy;
		base_deviation[i] =  fabs(exact_info->entropy - base_info->entropy);
		

		// if KLD is not started the we would not have an control group for 
		// base entropy , so we will just measure the base entropy.
		if(m_KLD == 1)
		{
			target_info = Clifford_cdf_est(Sim_tree);
			target_entropy[i] = target_info->entropy;
			target_deviation[i] = fabs(exact_info->entropy - target_info->entropy);
		}
		
		printf("Progress %g%%\r",(float)(i+1)/sim_times);
		fflush(stdout);
		
		

		
		tree_delete(Sim_tree);

	}
	
	for(int i =0;i<sim_times;i++)
	{
	
		// cal absolute error
	
		//base
		double dev_base_tmp = base_deviation[i];
		
		if(dev_base_tmp<= sim_error)error_porb_arg +=1.; 
		if(dev_base_tmp<= 0.1)error_base_porb1 +=1.; 
		if(dev_base_tmp<= 0.2)error_base_porb2 +=1.; 
		if(dev_base_tmp<= 0.3)error_base_porb3 +=1.; 
		if(dev_base_tmp<= 0.4)error_base_porb4 +=1.; 
		if(dev_base_tmp<= 0.5)error_base_porb5 +=1.; 
		if(dev_base_tmp<= 0.6)error_base_porb6 +=1.; 
		if(dev_base_tmp<= 0.7)error_base_porb7 +=1.; 
		if(dev_base_tmp<= 0.8)error_base_porb8 +=1.; 
		if(dev_base_tmp<= 0.9)error_base_porb9 +=1.; 

		
		if(m_KLD == 1)
		{
			double dev_target_tmp = target_deviation[i];
			if(dev_target_tmp<= 0.1)error_target_porb1 +=1.; 
			if(dev_target_tmp<= 0.2)error_target_porb2 +=1.; 
			if(dev_target_tmp<= 0.3)error_target_porb3 +=1.; 
			if(dev_target_tmp<= 0.4)error_target_porb4 +=1.; 
			if(dev_target_tmp<= 0.5)error_target_porb5 +=1.; 
			if(dev_target_tmp<= 0.6)error_target_porb6 +=1.; 
			if(dev_target_tmp<= 0.7)error_target_porb7 +=1.; 
			if(dev_target_tmp<= 0.8)error_target_porb8 +=1.; 
			if(dev_target_tmp<= 0.9)error_target_porb9 +=1.; 
		}
		
		// for relative error 
		
		entropy_exact_sum += exact_entropy[i];
		epsilon_base_sum += base_deviation[i]/exact_entropy[i];
		entropy_base_sum += base_entropy[i];

		if(m_KLD == 1)
		{
			epsilon_target_sum += target_deviation[i]/exact_entropy[i];
			entropy_target_sum += target_entropy[i];

		}

		



		distinct_avg += distinct[i];
	}
	//output 


	
	


	FILE *fp;
	char num[20];
	char output[200] = "./output/Simulation/";

	sprintf(num,"%d",zipf_slen);
	strcat(output,num);
	strcat(output,"_");

	sprintf(num,"%.1f",zipf_par);
	strcat(output,num);
	

	strcat(output,".csv");

	if( access( output, F_OK ) == 0 ) 
	{
    	fp = fopen(output,"a");
	} 	
	else 
	{
   		fp = fopen(output,"w");
		fprintf(fp,"Exact_entropy,B_erropP1,B_erropP2,B_erropP3,B_erropP4,B_erropP5,B_erropP6,B_erropP7,B_erropP8,B_erropP9,B_epsilon,B_entropy");
		if(m_KLD == 1)
		{
			fprintf(fp,",T_erropP1,T_erropP2,T_erropP3,T_erropP4,T_erropP5,T_erropP6,T_erropP7,T_erropP8,T_erropP9,T_epsilon,T_entropy,KLD");
		}
		fprintf(fp,",Distinct Count,Experiment Time\n");
	}
	
	
	fprintf(fp,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
							entropy_exact_sum/sim_times,
							error_base_porb1/sim_times,
							error_base_porb2/sim_times,
							error_base_porb3/sim_times,
							error_base_porb4/sim_times,
							error_base_porb5/sim_times,
							error_base_porb6/sim_times,
							error_base_porb7/sim_times,
							error_base_porb8/sim_times,
							error_base_porb9/sim_times,
							epsilon_base_sum/sim_times,
							entropy_base_sum/sim_times
							);


	
	if(m_KLD == 1)
	{
		fprintf(fp,",%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
							error_target_porb1/sim_times,
							error_target_porb2/sim_times,
							error_target_porb3/sim_times,
							error_target_porb4/sim_times,
							error_target_porb5/sim_times,
							error_target_porb6/sim_times,
							error_target_porb7/sim_times,
							error_target_porb8/sim_times,
							error_target_porb9/sim_times,
							epsilon_base_sum/sim_times,
							entropy_base_sum/sim_times,
							cal_KLD(
								exact_entropy,base_entropy,target_entropy,sim_times
							)
							);
	}
	



	fprintf(fp,",%f,%d\n",
							distinct_avg/sim_times,
							sim_times
							);





	fclose(fp);
}

