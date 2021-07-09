#include "pseudo_trace.h"

void Read_Pseudo_Trace(char* trace_path,tree_t * container){
    FILE *ifp1;
    unsigned long int key;
    if((ifp1=fopen(trace_path,"r"))==NULL)
    {
      printf("Error!! Can't open pattern file: %s !!!\n",trace_path);
      exit(0);
    }

     while (fscanf(ifp1, "%lu",&key)==1) {
          tree_insert(container,key);
     }
    fclose(ifp1);
    tree_to_list(container);
}

void Pseudo_Trace_processing(char* trace_path)
{
	//import tables 
	strcpy(Pseudo_Path,trace_path);

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
    /*
	if(Change_Hash){
			hash_para_gen(hashseed*(sim+1),para_a,para_c,K_Value*m_hash);
	}	
    */

	fprintf(stderr,"parsing %s \n",trace_path);
	trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));

    
    

    tree_t* Pseudo_tree;
    Pseudo_tree = tree_create();
    
    Read_Pseudo_Trace(trace_path,Pseudo_tree);
    
    if(exact(Pseudo_tree,info)!=0){
        fprintf(stderr,"simualtion fail\n");
        exit(0);
    }
    int output_idx = 0;
    
    exact_entropy[0][output_idx] = info->entropy;
    StreamDistinct[0][output_idx] =info->distinct;
    StreamLength[0][output_idx] =info->total_count;
    
    //choosing algorithm
    #ifndef RES_ALLDIFF
    for(int j=1;j<MAX_ALG;j++)
    {
        if(ALG_flag[j]==1)
        {
            switch (j){
                case 1:
                    if(Clifford_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 2:
                    if(Clifford_cdf_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_cdf_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 3:
                    if(Clifford_cdf_parallel_interpolation_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_cdf_parallel_interpolation_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 4:
                    if(Clifford_cdf_parallel_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_cdf_parallel_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 5:
                    if(Clifford_cdf_parallel_mhash_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_cdf_parallel_mhash_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 6:
                    if(Clifford_HT_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_HT_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 7:
                    if(Clifford_cdf_parallel_interpolation_2th_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_cdf_parallel_interpolation_2th_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 8:
                    if(Clifford_HTo_65536_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_HTo_65536_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 9:
                    if(Clifford_HTo_interpolation_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_HTo_interpolation_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 10:
                    if(Clifford_HTo_interpolation_65536_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    Clifford_HTo_interpolation_65536_entropy[0][output_idx] = info->entropy;
                    break;
                
                case 11:
                    if(PingLi_est(Pseudo_tree,info)!=0){
                        fprintf(stderr,"simualtion fail\n");
                        exit(0);
                    }
                    PingLi_entropy[0][output_idx] = info->entropy;
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
            if(Clifford_est(Pseudo_tree,info)!=0){
                fprintf(stderr,"simualtion fail\n");
                exit(0);
            }
            Clifford_resalldiff_entropy[i][output_idx] = info->entropy;
        }
        resolution = RAND_MAX;
        res_record[RES_EXP-1] = resolution;
        if(Clifford_est(Pseudo_tree,info)!=0){
            fprintf(stderr,"simualtion fail\n");
            exit(0);
        }
        Clifford_resalldiff_entropy[RES_EXP-1][output_idx] = info->entropy;
    #endif
    // if KLD is not started the we would not have an control group for 
    // base entropy , so we will just measure the base entropy.
    // printf("Progress %0.1f%%\r",(float)(sim+1)/sim_times*100);
    // flush(stdout);
    ent_cnt = 1;
    tree_delete(Pseudo_tree);


    free(info);
    printf("\n");
    printf("analysis done\n");
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
	Output_Pseudo();
	//output 	
}