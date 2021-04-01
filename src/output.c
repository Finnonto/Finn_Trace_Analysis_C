#include "output.h"


void Output_write_file(char* Output_filename,int index)
{
    FILE* fp;
    
    if( access( Output_filename, F_OK ) == 0 ) 
	{
    	fp = fopen(Output_filename,"a");
	} 	
	else 
	{
   		fp = fopen(Output_filename,"w");
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
						fprintf(fp,"Clifford_cdf_parallel_entropy,");
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
        
        fprintf(fp,"Distinct Count,Total Len,Table Size,resolution\n");
        
		}

	
	for(int i=0;i<ent_cnt;i++)
	{
        
		fprintf(fp,"%.30lf,",exact_entropy[index][i]);
    
    	for(int j=1;j<MAX_ALG;j++)
		{
			if(ALG_flag[j]==1)
			{
				switch (j)
				{
					case 1:
						fprintf(fp,"%.30lf,",Clifford_entropy[index][i]);
						break;
					
					case 2:
						fprintf(fp,"%.30lf,",Clifford_cdf_entropy[index][i]);
						break;
					
					case 3:
						fprintf(fp,"%.30lf,",Clifford_cdf_stage50_entropy[index][i]);
						break;
					
					case 4:
						fprintf(fp,"%.30lf,",Clifford_cdf_parallel_entropy[index][i]);
						break;
					
					case 5:
						fprintf(fp,"%.30lf,",Clifford_cdf_opt_entropy[index][i]);
						break;
					
					case 6:
						fprintf(fp,"%.30lf,",Clifford_HT_entropy[index][i]);
						break;
					
					case 7:
						fprintf(fp,"%.30lf,",Clifford_HTo_entropy[index][i]);
						break;
					
					case 8:
						fprintf(fp,"%.30lf,",Clifford_HTo_65536_entropy[index][i]);
						break;
					
					case 9:
						fprintf(fp,"%.30lf,",Clifford_HTo_interpolation_entropy[index][i]);
						break;
					
					case 10:
						fprintf(fp,"%.30lf,",Clifford_HTo_interpolation_65536_entropy[index][i]);
						break;
					
					case 11:						
						fprintf(fp,"%.30lf,",PingLi_entropy[index][i]);
						break;
				}
			}
		}
    
		fprintf(fp,"%d,%d,%d,%d \n",
								StreamDistinct[index][i],
								StreamLength[index][i],
								Table_Size,
								resolution
								);	
	}



	fclose(fp);

}


void create_folder()
{
    
    struct stat st = {0};
    
    if (TRACE)
    {   
        


        strcpy(Output_FolderName,"./output/Trace/");
        if (stat(Output_FolderName, &st) == -1) 
        {
            mkdir(Output_FolderName, 0700);
        }
        
        //to create a subfolder for different trace
        char *tmp1,*tmp2;
        
		tmp1 = strtok(Trace_Path,"/");
        while(tmp2!=NULL)
        {
            tmp1 = tmp2;
            tmp2 = strtok(NULL,"/");
        }
        
        strcat(Output_FolderName,strtok(tmp1,"."));
        if (stat(Output_FolderName, &st) == -1) 
        {
            mkdir(Output_FolderName, 0700);
        }
        strcat(Output_FolderName,"/");

        //to create several subfolder for tuples
        //SrcIP
        strcpy(Output_SrcIPName,Output_FolderName);
        strcat(Output_SrcIPName,"SrcIP/");
        if (stat(Output_SrcIPName, &st) == -1) 
        {
            mkdir(Output_SrcIPName, 0700);
        }
        
        //DstIP
        strcpy(Output_DstIPName,Output_FolderName);
        strcat(Output_DstIPName,"DstIP/");
        if (stat(Output_DstIPName, &st) == -1) 
        {
            
            mkdir(Output_DstIPName, 0700);
        }
        //SrcPort
        strcpy(Output_SrcPortName,Output_FolderName);
        strcat(Output_SrcPortName,"SrcPort/");
        if (stat(Output_SrcPortName, &st) == -1) 
        {
            
            mkdir(Output_SrcPortName, 0700);
        }
        //DstPort
        strcpy(Output_DstPortName,Output_FolderName);
        strcat(Output_DstPortName,"DstPort/");
        if (stat(Output_DstPortName, &st) == -1) 
        {
            
            mkdir(Output_DstPortName, 0700);
        }
        //PktLen
        strcpy(Output_PktLenName,Output_FolderName);
        strcat(Output_PktLenName,"PktLen/");
        if (stat(Output_PktLenName, &st) == -1) 
        {
            
            mkdir(Output_PktLenName, 0700);
        }
        



    }
    if(SIMULATION)
    {
        strcpy(Output_FolderName,"./output/Simulation/");
        if (stat(Output_FolderName, &st) == -1) 
        {
            mkdir(Output_FolderName, 0700);
        }
    }    
    
}


