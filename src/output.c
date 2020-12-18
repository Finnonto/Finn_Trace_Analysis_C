#include "output.h"


void create_folder()
{
    char Output_FolderName[100];
    if (EXACT)
    {
        strcpy(Output_FolderName,"./output/Exact");
        mkdir(Output_FolderName,0777);
        NORfolder(Output_FolderName);
    }
    if (CLIFFORD)
    {
        strcpy(Output_FolderName,"./output/Clifford");
        mkdir(Output_FolderName,0777);
        NORfolder(Output_FolderName);
    }
    if (INVERSE_CLI)
    {
        strcpy(Output_FolderName,"./output/Inverse_cli");
        mkdir(Output_FolderName,0777);
        NORfolder(Output_FolderName);
    }
    file_cnt=0;
}

void NORfolder( char *Output_FolderName)
{   
    char ALGFolderName[100];
    if (ORIGIN)
    {
        strcpy(ALGFolderName,Output_FolderName);
        strcat(ALGFolderName,"/origin");
        mkdir(ALGFolderName,0777);
        Output_Init(ALGFolderName);
    }
    if (DISTINCT)
    {
        strcpy(ALGFolderName,Output_FolderName);
        strcat(ALGFolderName,"/distinct");
        mkdir(ALGFolderName,0777);
        Output_Init(ALGFolderName);
    }
    if (TOTAL)
    {
        strcpy(ALGFolderName,Output_FolderName);
        strcat(ALGFolderName,"/total");
        mkdir(ALGFolderName,0777);
        Output_Init(ALGFolderName);
    }
} 

void Output_Init(char *FolderName)
{
    char time[10];
    char *tmp1,*tmp2;
    strcpy(CSV,".csv");   //csv=.csv
    
    // make output file name 
    strcpy(Output_FileName,"/Analysis_sec_");
    sprintf(time,"%d",intervalTime);
    strcat(Output_FileName,time);
    strcat(Output_FileName,"s_");
    tmp1 = strtok(Trace_Path,"/");
    while(tmp2!=NULL)
    {
        tmp1 = tmp2;
        tmp2 = strtok(NULL,"/");
    }
    strcat(Output_FileName,strtok(tmp1,"."));


    strcat(FolderName,Output_FileName);
    Output_File=fopen(strcat(FolderName,CSV),"w");
    filelist[file_cnt] = Output_File;
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
    file_cnt++;
}

void output()
{

    if (ORIGIN)
    {
        Output_File=filelist[file_cnt];
        output_entropy_csv_origin();
        file_cnt++;
    }
    if (DISTINCT)
    {
        Output_File=filelist[file_cnt];
        output_entropy_csv_distinct();
        file_cnt++;
    }
    if (TOTAL)
    {
        Output_File=filelist[file_cnt];
        output_entropy_csv_total();
        file_cnt++;
    }
}

void output_entropy_csv_origin()
{
    fprintf(Output_File,"%u,",Current_time);
    fprintf(Output_File,"%lf,",Info_list[0].entropy);
    fprintf(Output_File,"%u,",Info_list[0].total_count);
    fprintf(Output_File,"%u,",Info_list[0].distinct);

    fprintf(Output_File,"%lf,",Info_list[1].entropy);
    fprintf(Output_File,"%u,",Info_list[1].total_count);
    fprintf(Output_File,"%u,",Info_list[1].distinct);

    fprintf(Output_File,"%lf,",Info_list[2].entropy);
    fprintf(Output_File,"%u,",Info_list[2].total_count);
    fprintf(Output_File,"%u,",Info_list[2].distinct);

    fprintf(Output_File,"%lf,",Info_list[3].entropy);
    fprintf(Output_File,"%u,",Info_list[3].total_count);
    fprintf(Output_File,"%u,",Info_list[3].distinct);

    fprintf(Output_File,"%lf,",Info_list[4].entropy);
    fprintf(Output_File,"%u,",Info_list[4].total_count);
    fprintf(Output_File,"%u,",Info_list[4].distinct);
    fprintf(Output_File,"\n");    

}

void output_entropy_csv_distinct()
{
    fprintf(Output_File,"%u,",Current_time);
    fprintf(Output_File,"%lf,",Info_list[0].entropy/log(Info_list[0].distinct));
    fprintf(Output_File,"%u,",Info_list[0].total_count);
    fprintf(Output_File,"%u,",Info_list[0].distinct);

    fprintf(Output_File,"%lf,",Info_list[1].entropy/log(Info_list[1].distinct));
    fprintf(Output_File,"%u,",Info_list[1].total_count);
    fprintf(Output_File,"%u,",Info_list[1].distinct);

    fprintf(Output_File,"%lf,",Info_list[2].entropy/log(Info_list[2].distinct));
    fprintf(Output_File,"%u,",Info_list[2].total_count);
    fprintf(Output_File,"%u,",Info_list[2].distinct);

    fprintf(Output_File,"%lf,",Info_list[3].entropy/log(Info_list[3].distinct));
    fprintf(Output_File,"%u,",Info_list[3].total_count);
    fprintf(Output_File,"%u,",Info_list[3].distinct);

    fprintf(Output_File,"%lf,",Info_list[4].entropy/log(Info_list[4].distinct));
    fprintf(Output_File,"%u,",Info_list[4].total_count);
    fprintf(Output_File,"%u,",Info_list[4].distinct);
    fprintf(Output_File,"\n");    
}

void output_entropy_csv_total()
{
    fprintf(Output_File,"%u,",Current_time);
    fprintf(Output_File,"%lf,",Info_list[0].entropy/log(Info_list[0].total_count));
    fprintf(Output_File,"%u,",Info_list[0].total_count);
    fprintf(Output_File,"%u,",Info_list[0].distinct);

    fprintf(Output_File,"%lf,",Info_list[1].entropy/log(Info_list[1].total_count));
    fprintf(Output_File,"%u,",Info_list[1].total_count);
    fprintf(Output_File,"%u,",Info_list[1].distinct);

    fprintf(Output_File,"%lf,",Info_list[2].entropy/log(Info_list[2].total_count));
    fprintf(Output_File,"%u,",Info_list[2].total_count);
    fprintf(Output_File,"%u,",Info_list[2].distinct);

    fprintf(Output_File,"%lf,",Info_list[3].entropy/log(Info_list[3].total_count));
    fprintf(Output_File,"%u,",Info_list[3].total_count);
    fprintf(Output_File,"%u,",Info_list[3].distinct);

    fprintf(Output_File,"%lf,",Info_list[4].entropy/log(Info_list[4].total_count));
    fprintf(Output_File,"%u,",Info_list[4].total_count);
    fprintf(Output_File,"%u,",Info_list[4].distinct);
    fprintf(Output_File,"\n");    

}


void Close_Output_File()
{
    file_cnt=0;
    while(filelist[file_cnt])
    {
        Output_File=filelist[file_cnt];
        fclose(Output_File);
        file_cnt++;
    }
}