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
        // k value fot algorithm 
        K_Value = 20;
        
        // optional argument index  start from two
        if (argc < 2) {             

                fprintf(stderr, "Usage: %s inputURI\n", argv[0]);
                exit(0);
        }

        uint16_t arg_num=2;
        
        /******************** simulation mode ***************************/
        if(strncmp(argv[1],"simulation",10)==0)
        {
                
                
                // simulation mode parameter default value 
                SIMULATION = 1 ;// flag enable
                m_KLD = 0;
                sim_times = 30;
                sim_error = 1;
                zipf_slen = 100000;
                zipf_range = 10000000;
                zipf_par =  0.8;
                zipf_offset = 4;

                // simulation paramters
                it = 10;  //inverse table amount 
                Table_Size = 16384; // inverse table size

                algorithm = 0; //algorithm amount 
                alg_cnt = 0; // 
                nor_cnt = 0; 
                normalization = 0;
                
                //   algoritm flags init value
                EXACT = 0;  
                CLIFFORD = 0; 
                INVERSE_CLI = 0;
               
                while(arg_num<argc){
                        
                        //*************stream length****************/
                        if(strncmp(argv[arg_num],"-l",2)==0)
                        {
                                
                                
                                
                                zipf_slen = atoi(argv[++arg_num]);
                                
                                if(zipf_slen <=0)
                                {
                                        fprintf(stderr,"slen must be a positive integer");
                                        exit(0);
                                }
                                
                                arg_num++;
                                
                        }
                        /**************stream data range***********/
                        else if(strncmp(argv[arg_num],"-r",2)==0)
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
                        else if(strncmp(argv[arg_num],"-k",2)==0){
                                
                                K_Value = atoi(argv[++arg_num]);
                                if(K_Value<=0)
                                {
                                        fprintf(stderr,"K value must be greater than 0");
                                        exit(0);
                                }
                                arg_num++;
                        }
                        /*************Z value for zipf distribution*/
                        else if(strncmp(argv[arg_num],"-z",2)==0)
                        {       
                                
                                zipf_par = atof(argv[++arg_num]);
                                arg_num++;
                                
                        }
                        /************experiment times**************/
                        else if(strncmp(argv[arg_num],"-t",2)==0)
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
                        else if(strncmp(argv[arg_num],"-e",2)==0)
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
                        else if(strncmp(argv[arg_num],"-o",2)==0)
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
                        else if(strncmp(argv[arg_num],"-a",2)==0){
                                alg_cnt = atoi(argv[++arg_num]);
                                ++arg_num;
                                for(int i = arg_num;i< arg_num+alg_cnt;++i)
                                {       
                                        if(strncmp(argv[arg_num],"exact",5)==0)EXACT=1;
                                        else if(strncmp(argv[arg_num],"clifford",8)==0)CLIFFORD=1;
                                        else if(strncmp(argv[arg_num],"inverse_cli",11)==0)INVERSE_CLI=1;
                                        else if(strncmp(argv[arg_num],"all",3)==0){EXACT=1;CLIFFORD=1;INVERSE_CLI=1;}
                                        else {fprintf(stderr, "algorithm_error%s\n",argv[arg_num]); exit(0);}
                                        
                                }
                                arg_num += alg_cnt;;
                        }
                        /*************inverse table amount********/
                        else if(strncmp(argv[arg_num],"-it",3)==0){
                                if(atoi(argv[arg_num+1])<11 && atoi(argv[arg_num+1])>0)it = atoi(argv[arg_num+1]);
                                else {fprintf(stderr, "it_error"); exit(0);}
                                arg_num+=2;
                        }
                        else if(strncmp(argv[arg_num],"-KLD",3)==0){
                                m_KLD = 1;
                                arg_num+=2;
                        }
                 }
                 
        }
        /******************** trace analysis mode ***********************/ 
        else{

                // tace analysis mode parameters default value 
                TRACE = 1;
                
                // to init the time interval
                next_report_time = 0;
                intervalTime = 30; 

                it = 10;  //inverse table amount 
                Table_Size = 16384; // inverse table size

                algorithm = 0; //algorithm amount 
                alg_cnt = 0; // 
                nor_cnt = 0; 
                normalization = 0;
                        
                file_cnt=0;
                //   algoritm flags init value
                EXACT = 0;  
                CLIFFORD = 0; 
                INVERSE_CLI = 0;
                // normalization flags init value
                ORIGIN = 0; 
                TOTAL = 0;    
                DISTINCT = 0;

                
                while(arg_num<argc){
                        //-------intervalTime-------// (seconds)
                        if(strncmp(argv[arg_num],"-t",2)==0){
                                intervalTime = atoi(argv[arg_num+1]);
                                if(intervalTime<=0)
                                {
                                        fprintf(stderr,"interval time must be greater than 0, default value is 30(sec)");
                                        exit(0);
                                }
                                arg_num+=2;
                        }
                        //--------k_value--------// must be positive integer 
                        else if(strncmp(argv[arg_num],"-k",2)==0){
                                K_Value = atoi(argv[arg_num+1]);
                                if(K_Value<=0)
                                {
                                        fprintf(stderr,"K value must be greater than 0");
                                        exit(0);
                                }
                                arg_num+=2;
                        }
                        //------algorithm------  // {number} {algorithm ... }  0:exact  1:clifford  2.inverse_cli 3.all
                        else if(strncmp(argv[arg_num],"-a",2)==0){
                                alg_cnt = atoi(argv[++arg_num]);
                                ++arg_num;
                                for(int i = arg_num;i< arg_num+alg_cnt;++i)
                                {       
                                        if(strncmp(argv[arg_num],"exact",5)==0)EXACT=1;
                                        else if(strncmp(argv[arg_num],"clifford",8)==0)CLIFFORD=1;
                                        else if(strncmp(argv[arg_num],"inverse_cli",11)==0)INVERSE_CLI=1;
                                        else if(strncmp(argv[arg_num],"all",3)==0){EXACT=1;CLIFFORD=1;INVERSE_CLI=1;}
                                        else {fprintf(stderr, "algorithm_error%s\n",argv[arg_num]); exit(0);}
                                        
                                }
                                arg_num += alg_cnt;;
                        }
                        //-----normalization-----  // {number} {normalization ... } 0:origin 1:distinct 2:total 3:all
                        else if(strncmp(argv[arg_num],"-n",2)==0){
                                nor_cnt = atoi(argv[++arg_num]);
                                ++arg_num;
                                for(int i = arg_num;i< arg_num+nor_cnt;++i)
                                {       
                                        if(strncmp(argv[arg_num],"origin",5)==0)ORIGIN=1;
                                        else if(strncmp(argv[arg_num],"distinct",8)==0)DISTINCT=1;
                                        else if(strncmp(argv[arg_num],"inverse_cli",11)==0)TOTAL=1;
                                        else if(strncmp(argv[arg_num],"all",3)==0){ORIGIN=1;DISTINCT=1;TOTAL=1;}
                                        else {fprintf(stderr, "normalization_error%s",argv[arg_num]); exit(0);}
                                        
                                }
                                arg_num += nor_cnt;
                        }
                        //-------table--------// must be greater than 1 ,at least 1 external table 
                        else if(strncmp(argv[arg_num],"-it",3)==0){
                                if(atoi(argv[arg_num+1])<11 && atoi(argv[arg_num+1])>0)it = atoi(argv[arg_num+1]);
                                else {fprintf(stderr, "it_error"); exit(0);}
                                arg_num+=2;
                        }
                        /************help********************/
                        else 
                        {
                                fprintf(stderr,"input parameter error check --help");
                                exit(0);
                        }
                } 


                // Flags default value 
                if( ORIGIN == 0 && DISTINCT == 0 && TOTAL == 0)
                {
                        ORIGIN=1;
                        DISTINCT=1;
                        TOTAL=1;
                }
                if( EXACT == 0 && CLIFFORD == 0 && INVERSE_CLI == 0)EXACT=1;
        }
        
}

int main(int argc, char *argv[])
{
        fprintf(stderr,
	  "____________________________________________________________\n");
        fprintf(stderr,
                "%s running at %s, %s\n", __FILE__, __TIME__, __DATE__);

        Checkargument(argc,argv);
        // if TRACE flag is enabled ,do tace analysis
        if(TRACE)
        {
                Trace_processing(argv[1]);
        }
        // if SIMULATION flag is enabled ,do zipf distribution simulation
        else if(SIMULATION)
        {
                printf("sim start: z:%g length:%d range:%d \n",zipf_par,zipf_slen,zipf_range);
                Simulation_processing();

        }
        
        return 0;
        

}