void Output_Simulation()
{
    
    create_folder();
	
    
	sprintf(filename,"%d_%.1f_%d_%d.csv",zipf_slen,zipf_par,zipf_range,resolution);
	strcat(Output_FolderName,filename);
    
    Output_write_file(Output_FolderName,0);

}


void Output_Trace()
{
    create_folder();
   
	
    
	sprintf(filename,"%d_sec_%d_%d.csv",intervalTime,resolution,Table_Size);
	
    //output 5 tuple 
    
    strcat(Output_SrcIPName,filename);
    Output_write_file(Output_SrcIPName,0);

    strcat(Output_DstIPName,filename);
    Output_write_file(Output_DstIPName,1);

    strcat(Output_SrcPortName,filename);
    Output_write_file(Output_SrcPortName,2);

    strcat(Output_DstPortName,filename);
    Output_write_file(Output_DstPortName,3);

    strcat(Output_PktLenName,filename);
    Output_write_file(Output_PktLenName,4);

}


void Output_log()
{
	FILE* fp;
    
	char logfilename[CHAR_LEN]	= "./ArgLog.txt";
    if( access( logfilename, F_OK ) == 0 ) 
	{
    	fp = fopen(logfilename,"a");
	} 	
	else 
	{
   		fp = fopen(logfilename,"w");
	}
	
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	if(SIMULATION){
		
		fprintf(fp,"simulation mode \n");
		fprintf(fp,"resolution:%d\n",resolution);
		fprintf(fp,"z:%g length:%d range:%d offset:%d \n",zipf_par,zipf_slen,zipf_range,zipf_offset);
		fprintf(fp,"Table Size:%d it:%d K_vlaue:%d\n",Table_Size,it,K_Value);
		fprintf(fp,"algorithm:");
		for(int i=0;i<MAX_ALG;i++){
			if(ALG_flag[i]==1){
				switch (i)
				{
				case 0:
					fprintf(fp,"exact,");
					break;
				case 1:
					fprintf(fp,"Clifford,");
					break;
				case 2:
					fprintf(fp,"Clifford_cdf,");
					break;
				case 3:
					fprintf(fp,"Clifford_cdf_stage50,");
					break;
				case 4:
					fprintf(fp,"Clifford_cdf_parallel,");
					break;
				case 5:
					fprintf(fp,"Clifford_cdf_opt,");
					break;
				case 6:
					fprintf(fp,"Clifford_HT,");
					break;
				case 7:
					fprintf(fp,"Clifford_HTo,");
					break;
				case 8:
					fprintf(fp,"Clifford_HTo_65536,");
					break;
				case 9:
					fprintf(fp,"Clifford_HTo_interpolation,");
					break;
				case 10:
					fprintf(fp,"Clifford_HTo_interpolation_65536,");
					break;
				case 11:
					fprintf(fp,"PingLi\n");
					break;
				default:
					break;
				}
			}
		}
	}
	else if(TRACE){
		fprintf(fp,"trace mode \n");
		fprintf(fp,"resolution:%d\n",resolution);
		fprintf(fp,"Timeinval:%d\n",intervalTime);
		fprintf(fp,"Trace: %s\n",TraceName);
		fprintf(fp,"Table Size:%d it:%d K_vlaue:%d PA:%lf\n",Table_Size,it,K_Value,pingli_alpha);
		fprintf(fp,"algorithm:");
		fprintf(fp,"algorithm:");
		for(int i=0;i<MAX_ALG;i++){
			if(ALG_flag[i]==1){
				switch (i)
				{
				case 0:
					fprintf(fp,"exact,");
					break;
				case 1:
					fprintf(fp,"Clifford,");
					break;
				case 2:
					fprintf(fp,"Clifford_cdf,");
					break;
				case 3:
					fprintf(fp,"Clifford_cdf_stage50,");
					break;
				case 4:
					fprintf(fp,"Clifford_cdf_parallel,");
					break;
				case 5:
					fprintf(fp,"Clifford_cdf_opt,");
					break;
				case 6:
					fprintf(fp,"Clifford_HT,");
					break;
				case 7:
					fprintf(fp,"Clifford_HTo,");
					break;
				case 8:
					fprintf(fp,"Clifford_HTo_65536,");
					break;
				case 9:
					fprintf(fp,"Clifford_HTo_interpolation,");
					break;
				case 10:
					fprintf(fp,"Clifford_HTo_interpolation_65536,");
					break;
				case 11:
					fprintf(fp,"PingLi\n");
					break;
				default:
					break;
				}
			}
		}
	}
	
	printf("log done \n");

}

