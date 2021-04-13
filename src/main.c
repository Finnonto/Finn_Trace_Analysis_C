//
// Distribution(zipf) simulation usage: ./Finn_traceanalysis simulation
//
//
//
// Trace analysis usage: ./Finn_traceanalysis {trace} {param}
// @param trace : Compressed or not of pcap is fine.
// @param param : 
//

#include "libtrace.h"
#include "trace_analysis.h"
#include "simulation.h"
#include <string.h>
#include <unistd.h> 
#include <stdio.h>

void Checkargument(int  argc, char** argv);





void Checkargument(int  argc, char** argv)
{
        
        TRACE = 0;
        SIMULATION = 0;       
        resolution = RAND_MAX;
        // k value fot algorithm 
        K_Value = 20;
        
        // optional argument index  start from two
        if (argc < 2) {             

                fprintf(stderr, "Usage: %s inputURI\n", argv[0]);
                exit(0);
        }
            
                        
        /******************** simulation mode ***************************/
        if(strcmp(argv[1],"simulation")==0)
        {
                
                uint16_t arg_num=2;       
                // simulation mode parameter default value 
                SIMULATION = 1 ;// flag enable
                sim_times = 30;
                sim_error = 1;
                zipf_slen = 100000;
                zipf_range = 10000000;
                zipf_par =  0.8;
                zipf_offset = 4;
                
                
                // algorithm parameter
                it = 1;  //inverse table amount 
                TableIndex = 0 ;//Table index
                Table_Size = 16384; // inverse table size    
                TableINT = 0;         
                alg_cnt = 0; //               
                pingli_alpha = 0.9;
                interpolation_threshold = 10;
                interpolation_span = 4;
                //   algoritm flags init value
                memset(ALG_flag, 0, sizeof(int) * MAX_ALG);
                
                while(arg_num<argc){
                        
                        //*************stream length****************/
                        if(strcmp(argv[arg_num],"-l")==0)
                        {
                                
                                
                                
                                zipf_slen = atoi(argv[++arg_num]);
                                
                                if(zipf_slen <=0)
                                {
                                        fprintf(stderr,"slen must be a positive integer");
                                        exit(0);
                                }
                                
                                arg_num++;
                                
                        }
                        /*************Resolution bit num ********/
                        else if(strcmp(argv[arg_num],"-resolution")==0){
                                
                                if(strcmp(argv[++arg_num],"MAX")==0){
                                        resolution = RAND_MAX;        
                                }
                                else{
                                        resolution = pow(2,atoi(argv[arg_num]));
                                }                
                                if (resolution<=0){fprintf(stderr, "resolution must be positive integer"); exit(0);}
                                arg_num++;
                        }
                        /**************stream data range***********/
                        else if(strcmp(argv[arg_num],"-r")==0)
                        {
                                
                                
                                zipf_range = atoi(argv[++arg_num]);
                                if(zipf_range <=0)
                                {
                                        fprintf(stderr,"range must be a positive integer");
                                        exit(0);
                                }
                                arg_num++;
                                
                        }
                        /***************K value for algorithm*******/
                        else if(strcmp(argv[arg_num],"-k")==0){
                                
                                K_Value = atoi(argv[++arg_num]);
                                
                                if(K_Value<=0)
                                {
                                        fprintf(stderr,"K value must be greater than 0");
                                        exit(0);
                                }
                                arg_num++;
                        }
                        /*************Z value for zipf distribution*/
                        else if(strcmp(argv[arg_num],"-z")==0)
                        {       
                                
                                zipf_par = atof(argv[++arg_num]);
                                arg_num++;
                                
                        }
                        /************experiment times**************/
                        else if(strcmp(argv[arg_num],"-t")==0)
                        {
                                
                                sim_times = atoi(argv[++arg_num]);
                                if(sim_times <=0)
                                {
                                        fprintf(stderr,"simulation times must be a positive integer");
                                        exit(0);
                                }
                                arg_num++;
                        }
                        /*******error probability threhold********/
                        else if(strcmp(argv[arg_num],"-e")==0)
                        {
                                
                                sim_error = atof(argv[++arg_num]);
                                if(sim_error <=0)
                                {
                                        fprintf(stderr,"error probability threshold must be greater than zero");
                                        exit(0);
                                }
                                arg_num++;
                        }
                        /*********stream offset******************/
                        else if(strcmp(argv[arg_num],"-o")==0)
                        {
                                
                                zipf_offset = atoi(argv[++arg_num]);
                                if(zipf_offset <0)
                                {
                                        fprintf(stderr,"zipf distribution offset must be greater be positve integer");
                                        exit(0);
                                }
                                arg_num++;
                        }
                        /***************algorithm*****************/
                        else if(strcmp(argv[arg_num],"-a")==0){
                                
                                alg_cnt = atoi(argv[++arg_num]);
                                
                                int tmp =  arg_num+alg_cnt;
                                
                                
                                while (arg_num<tmp)
                                {       
                                        
                                        ++arg_num; 
                                        
                                        
                                        if(strcmp(argv[arg_num],"exact")==0){ALG_flag[0]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HTo_interpolation_65536")==0){ALG_flag[10]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HTo_interpolation")==0){ALG_flag[9]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_cdf_parallel")==0){ALG_flag[4]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_cdf_parallel_interpolation")==0){ALG_flag[3]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HTo_65536")==0){ALG_flag[8]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_cdf_opt")==0){ALG_flag[5]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_cdf")==0){ALG_flag[2]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HTo")==0){ALG_flag[7]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HT")==0){ALG_flag[6]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford")==0){ALG_flag[1]=1;}
                                        else if(strcmp(argv[arg_num],"PingLi")==0){ALG_flag[11]=1;}
                                        else {
                                                fprintf(stderr, "algorithm name error:%s\n",argv[arg_num]);
                                                exit(0);}
                                }       
                                arg_num++;
                                
                             
                                
                        }
                        /*************inverse table amount********/
                        else if(strcmp(argv[arg_num],"-it")==0){
                                if(atoi(argv[arg_num+1])<21 && atoi(argv[arg_num+1])>0)it = atoi(argv[++arg_num]);
                                else {fprintf(stderr, "it_error"); exit(0);}
                                arg_num++;
                        }
                        /*************CDF table size********/
                        else if(strcmp(argv[arg_num],"-Tbs")==0){
                                int tmp = atoi(argv[++arg_num]);
                                if( tmp == 1024 || tmp== 16384 || tmp==65536 || tmp==32768 || tmp==4096)Table_Size = tmp;
                                else {fprintf(stderr, "Table size errror 4096 16384 ,65536,32768"); exit(0);}
                                arg_num++;
                        }
                        /*************PingLi alpha ********/
                        else if(strcmp(argv[arg_num],"-PA")==0){
                                pingli_alpha = atof(argv[++arg_num]);
                                if (pingli_alpha>=1){fprintf(stderr, "alpha must be less than 1"); exit(0);}
                                arg_num++;
                        }
                        /*************table index to start loading table ********/
                        else if(strcmp(argv[arg_num],"-tbidx")==0){
                                TableIndex = atoi(argv[++arg_num]);
                                if (TableIndex>10 || TableIndex<0){fprintf(stderr, "Table index must be between 0,1"); exit(0);}
                                arg_num++;
                        }
                        /*************index to choose integer table********/
                        else if(strcmp(argv[arg_num],"-tbint")==0){
                                TableINT = atoi(argv[++arg_num]);
                                
                                //if (TableINT !=1 || TableINT !=0){fprintf(stderr, "Table INT index must be 0 or 1\n"); exit(0);}
                                arg_num++;
                        }
                        /*************threshold of interpolation********/
                        else if(strcmp(argv[arg_num],"-interth")==0){
                                interpolation_threshold = atoi(argv[++arg_num]);
                                
                                //if (TableINT !=1 || TableINT !=0){fprintf(stderr, "Table INT index must be 0 or 1\n"); exit(0);}
                                arg_num++;
                        }
                        /*************span value of interpolation********/
                        else if(strcmp(argv[arg_num],"-interspan")==0){
                                interpolation_span = atoi(argv[++arg_num]);
                                
                                //if (TableINT !=1 || TableINT !=0){fprintf(stderr, "Table INT index must be 0 or 1\n"); exit(0);}
                                arg_num++;
                        }
                        /*************raising exception********/
                        else 
                        {
                                printf("There is no argument %s ,please check again\n",argv[arg_num]);
                                exit(0);
                        }
                 }
                 
        }
        /******************** trace analysis mode ***********************/ 
        else if(strcmp(argv[1],"trace")==0) {
                uint16_t arg_num=3;
                // tace analysis mode parameters default value 
                sprintf(TraceName,"%s",argv[2]);
                TRACE = 1;                
                // to init the time interval
                next_report_time = 0;
                intervalTime = 30;                 

                // algorithm parameter
                it = 1;  //inverse table amount 
                TableIndex = 0 ;//Table index
                Table_Size = 16384; // inverse table size    
                TableINT = 0;         
                alg_cnt = 0; //               
                pingli_alpha = 0.9;
                interpolation_threshold = 10;
                interpolation_span = 4;
                                     
                
                while(arg_num<argc){
                        //-------intervalTime-------// (seconds)
                        if(strcmp(argv[arg_num],"-t")==0){
                                intervalTime = atoi(argv[arg_num+1]);
                                if(intervalTime<=0)
                                {
                                        fprintf(stderr,"interval time must be greater than 0, default value is 30(sec)\n");
                                        exit(0);
                                }
                                arg_num+=2;
                        }
                        //--------k_value--------// must be positive integer 
                        else if(strcmp(argv[arg_num],"-k")==0){
                                K_Value = atoi(argv[arg_num+1]);
                                if(K_Value<=0)
                                {
                                        fprintf(stderr,"K value must be greater than 0\n");
                                        exit(0);
                                }
                                arg_num+=2;
                        }
                        //-------table--------// must be greater than 1 ,at least 1 external table 
                        else if(strcmp(argv[arg_num],"-it")==0){
                                if(atoi(argv[arg_num+1])<21 && atoi(argv[arg_num+1])>0)it = atoi(argv[arg_num+1]);
                                else {fprintf(stderr, "it_error\n"); exit(0);}
                                arg_num+=2;
                        }
                        /************help********************/
                        else if(strcmp(argv[arg_num],"-Tbs")==0){
                                int tmp = atoi(argv[++arg_num]);
                                if(tmp== 16384 || tmp==65536 || tmp==32768 || tmp==4096)Table_Size = tmp;
                                else {fprintf(stderr, "Table size errror 4096 16384 ,65536,32768\n"); exit(0);}
                                arg_num++;
                        }
                        /*************PingLi alpha ********/
                        else if(strcmp(argv[arg_num],"-PA")==0){
                                pingli_alpha = atof(argv[++arg_num]);
                                if (pingli_alpha>=1){fprintf(stderr, "alpha must be less than 1\n"); exit(0);}
                                arg_num++;
                        }
                        /***************algorithm*****************/
                        else if(strcmp(argv[arg_num],"-a")==0){
                                
                                alg_cnt = atoi(argv[++arg_num]);
                                
                                int tmp =  arg_num+alg_cnt;
                                
                                
                                while (arg_num<tmp)
                                {       
                                        
                                        ++arg_num; 
                                        
                                        
                                        if(strcmp(argv[arg_num],"exact")==0){ALG_flag[0]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HTo_interpolation_65536")==0){ALG_flag[10]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HTo_interpolation")==0){ALG_flag[9]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_cdf_parallel")==0){ALG_flag[4]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_cdf_parallel_interpolation")==0){ALG_flag[3]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HTo_65536")==0){ALG_flag[8]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_cdf_opt")==0){ALG_flag[5]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_cdf")==0){ALG_flag[2]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HTo")==0){ALG_flag[7]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford_HT")==0){ALG_flag[6]=1;}
                                        else if(strcmp(argv[arg_num],"Clifford")==0){ALG_flag[1]=1;}
                                        else if(strcmp(argv[arg_num],"PingLi")==0){ALG_flag[11]=1;}
                                        else {
                                                fprintf(stderr, "algorithm name error:%s\n",argv[arg_num]);
                                                exit(0);}
                                }       
                                arg_num++;
                                
                             
                                
                        }
                        /*************Resolution bit num ********/
                        else if(strcmp(argv[arg_num],"-resolution")==0){
                                
                                if(strcmp(argv[++arg_num],"MAX")==0){
                                        resolution = RAND_MAX;        
                                }
                                else{
                                        resolution = pow(2,atoi(argv[arg_num]));
                                }                
                                if (resolution<=0){fprintf(stderr, "resolution must be positive integer"); exit(0);}
                                arg_num++;
                        }
                        /*************threshold of interpolation********/
                        else if(strcmp(argv[arg_num],"-tbidx")==0){
                                TableIndex = atoi(argv[++arg_num]);
                                if (TableIndex>10 || TableIndex<0){fprintf(stderr, "Table index must be between 0,1"); exit(0);}
                                arg_num++;
                        }
                        /*************index to choose integer table********/
                        else if(strcmp(argv[arg_num],"-tbint")==0){
                                TableINT = atoi(argv[++arg_num]);
                                
                                //if (TableINT !=1 || TableINT !=0){fprintf(stderr, "Table INT index must be 0 or 1\n"); exit(0);}
                                arg_num++;
                        }
                        /*************threshold of interpolation********/
                        else if(strcmp(argv[arg_num],"-interth")==0){
                                interpolation_threshold = atoi(argv[++arg_num]);
                                
                                //if (TableINT !=1 || TableINT !=0){fprintf(stderr, "Table INT index must be 0 or 1\n"); exit(0);}
                                arg_num++;
                        }
                        /*************span value of interpolation********/
                        else if(strcmp(argv[arg_num],"-interspan")==0){
                                interpolation_span = atoi(argv[++arg_num]);
                                
                                //if (TableINT !=1 || TableINT !=0){fprintf(stderr, "Table INT index must be 0 or 1\n"); exit(0);}
                                arg_num++;
                        }
                        else {
                                fprintf(stderr,"input parameter error check --help");
                                exit(0);
                        }
                } 

        }
        else{
                fprintf(stderr,"'simulation' or 'trace' ?");
                exit(0);
        }
}

int main(int argc, char *argv[])
{
        
        fprintf(stderr,
	  "____________________________________________________________\n");
        fprintf(stderr,
                "%s compiled at %s, %s\n", __FILE__, __TIME__, __DATE__);
               
        Checkargument(argc,argv);
        Output_log();
        // if TRACE flag is enabled ,do tace analysis
        if(TRACE)
        {
                printf("trace analysis start: k:%d PA:%g Timeinval:%d it:%d Table Index: %d Table Size:%d resolution:%d interth:%d interspan:%d\n",
                K_Value,pingli_alpha,intervalTime,it,TableIndex,Table_Size,resolution,interpolation_threshold,interpolation_span);
                Trace_processing(argv[2]);
        }
        // if SIMULATION flag is enabled ,do zipf distribution simulation
        else if(SIMULATION)
        {
                printf("sim start: z:%g length:%d range:%d offset:%d %dTables Table Index:%d Table Size:%d resolution:%d interth:%d interspan:%d\n"
                ,zipf_par,zipf_slen,zipf_range,zipf_offset,it,TableIndex,Table_Size,resolution,interpolation_threshold,interpolation_span);
        
                Simulation_processing();

        }
        
        return 0;
        

}

