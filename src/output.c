#include "output.h"

void Output_Init()
{
    strcpy(CSV,".csv");
    Output_File = fopen(strcat(Ouput_FileName,CSV),"w");
    fprintf(Output_File,"time(sec),");
    fprintf(Output_File,"SrcIP_entropy,");
    fprintf(Output_File,"SrcIP_total_cnt,");
    fprintf(Output_File,"SrcIP_distinct,");
    fprintf(Output_File,"DesIP_entropy,");
    fprintf(Output_File,"DesIP_total_cnt,");
    fprintf(Output_File,"DesIP_distinct,");
    fprintf(Output_File,"SrcPort_entropy,");
    fprintf(Output_File,"SrcPort_total_cnt,");
    fprintf(Output_File,"SrcPort_distinct,");
    fprintf(Output_File,"DesPort_entropy,");
    fprintf(Output_File,"DesPort_total_cnt,");
    fprintf(Output_File,"DesPort_distinct,");
    fprintf(Output_File,"PktLe_entropy,");
    fprintf(Output_File,"PktLe_total_cnt,");
    fprintf(Output_File,"PktLe_distinct,");
    fprintf(Output_File,"\n");

}

void output_entropy_csv(trace_info_t *info ,uint32_t ct )
{
    fprintf(Output_File,"%u,",ct);
    fprintf(Output_File,"%lf,",info[0].entropy);
    fprintf(Output_File,"%u,",info[0].total_count);
    fprintf(Output_File,"%u,",info[0].distinct);

    fprintf(Output_File,"%lf,",info[1].entropy);
    fprintf(Output_File,"%u,",info[1].total_count);
    fprintf(Output_File,"%u,",info[1].distinct);

    fprintf(Output_File,"%lf,",info[2].entropy);
    fprintf(Output_File,"%u,",info[2].total_count);
    fprintf(Output_File,"%u,",info[2].distinct);

    fprintf(Output_File,"%lf,",info[3].entropy);
    fprintf(Output_File,"%u,",info[3].total_count);
    fprintf(Output_File,"%u,",info[3].distinct);

    fprintf(Output_File,"%lf,",info[4].entropy);
    fprintf(Output_File,"%u,",info[4].total_count);
    fprintf(Output_File,"%u,",info[4].distinct);
    fprintf(Output_File,"\n");    

}

void Close_Output_File()
{
    fclose(Output_File);
}