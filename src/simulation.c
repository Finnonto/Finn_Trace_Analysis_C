#include "simulation.h"






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

void CreateAllDifferentStream(tree_t * container,int length,int offset)
{
	srand(offset);
	int N = RAND_MAX-1;//avoid overflow
	int M = length;
	int in, im;
	node_t * current_node;
	im = 0;

	for (in = 0; in < N && im < M; ++in) {
		int rn = N - in;
		int rm = M - im;
		if (rand() % rn < rm){
			if(im == 0){
				container->root = node_create(in+1);
				current_node = container->root;
				im++;
			}
			if(im>0){
				current_node->right = node_create(in+1);
				current_node = current_node->right;
				im++;
			}
		}    
	}
}

void CreateAllSameStream(tree_t * container,int length,int offset)
{
	for(int i=0;i<length;i++)
	{
		tree_insert(container,offset);
	}
	tree_to_list(container);
}

void Simulation_processing()
{
	//import tables 
	
	if(resolution == 4096 || resolution == 16384||resolution == 65536||resolution == (pow(2,31)-1)){
		
		//import_optimized_cdf_table(it);
		//import_inverse_cdf_stage50_table(it,50);
		//import_inverse_cdf_parallel_table(it,100);
		import_inverse_cdf_table(it,TableIndex,TableINT);
		//import_HeadTail_table();
	}
	//decimal init
	mpd_init(&ctx, 20);
	// create  streams and use binary tree to classify
	// if we only need to calculate the sketch entropy 
	// just treat stream as a turnstile data stream is
	// fine .classification is only for exact entropy.
	// We use for loop to calculate the error probability 
	//of these streams
	// 
	if(Change_Hash==0){
			hash_para_gen(hashseed,para_a,para_c,K_Value*m_hash);	
	}
		


	fprintf(stderr,"simulate %d sim_times\n",sim_times);
	trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));
	for(int sim =0;sim<sim_times;sim++)
	{
		if(Change_Hash){
			hash_para_gen(hashseed*(sim+1),para_a,para_c,K_Value*m_hash);
		}
		

		tree_t* Sim_tree;
		Sim_tree = tree_create();
		if(zipf_par == -1)
		{
			CreateAllDifferentStream(Sim_tree,zipf_slen,sim);
		}
		if (zipf_par == 0 )
		{
			CreateAllSameStream(Sim_tree,zipf_slen,sim);
		}
		if (sim_start_idx != 0)
		{
			CreateStream(Sim_tree,zipf_slen,zipf_par,zipf_range,sim_start_idx);
		}
		if (sim_start_idx == 0)
		{
			CreateStream(Sim_tree,zipf_slen,zipf_par,zipf_range,sim);
		}
		
		// flatten the tree to linked list 
		//in this simulation we calculate the exact entropy
		// and clifford entropy
		// exact entropy
		if(exact(Sim_tree,info)!=0){
			fprintf(stderr,"simualtion fail\n");
			exit(0);
		}

		
		exact_entropy[0][sim] = info->entropy;
		StreamDistinct[0][sim] =info->distinct;
		StreamLength[0][sim] =info->total_count;
		
		//choosing algorithm
		#ifndef RES_ALLDIFF
		for(int j=1;j<MAX_ALG;j++)
		{
			if(ALG_flag[j]==1)
			{
				switch (j){
					case 1:
						if(Clifford_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_entropy[0][sim] = info->entropy;
						break;
					
					case 2:
						if(Clifford_cdf_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_entropy[0][sim] = info->entropy;
						break;
					
					case 3:
						if(Clifford_cdf_parallel_interpolation_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_parallel_interpolation_entropy[0][sim] = info->entropy;
						break;
					
					case 4:
						if(Clifford_cdf_parallel_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_parallel_entropy[0][sim] = info->entropy;
						break;
					
					case 5:
						if(Clifford_cdf_parallel_mhash_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_parallel_mhash_entropy[0][sim] = info->entropy;
						break;
					
					case 6:
						if(Clifford_HT_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_HT_entropy[0][sim] = info->entropy;
						break;
					
					case 7:
						if(Clifford_cdf_parallel_interpolation_2th_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_cdf_parallel_interpolation_2th_entropy[0][sim] = info->entropy;
						break;
					
					case 8:
						if(Clifford_HTo_65536_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_HTo_65536_entropy[0][sim] = info->entropy;
						break;
					
					case 9:
						if(Clifford_HTo_interpolation_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_HTo_interpolation_entropy[0][sim] = info->entropy;
						break;
					
					case 10:
						if(Clifford_HTo_interpolation_65536_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						Clifford_HTo_interpolation_65536_entropy[0][sim] = info->entropy;
						break;
					
					case 11:
						if(PingLi_est(Sim_tree,info)!=0){
							fprintf(stderr,"simualtion fail\n");
							exit(0);
						}
						PingLi_entropy[0][sim] = info->entropy;
						break;

					default:
						fprintf(stderr,"invoke algorthm error!\n");
						exit(0);
				}
			}
		}
		#endif

		#ifdef RES_ALLDIFF
			for(int i =0 ;i<RES_EXP-1;++i){
				resolution = pow(2,(i+2)*2);
				res_record[i] = (i+2)*2;
				if(Clifford_est(Sim_tree,info)!=0){
					fprintf(stderr,"simualtion fail\n");
					exit(0);
				}
				Clifford_resalldiff_entropy[i][sim] = info->entropy;
			}
			resolution = RAND_MAX;
			res_record[RES_EXP-1] = resolution;
			if(Clifford_est(Sim_tree,info)!=0){
				fprintf(stderr,"simualtion fail\n");
				exit(0);
			}
			Clifford_resalldiff_entropy[RES_EXP-1][sim] = info->entropy;
		#endif
		// if KLD is not started the we would not have an control group for 
		// base entropy , so we will just measure the base entropy.
		printf("Progress %0.1f%%\r",(float)(sim+1)/sim_times*100);
		fflush(stdout);
		ent_cnt = sim_times;
		tree_delete(Sim_tree);

	}
	free(info);
	printf("\n");
	for(int j=1;j<MAX_ALG;j++)
	{
		if(ALG_flag[j]==1)
		{
			switch (j){
				case 1:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_entropy,0,sim_times);
					break;
				case 2:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_cdf_entropy,0,sim_times);
					break;
				case 3:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_cdf_parallel_interpolation_entropy,0,sim_times);
					break;
				case 4:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_cdf_parallel_entropy,0,sim_times);
					break;
				case 5:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_cdf_parallel_mhash_entropy,0,sim_times);
					break;
				case 6:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_HT_entropy,0,sim_times);
					break;
				case 7:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_cdf_parallel_interpolation_2th_entropy,0,sim_times);
					break;
				case 8:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_HTo_65536_entropy,0,sim_times);
					break;
				case 9:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_HTo_interpolation_entropy,0,sim_times);
					break;
				case 10:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*Clifford_HTo_interpolation_65536_entropy,0,sim_times);
					break;
				case 11:
					MAPE[0][j] = cal_MAPE(*exact_entropy,*PingLi_entropy,0,sim_times);
					break;
				default:
					fprintf(stderr,"invoke algorthm error!\n");
					exit(0);
			}
		}
	}
	Output_MAPE();
	Output_Simulation();
	//output 	
}

