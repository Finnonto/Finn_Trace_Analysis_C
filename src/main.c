//
// Trace analysis usage: ./Finn_traceanalysis {trace} {timeinterval}
// @param trace : Compressed or not of pcap is fine.
// @param timeinterval :plese assign this to positive interger (exclude 0)
//


#include "libtrace.h"
#include "trace_analysis.h"

#include <stdio.h>



int main(int argc, char *argv[])
{
        
        
        libtrace_t *trace = NULL;
        libtrace_packet_t *packet = NULL;
        next_report_time = 0;
        strcpy(Ouput_FileName,"./output/");
        

        if (argc < 4) {
                fprintf(stderr, "Usage: %s inputURI\n", argv[0]);
                return 1;
        }

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

        invalTime = strtoul(argv[3],NULL,10);
        
        if(invalTime<=0)
        {
                invalTime = 1;
        }

        strcat(Ouput_FileName,argv[2]);
       
        Output_Init();
        
        while (trace_read_packet(trace,packet)>0) {
                
                per_packet(packet);
                
        }
        Items_Processing();     
        Close_Output_File();


        if (trace_is_err(trace)) {
                trace_perror(trace,"Reading packets");
                libtrace_cleanup(trace, packet);
                return 1;
        }

        libtrace_cleanup(trace, packet);
        return 0;
}