void Output_MAPE()
{
	FILE* fp;
	sprintf(MAPE_filename,"./output/Simulation/MAPE_%d_%.1f_%d_%d.csv",zipf_slen,zipf_par,zipf_range,resolution);
	if( access( MAPE_filename, F_OK ) == 0 ) 
	{
    	fp = fopen(MAPE_filename,"a");
	} 	
	else 
	{
   		fp = fopen(MAPE_filename,"w");
	}

	for(int i=1;i<MAX_ALG;i++)
	{
		if(ALG_flag[i]==1)
		{
			switch (i)
			{
				case 1:
					fprintf(fp,"Clifford_MAPE,");
					break;
			
				case 2:
					fprintf(fp,"Clifford_cdf_MAPE,");
					break;
				
				case 3:
					fprintf(fp,"Clifford_cdf_stage50_MAPE,");
					break;
				
				case 4:
					fprintf(fp,"Clifford_cdf_parallel_MAPE,");
					break;
				
				case 5:
					fprintf(fp,"Clifford_cdf_opt_MAPE,");
					break;
				
				case 6:
					fprintf(fp,"Clifford_HT_MAPE,");
					break;
				
				case 7:
					fprintf(fp,"Clifford_HTo_MAPE,");
					break;
				
				case 8:
					fprintf(fp,"Clifford_HTo_65536_MAPE,");
					break;
				
				case 9:
					fprintf(fp,"Clifford_HTo_interpolation_MAPE,");
					break;
				
				case 10:
					fprintf(fp,"Clifford_HTo_interpolation_65536_MAPE,");
					break;
				
				case 11:
					fprintf(fp,"PingLi_MAPE,");
					break;
			}
		}
	}

	fprintf(fp,"\n");

	for(int j=1;j<MAX_ALG;j++)
	{
		if(ALG_flag[j]==1)
		{
			switch (j)
			{
				case 0:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 1:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 2:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 3:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 4:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 5:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 6:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 7:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 8:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 9:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 10:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
				case 11:
					fprintf(fp,"%.30lf,",MAPE[0][j]);
					break;
			}
		}
	}
	fprintf(fp,"\n");

	fclose(fp);
}



