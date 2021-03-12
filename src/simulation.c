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

void Simulation_processing()
{
	//import tables 
	
	printf("importing table ...\n");
	//import_optimized_cdf_table(it);
	//import_inverse_cdf_stage50_table(it,50);
	//import_inverse_cdf_stage100_table(it,100);
	import_HeadTail_table();
	printf("import table done!\n");

	


	// create  streams and use binary tree to classify
	// if we only need to calculate the sketch entropy 
	// just treat stream as a turnstile data stream is
	// fine .classification is only for exact entropy.
	// We use for loop to calculate the error probability 
	//of these streams
	// 
	fprintf(stderr,"simulate %d sim_times\n",sim_times);
	for(int sim =0;sim<sim_times;sim++)
	{

		

		tree_t* Sim_tree;
		Sim_tree = tree_create();
		CreateStream(Sim_tree,zipf_slen,zipf_par,zipf_range,sim);
		
		
		// flatten the tree to linked list 
		
		
		//in this simulation we calculate the exact entropy
		// and clifford entropy
		
		// exact entropy
		exact_info = exact(Sim_tree);
		exact_entropy[sim] = exact_info->entropy;
		Distinct[sim] =exact_info->distinct;
		//choosing algorithm
		for(int j=1;j<MAX_ALG;j++)
		{
			if(ALG_flag[j]==1)
			{
				switch (j){
					case 1:
						Clifford_info = Clifford_est(Sim_tree);
						Clifford_entropy[sim] = Clifford_info->entropy;
						break;
					
					case 2:
						Clifford_cdf_info = Clifford_cdf_est(Sim_tree);
						Clifford_cdf_entropy[sim] = Clifford_cdf_info->entropy;
						break;
					
					case 3:
						Clifford_cdf_stage50_info = Clifford_cdf_stage50_est(Sim_tree);
						Clifford_cdf_stage50_entropy[sim] = Clifford_cdf_stage50_info->entropy;
						break;
					
					case 4:
						Clifford_cdf_stage100_info = Clifford_cdf_stage100_est(Sim_tree);
						Clifford_cdf_stage100_entropy[sim] = Clifford_cdf_stage100_info->entropy;
						break;
					
					case 5:
						Clifford_cdf_opt_info = Clifford_cdf_opt_est(Sim_tree);
						Clifford_cdf_opt_entropy[sim] = Clifford_cdf_opt_info->entropy;
						break;
					
					case 6:
						Clifford_HT_info = Clifford_HT_est(Sim_tree);
						Clifford_HT_entropy[sim] = Clifford_HT_info->entropy;
						break;
					
					case 7:
						Clifford_HTo_info = Clifford_HTo_est(Sim_tree);
						Clifford_HTo_entropy[sim] = Clifford_HTo_info->entropy;
						break;
					
					case 8:
						Clifford_HTo_65536_info = Clifford_HTo_65536_est(Sim_tree);
						Clifford_HTo_65536_entropy[sim] = Clifford_HTo_65536_info->entropy;
						break;
					
					case 9:
						Clifford_HTo_interpolation_info = Clifford_HTo_interpolation_est(Sim_tree);
						Clifford_HTo_interpolation_entropy[sim] = Clifford_HTo_interpolation_info->entropy;
						break;
					
					case 10:
						Clifford_HTo_interpolation_65536_info = Clifford_HTo_interpolation_65536_est(Sim_tree);
						Clifford_HTo_interpolation_65536_entropy[sim] = Clifford_HTo_interpolation_65536_info->entropy;
						break;
					
					case 11:
						PingLi_info = PingLi_est(Sim_tree);
						PingLi_entropy[sim] = PingLi_info->entropy;
						break;

					default:
						fprintf(stderr,"invoke algorthm error!\n");
						exit(0);
				}
			}
		}

		// if KLD is not started the we would not have an control group for 
		// base entropy , so we will just measure the base entropy.
		
	
		
		printf("Progress %g%%\r",(float)(sim+1)/sim_times);
		fflush(stdout);
		
		

		
		tree_delete(Sim_tree);

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
	strcat(output,"_");

	sprintf(num,"%d",zipf_range);
	strcat(output,num);		

	strcat(output,".csv");

	if( access( output, F_OK ) == 0 ) 
	{
    	fp = fopen(output,"a");
	} 	
	else 
	{
   		fp = fopen(output,"w");
		fprintf(fp,"Exact_entropy,");
		for(int i=1;i<MAX_ALG;i++)
		{
			if(ALG_flag[i]==1)
			{
				switch (i)
				{
					case 1:
					fprintf(fp,"Clifford_entropy,");
					break;
				
					case 2:
						fprintf(fp,"Clifford_cdf_entropy,");
						break;
					
					case 3:
						fprintf(fp,"Clifford_cdf_stage50_entropy,");
						break;
					
					case 4:
						fprintf(fp,"Clifford_cdf_stage100_entropy,");
						break;
					
					case 5:
						fprintf(fp,"Clifford_cdf_opt_entropy,");
						break;
					
					case 6:
						fprintf(fp,"Clifford_HT_entropy,");
						break;
					
					case 7:
						fprintf(fp,"Clifford_HTo_entropy,");
						break;
					
					case 8:
						fprintf(fp,"Clifford_HTo_65536_entropy,");
						break;
					
					case 9:
						fprintf(fp,"Clifford_HTo_interpolation_entropy,");
						break;
					
					case 10:
						fprintf(fp,"Clifford_HTo_interpolation_65536_entropy,");
						break;
					
					case 11:
						fprintf(fp,"PingLi_entropy,");
						break;
				}
			}
		}					
		fprintf(fp,"Distinct Count,Total Len,Table Size\n");
	}
	
	for(int i=0;i<sim_times;i++)
	{

		fprintf(fp,"%f,",exact_entropy[i]);
		for(int j=1;j<MAX_ALG;j++)
		{
			if(ALG_flag[j]==1)
			{
				switch (j)
				{
					case 1:
						fprintf(fp,"%f,",Clifford_entropy[i]);
						break;
					
					case 2:
						fprintf(fp,"%f,",Clifford_cdf_entropy[i]);
						break;
					
					case 3:
						fprintf(fp,"%f,",Clifford_cdf_stage50_entropy[i]);
						break;
					
					case 4:
						fprintf(fp,"%f,",Clifford_cdf_stage100_entropy[i]);
						break;
					
					case 5:
						fprintf(fp,"%f,",Clifford_cdf_opt_entropy[i]);
						break;
					
					case 6:
						fprintf(fp,"%f,",Clifford_HT_entropy[i]);
						break;
					
					case 7:
						fprintf(fp,"%f,",Clifford_HTo_entropy[i]);
						break;
					
					case 8:
						fprintf(fp,"%f,",Clifford_HTo_65536_entropy[i]);
						break;
					
					case 9:
						fprintf(fp,"%f,",Clifford_HTo_interpolation_entropy[i]);
						break;
					
					case 10:
						fprintf(fp,"%f,",Clifford_HTo_interpolation_65536_entropy[i]);
						break;
					
					case 11:						
						fprintf(fp,"%f,",PingLi_entropy[i]);
						break;
				}
			}
		}		
		fprintf(fp,"%d,%d,%d \n",
								Distinct[i],
								zipf_slen,
								Table_Size
								);	
	}
	


	fclose(fp);
}

