#include "simulation.h"



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
	
    return S;

}

double cal_KLD(double* exact_stream ,double* base_stream,double* target_stream,int length)
{

	double base_dev[length];
	double target_dev[length];
	

	

	// to calculate the base_deviation with exact stream 
	for(int i =0;i<length;i++)
	{
		base_dev[i] = (exact_stream[i]-base_stream[i])/log(length);
		target_dev[i] = (exact_stream[i]-target_stream[i])/log(length);
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
	/*
	printf("importing table ...\n");
	import_optimized_cdf_table(it);
	//import_inverse_cdf_stage50_table(it,50);
	//import_inverse_cdf_stage100_table(it,100);
	//import_HeadTail_table();
	printf("import table done!\n");

	*/
    
  	tree_t* Sim_tree;
	trace_info_t *exact_info,*Clifford_info,*PingLi_info;
	
	

	//for absolute error measurement 
	
	double exact_entropy[sim_times];//store exact entropies
	double Clifford_entropy[sim_times];// store base entropies of algorithm
	double PingLi_entropy[sim_times];// store target entropies of algorithm
	int    Distinct[sim_times];
	

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
		Distinct[i] =exact_info->distinct;
		
		
		
		// estimate entropy
		Clifford_info = Clifford_est(Sim_tree);
		Clifford_entropy[i] = Clifford_info->entropy;
		
		PingLi_info = PingLi_est(Sim_tree);
		PingLi_entropy[i] = PingLi_info->entropy;
		

		// if KLD is not started the we would not have an control group for 
		// base entropy , so we will just measure the base entropy.
		
	
		
		printf("Progress %g%%\r",(float)(i+1)/sim_times);
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
		fprintf(fp,"Exact_entropy,Clifford_entropy,PingLi_entropy");
		
		fprintf(fp,",Distinct Count,Total Len,Table Size\n");
	}
	
	for(int i=0;i<sim_times;i++)
	{
		fprintf(fp,"%f,%f,%f,%d,%d,%d \n",
							
										exact_entropy[i],
										Clifford_entropy[i],
										PingLi_entropy[i],
										Distinct[i],
										zipf_slen,
										Table_Size
										);	
	}
	




	fclose(fp);
}

