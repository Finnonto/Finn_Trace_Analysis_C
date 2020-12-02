//
// Trace analysis usage: ./Finn_traceanalysis {trace} {timeinterval}
// @param trace : Compressed or not of pcap is fine.
// @param timeinterval :plese assign this to positive interger (exclude 0)
//

#include "libtrace.h"
#include "trace_analysis.h"

#include <stdio.h>


/*-------xiu---------*/
// default values 
uint32_t K_value = 20;
uint32_t intervalTime = 30; 
uint16_t it = 10;  
uint16_t algorithm = 0; 
uint16_t alg_cnt = 0; 
uint16_t nor_cnt = 0; 
uint16_t arg_tmp = 0;
uint16_t normalization = 0;
uint16_t output_num;
uint16_t file_cnt=0;
uint8_t EXACT = 0;  uint8_t CLIFFORD = 0; uint8_t INVERSE_CLI = 0;
uint8_t ORIGIN = 0; uint8_t TOTAL = 0;    uint8_t DISTINCT = 0;

#include <string.h>
#include <unistd.h> 



int main(int argc, char *argv[])
{
        uint16_t arg_num=2;   //xiu
        char time[10]="30";
        libtrace_t *trace = NULL;
        libtrace_packet_t *packet = NULL;
        Table_Size = 16384; 
        //Table_Amount = 10;
        next_report_time = 0;


        if (argc < 2) {             //xiu

                fprintf(stderr, "Usage: %s inputURI\n", argv[0]);
                return 1;
        }

        //--------------------------xiu--------------------------------------------------
        while(arg_num<argc){
                //-------intervalTime-------
                if(strncmp(argv[arg_num],"-t",2)==0){
                        intervalTime = atoi(argv[arg_num+1]);
                        strcpy(time,argv[arg_num+1]);
                        if(intervalTime<=0)intervalTime=30;
                        arg_num+=2;
                }
                //--------k_value--------
                else if(strncmp(argv[arg_num],"-k",2)==0){
                        K_value = atoi(argv[arg_num+1]);
                        arg_num+=2;
                }
                //------algorithm------  //0:exact  1:clifford  2.inverse_cli 3.all
                else if(strncmp(argv[arg_num],"-a",2)==0){
                        arg_num+=2;
                        arg_tmp=arg_num;
                        alg_cnt=atoi(argv[arg_num-1]);
                        while(arg_num<arg_tmp+alg_cnt)
                        {       
                                if(strncmp(argv[arg_num],"exact",5)==0)EXACT=1;
                                else if(strncmp(argv[arg_num],"clifford",8)==0)CLIFFORD=1;
                                else if(strncmp(argv[arg_num],"inverse_cli",11)==0)INVERSE_CLI=1;
                                else if(strncmp(argv[arg_num],"all",3)==0){EXACT=1;CLIFFORD=1;INVERSE_CLI=1;}
                                else {fprintf(stderr, "algorithm_error%s\n",argv[arg_num]); return 1;}
                                arg_num++;
                        }
                }
                //-----normalization-----  //0:origin 1:distinct 2:total 3:all
                else if(strncmp(argv[arg_num],"-n",2)==0){
                        arg_num+=2;
                        arg_tmp=arg_num;
                        nor_cnt=atoi(argv[arg_num-1]);
                        while(arg_num<arg_tmp+nor_cnt)
                        {       
                                if(strncmp(argv[arg_num],"origin",5)==0)ORIGIN=1;
                                else if(strncmp(argv[arg_num],"distinct",8)==0)DISTINCT=1;
                                else if(strncmp(argv[arg_num],"inverse_cli",11)==0)TOTAL=1;
                                else if(strncmp(argv[arg_num],"all",3)==0){ORIGIN=1;DISTINCT=1;TOTAL=1;}
                                else {fprintf(stderr, "normalization_error%s",argv[arg_num]); return 1;}
                                arg_num++; 
                        }
                }
                //-------table--------
                else if(strncmp(argv[arg_num],"-it",3)==0){
                        if(atoi(argv[arg_num+1])<11 && atoi(argv[arg_num+1])>0)it = atoi(argv[arg_num+1]);
                        else {fprintf(stderr, "it_error"); return 1;}
                        arg_num+=2;
                }
                else arg_num++;
        } 
        if( ORIGIN == 0 && DISTINCT == 0 && TOTAL == 0)
        {
                ORIGIN=1;
                DISTINCT=1;
                TOTAL=1;
        }
        if( EXACT == 0 && CLIFFORD == 0 && INVERSE_CLI == 0)EXACT=1;


        //------------------------xiu-----------------------


        packet = trace_create_packet();
        if (packet == NULL) {
                perror("Creating libtrace packet");
                libtrace_cleanup(trace, packet);
                return 1;
        }

        trace = trace_create(argv[1]);

        if (trace_is_err(trace)) {
                trace_perror(trace,"Opening trace file");
                libtrace_cleanup(trace, packet);
                return 1;
        }

        if (trace_start(trace) == -1) {
                trace_perror(trace,"Starting trace");
                libtrace_cleanup(trace, packet);
                return 1;
        }

         //Filename
        
        strcpy(Output_FileName,"/Analysis_sec_");
        strcat(Output_FileName,time);
        strcat(Output_FileName,"s_");
        strtok(argv[1],"/");
        strtok(NULL,"/");
        strtok(NULL,"/");
        strtok(NULL,"/");
        strcat(Output_FileName,strtok(strtok(NULL,"/"),"."));
   
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
                return 1;
        }

        libtrace_cleanup(trace, packet);
        return 0;
        

}

