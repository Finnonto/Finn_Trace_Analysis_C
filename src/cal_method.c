#include "cal_method.h"
#include "trace_analysis.h"


double RandInRnage()
{   
    
    rand_tmp = rand();
    
    if(rand_tmp<=0)
    {
        rand_tmp = 0 ;
        rand_tmp += 1;
    }
    else if (rand_tmp >= RAND_MAX)
    {
        rand_tmp = RAND_MAX;
        rand_tmp -= 1;
    }
    
    return (double)rand_tmp /RAND_MAX;
}


static void hash_affine_20para(uint32_t in_data,uint16_t table_size,uint32_t *hash_result)
{
    uint32_t para_a[20]={0x177510d1, 0xda1e0f42, 0x964fbf1e, 0x269df1e6, 0x916cc092,
                    0x7931bd51, 0x12a504fc, 0x76100f01, 0xc246978c, 0x87f2cc91, 
                    0x97910a77, 0x49a930b6, 0x3c48cc20, 0xbbbbff4e, 0xca2d6493, 
                    0xfabac315, 0xefbb19dd, 0x954361d, 0x5bff105e, 0x1ad1e815};
    uint32_t para_c[20]={0x930874a7, 0x7df6e6b4, 0xc3e1046a, 0x289efd5b, 0xbe9694c1, 
                    0x2d85c18e, 0x3bfbe5bc, 0xb0e2d294, 0x5031b9cf, 0xe554adbb, 
                    0x7a6ea2de, 0x829a2955, 0x6b80de1d, 0x6cf04e75, 0x277f56b1, 
                    0x301c379c, 0xf65eeac1, 0xd7ab0949, 0xef0ded41, 0xf2cba001};
    uint32_t mod_m = pow(2,31)-1;
    
    memset(hash_result, 0, sizeof(int) * K_Value);
    for(int i= 0;i<K_Value;i++)
    {
        hash_result[i] = ( (para_a[i]*in_data + para_c[i]) % mod_m ) % table_size;
    }
}

static void hash_affine_20_2para(uint32_t in_data,uint16_t table_size,uint32_t *hash_result_a,uint32_t *hash_result_b)
{
    uint32_t para_a[40]={0x177510d1, 0xda1e0f42, 0x964fbf1e, 0x269df1e6, 0x916cc092,
                        0x7931bd51, 0x12a504fc, 0x76100f01, 0xc246978c, 0x87f2cc91, 
                        0x97910a77, 0x49a930b6, 0x3c48cc20, 0xbbbbff4e, 0xca2d6493, 
                        0xfabac315, 0xefbb19dd, 0x954361d, 0x5bff105e, 0x1ad1e815,
                        0xa4d41053, 0xa5507e69, 0x9f571b50, 0x5f03492f, 0x35b3e590, 
                        0x426796b6, 0x1a462f78, 0x5f7404e2, 0xcb5e5215, 0x8f081cb7, 
                        0x95ab0a84, 0x7d2fd9ba, 0xd7d748db, 0x2c3137eb, 0x8c0c1e71, 
                        0x7eeab7d9, 0x4297c61f, 0xbaeec404, 0x55e56436, 0xe281d41a};
    uint32_t para_c[40]={0x930874a7, 0x7df6e6b4, 0xc3e1046a, 0x289efd5b, 0xbe9694c1, 
                        0x2d85c18e, 0x3bfbe5bc, 0xb0e2d294, 0x5031b9cf, 0xe554adbb, 
                        0x7a6ea2de, 0x829a2955, 0x6b80de1d, 0x6cf04e75, 0x277f56b1, 
                        0x301c379c, 0xf65eeac1, 0xd7ab0949, 0xef0ded41, 0xf2cba001,
                        0xe50f46da, 0xae65910f, 0xb2ec9527, 0x5241b84f, 0x6eb745dc, 
                        0x8b14934a, 0xe3087d58, 0xac7035bc, 0xac6c7471, 0xd0a517bc, 
                        0x71d65fe2, 0xbab80ff2, 0x224de591, 0xe433876b, 0x7485409b, 
                        0x7f067133, 0xa97e19bf, 0x9f3a5ff9, 0x5a545530, 0x6a115f65};
    uint32_t mod_m = pow(2,31)-1;
    
    memset(hash_result_a, 0, sizeof(uint32_t) * K_Value);
    memset(hash_result_b, 0, sizeof(uint32_t) * K_Value);
    for(int i= 0;i<K_Value;i++)
    {
        hash_result_a[i] = ( (para_a[i]*in_data + para_c[i]) % mod_m ) % table_size;
        hash_result_b[i] = ( (para_a[i+20]*in_data + para_c[i+20]) % mod_m ) % table_size;
    }
}

void import_inverse_cdf_table(uint16_t table_amount)
{
    

    char table_size_char[10];
    char table_path[60] = {"tables/inverse_table/inverse_table_"};
    uint32_t entry_cnt = 0;
    sprintf(table_size_char,"%d",Table_Size);
    strcat(table_path,table_size_char);
    strcat(table_path,"/table");
    for(int i=0;i<table_amount;++i)
    {
        entry_cnt = 0;
        FILE *tb;
        
        char table_path_tmp[60];
        strcpy(table_path_tmp,table_path);
        char num[4];
        sprintf(num,"%d",i);
        strcat(table_path_tmp,num);
        strcat(table_path_tmp,".txt");
                
        if(!(tb = fopen(table_path_tmp,"r")))
        {
            printf("load inverse table file fail\n");
            exit(0);
        }
        
        
        while(fscanf(tb,"%lf",&Inverse_table[i].Table[entry_cnt]) != EOF)
        {
            entry_cnt ++;
        }
        Table_Entry_list[i] = entry_cnt;
        fclose(tb);
    }
    
}

void import_inverse_cdf_stage_table(uint16_t table_amount)
{
    
    char table_path[100] = {"tables/inverse_table/inverse_table_16384_50/table_50_"};
    char TXT[5]={".txt"};

    
    for(int i=0;i<table_amount;++i)
    {
        IC_Entry = 1;
        FILE *tb;
        char table_path_tmp[100];
        strcpy(table_path_tmp,table_path);
        char num[4];
        sprintf(num,"%d",i);

        strcat(table_path_tmp,num);
        strcat(table_path_tmp,TXT);
                
        if(!(tb = fopen(table_path_tmp,"r")))
        {
            printf("load inverse table file fail\n");
            exit(0);
        }
        
        
        while(fscanf(tb,"%lf %d",&Inverse_Stage_table[i].Table[IC_Entry],&Inverse_Stage_point[i].point[IC_Entry]) != EOF)
        {
            IC_Entry ++;
        }
        Table_Entry_list[i] = IC_Entry;
        fclose(tb);
        printf("%u %u\n",Inverse_Stage_point[i].point[IC_Entry-1],IC_Entry-1);
    }
}

void import_inverse_cdf_single_table(uint32_t index)
{
    char table_path[100] = {"tables/inverse_table/inverse_table_16384_100/table_100_"};
    char TXT[5]={".txt"};

    
    Table_Amount = 1;
    IC_Entry = 1;
    FILE *tb;
    char table_path_tmp[100];
    strcpy(table_path_tmp,table_path);
    char num[4];
    sprintf(num,"%d",index);

    strcat(table_path_tmp,num);
    strcat(table_path_tmp,TXT);
            
    if(!(tb = fopen(table_path_tmp,"r")))
    {
        printf("load inverse table file fail\n");
        exit(0);
    }
    
    
    while(fscanf(tb,"%lf %d",&Inverse_Stage_table[0].Table[IC_Entry],&Inverse_Stage_point[0].point[IC_Entry]) != EOF)
    {
        IC_Entry ++;
    }
    Table_Entry_list[0] = IC_Entry;
    fclose(tb);
    printf("%u %u\n",Inverse_Stage_point[0].point[IC_Entry-1],IC_Entry-1);


}

void import_HeadTail_table()
{
    char table_path[60] = {"tables/ht_table/HT_Table_50.txt"};
    
    HT_Table_Entry  = 1;
    FILE *tb;
    
    if(!(tb = fopen(table_path,"r")))
    {
        printf("load inverse table file fail\n");
        exit(0);
    }
    while(fscanf(tb,"%lf %d",&Head_Value[HT_Table_Entry],&Stage_Point[HT_Table_Entry]) != EOF)
    {
        HT_Table_Entry ++;
    }
    
    fclose(tb);
    printf("%u %u\n",Stage_Point[HT_Table_Entry-1],HT_Table_Entry-1);

}


trace_info_t *exact(tree_t *item)
{

    //return info init
    trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));
    info->entropy = 0;
    info->total_count = 0;
    info->distinct = 0;
    //cal value init
    uint32_t total_item_cnt = 0;
    double entropy = 0;
    uint32_t distinct = 0;

    //init node
    node_t *current_node;
    current_node = item->root;
    
    while(current_node)
    {
        entropy += (current_node->cnt*log(current_node->cnt));                                                                    
        total_item_cnt += current_node->cnt;                                                                
        distinct++;
        current_node = current_node->right;
    }
    
    if (total_item_cnt == 0 || total_item_cnt == 1)return NULL;
										
    entropy -= (total_item_cnt * log(total_item_cnt));
									
    entropy = -entropy/total_item_cnt;

    
    // set info member
    info->entropy = entropy;
    info->total_count = total_item_cnt;
    info->distinct = distinct;

    return info;								
    
}

trace_info_t *Clifford_est(tree_t *item)
{   
    //return info init
    trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));
    info->entropy = 0;
    info->total_count = 0;
    info->distinct = 0;
    //cal value init
    uint32_t total_item_cnt = 0;
    double entropy = 0;
    uint32_t distinct = 0;

    //init node
    node_t *current_node;
    current_node = item->root;
    //Clifford param
    double k_register[K_Value];
    double u1=0;
    double u2=0;
    double w1=0;
    double w2=0;
    double ran1=0;
    double ran2=0;
    double ran = 0;
    


    memset(k_register, 0, sizeof(double) * K_Value);


    while(current_node)
    {
        total_item_cnt += current_node->cnt;
        distinct++;
        srand(current_node->data + Deviation);

        for(uint32_t i=0;i<K_Value;i++)
        {
            

            u1 = RandInRnage();

            u2 = RandInRnage();

            // Alpha stable 
            w1 = PI*(u1-0.5);
            w2 = -log(u2);
            
            ran1 = tan(w1) * (PI/2 - w1);
            ran2 = log(w2 * cos(w1) / (PI/2-w1) );
            ran = ran1 + ran2;

            // store k value

            k_register[i] += ran * current_node->cnt;

        }
        
	    current_node = current_node->right;

    }

    if (total_item_cnt == 0 || total_item_cnt == 1)return NULL;
    else{

        for(uint32_t i=0;i<K_Value;i++){

            k_register[i] /= total_item_cnt;
            entropy += exp(k_register[i]);
            
        }

        
        entropy /= K_Value;
        
        

        entropy = -log(entropy);
    }

    // set info member
    info->entropy = entropy;
    info->total_count = total_item_cnt;
    info->distinct = distinct;
    return info;

}

trace_info_t *Clifford_cdf_est(tree_t *item){
    //return info init
    trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));
    info->entropy = 0;
    info->total_count = 0;
    info->distinct = 0;
    //cal value init
    Table_Size = 16384;
    Table_Amount = 10;
    K_Value = 20;
    
    double k_register[K_Value];    
    double entropy = 0;
    double entropy_sum = 0;
    double entropy_list[K_Value];
    uint32_t hash_result[K_Value];
    uint32_t total_item_cnt = 0;
    uint32_t distinct = 0;
    node_t *current_node;
    
    
    
    for(int tn = 0 ;tn< Table_Amount; tn++)
    {
        
        total_item_cnt = 0;
        distinct = 0;
        current_node = item->root;

        memset(k_register, 0, sizeof(double) * K_Value);
        memset(hash_result, 0, sizeof(uint32_t) * K_Value);
        // create inverse cdf table
        
        // calculate elements within the time interval
        while(current_node)
        {
            // total cnt
            total_item_cnt += current_node->cnt;
            distinct++;
            // store k value
            hash_affine_20para(current_node->data , Table_Size , hash_result);
            
            for(int i=0; i<K_Value; i++)
            {
                k_register[i] += Inverse_table[tn].Table[hash_result[i]] * current_node->cnt;	
            }
            current_node = current_node->right;
        }

        if (total_item_cnt == 0 || total_item_cnt == 1)return NULL;
        
        for(int i=0;i<K_Value;i++)
        {
            k_register[i] /= total_item_cnt;
            entropy += exp(k_register[i]);
        }

        entropy /= K_Value;
        entropy_list[tn] = -log(entropy);
    }
    // get the mean of entropy list 




    for(int i =0 ;i<Table_Amount;i++)
    {
        
        entropy_sum += entropy_list[i];
    }
    entropy = entropy_sum/Table_Amount;
    info->entropy = entropy;
    info->total_count = total_item_cnt;
    info->distinct = distinct;

    return info;	



}

trace_info_t *Clifford_cdf_stage_est(tree_t *item)
{
    //return info init
    trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));
    info->entropy = 0;
    info->total_count = 0;
    info->distinct = 0;
    //cal value init
    Table_Size = 16384;

    K_Value = 20;
    
    double k_register[K_Value];    
    double entropy = 0;
    double entropy_sum = 0;
    double entropy_list[K_Value];
    uint32_t hash_result[K_Value];
    uint32_t total_item_cnt = 0;
    uint32_t distinct = 0;
    node_t *current_node;
    
    
    
    for(int tn = 0 ;tn< Table_Amount; tn++)
    {
        
        total_item_cnt = 0;
        distinct = 0;
        current_node = item->root;

        memset(k_register, 0, sizeof(double) * K_Value);
        memset(hash_result, 0, sizeof(uint32_t) * K_Value);
        // create inverse cdf table
        
        // calculate elements within the time interval
        while(current_node)
        {
            // total cnt
            total_item_cnt += current_node->cnt;
            distinct++;
            // store k value
            hash_affine_20para(current_node->data , Table_Size , hash_result);
            
            for(int i=0; i<K_Value; i++)
            {
                if(hash_result[i]>=Table_Size)
                {
                hash_result[i]-=1;
                }
                else if(hash_result[i]<=0)
                {
                hash_result[i]+=1;
                }
                

                if(hash_result[i]<=274)k_register[i] += Inverse_Stage_table[tn].Table[hash_result[i]] * current_node->cnt;	
                else
                {
                    for(int sp =275;sp<Table_Entry_list[tn];sp++)
                    {
                        if(hash_result[i]< Inverse_Stage_point[tn].point[sp])
                        {
                            k_register[i] += Inverse_Stage_table[tn].Table[sp] * current_node->cnt;	
                            break;
                        }
                        if(sp==(Table_Entry_list[tn]-1) && hash_result[i] > Inverse_Stage_point[tn].point[Table_Entry_list[tn]-1])
                        {
                            k_register[i] += Inverse_Stage_table[tn].Table[sp] * current_node->cnt;	
                        }
                    }
                
                }

            }
            current_node = current_node->right;
        }

        if (total_item_cnt == 0 || total_item_cnt == 1)return NULL;
        
        for(int i=0;i<K_Value;i++)
        {
            k_register[i] /= total_item_cnt;
            entropy += exp(k_register[i]);
        }

        entropy /= K_Value;
        entropy_list[tn] = -log(entropy);
    }
    // get the mean of entropy list 




    for(int i =0 ;i<Table_Amount;i++)
    {
        
        entropy_sum += entropy_list[i];
    }
    entropy = entropy_sum/Table_Amount;
    info->entropy = entropy;
    info->total_count = total_item_cnt;
    info->distinct = distinct;

    return info;
}

trace_info_t *Clifford_HT_est(tree_t *item)
{
    uint32_t Tail_Threshold[13] = {104, 2154, 7507, 12230, 14702, 15742, 16144, 16295, 16351, 16372, 16380, 16383, 16384};
    
    trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));
    info->entropy = 0;
    info->total_count = 0;
    info->distinct = 0;
    //cal value init
    Table_Size = 16384;
    K_Value = 20;
    
    double k_register[K_Value];    
    double entropy = 0;
    
    uint32_t hash_result_a[K_Value];
    uint32_t hash_result_b[K_Value];

    uint32_t total_item_cnt = 0;
    uint32_t distinct = 0;
    
    //alpha stable 
    double u1=0;
    double u2=0;
    double w1=0;
    double w2=0;
    double ran1=0;
    double ran2=0;
    long long ran =0;
    // head & tail
    
    memset(k_register, 0, sizeof(double) * K_Value);
    memset(hash_result_a, 0, sizeof(uint32_t) * K_Value);
    memset(hash_result_b, 0, sizeof(uint32_t) * K_Value);

    node_t* current_node;

    current_node  = item->root;
    
    while (current_node)
    {
        total_item_cnt+=current_node->cnt;
        distinct++;
        hash_affine_20_2para(current_node->data,Table_Size,hash_result_a,hash_result_b);
        for(int i = 0;i<K_Value;i++)
        {
            if(hash_result_a[i]>=Table_Size)
            {
                u1 = (double)hash_result_a[i]-1/Table_Size;
            }
            else if(hash_result_a[i]<=0)
            {
                u1 = (double)hash_result_a[i]+1/Table_Size;        
            }
            else 
            {
                u1 = (double)hash_result_a[i]/Table_Size;        
            }
            u2 = (double)1/Table_Size;
            //printf("u1=%lf,u2=%lf\n",u1,u2);
            w1 = PI*(u1-0.5);
            w2 = -log(u2);
           
            ran1 = tan(w1) * (PI/2 - w1);
            ran2 = log(w2 * cos(w1) / (PI/2-w1) );
            //printf("ran1=%lf,ran2=%lf\n",ran1,ran2);
            ran = (long long)ran1 + ran2;

            //printf("ran =%lld \n",ran);
            
            
            int table_end_correction = 0;
            uint32_t table_end_key;
            
            
            if( hash_result_b[i]==Table_Size)
            {
                table_end_key = hash_result_b[i]-1;
            }
            else
            {
                table_end_key = hash_result_b[i];
            }
            

            if (table_end_key   <= Tail_Threshold[0])table_end_correction = 0;
            else if (table_end_key <= Tail_Threshold[1])table_end_correction = -1;
            else if (table_end_key <= Tail_Threshold[2])table_end_correction = -2;
            else if (table_end_key <= Tail_Threshold[3])table_end_correction = -3;
            else if (table_end_key <= Tail_Threshold[4])table_end_correction = -4;
            else if (table_end_key <= Tail_Threshold[5])table_end_correction = -5;
            else if (table_end_key <= Tail_Threshold[6])table_end_correction = -6;
            else if (table_end_key <= Tail_Threshold[7])table_end_correction = -7;
            else if (table_end_key <= Tail_Threshold[8])table_end_correction = -8;
            else if (table_end_key <= Tail_Threshold[9])table_end_correction = -9;
            else if (table_end_key <= Tail_Threshold[10])table_end_correction = -10;
            else if (table_end_key <= Tail_Threshold[11])table_end_correction = -11;
            else if (table_end_key <= Tail_Threshold[12])table_end_correction = -12;
        
            k_register[i] += (double)(ran+table_end_correction)*current_node->cnt; 
            //printf("k_i=%lf\n",k_register[i]);


        }
        current_node = current_node->right;

    }

    if (total_item_cnt == 0 || total_item_cnt == 1)return NULL;
    else{
        for(uint32_t i=0;i<K_Value;i++){
            k_register[i] /= total_item_cnt;
            
            entropy += exp(k_register[i]);
            
        }
        
        entropy /= K_Value;
        
        
        entropy = -log(entropy);
    }

    // set info member
    info->entropy = entropy;
    info->total_count = total_item_cnt;
    info->distinct = distinct;

    return info;

    }

trace_info_t *Clifford_HTo_est(tree_t *item)
{
    uint32_t Tail_Threshold[13] = {104, 2154, 7507, 12230, 14702, 15742, 16144, 16295, 16351, 16372, 16380, 16383, 16384};
    
    trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));
    info->entropy = 0;
    info->total_count = 0;
    info->distinct = 0;
    //cal value init
    Table_Size = 16384;
    K_Value = 20;
    
    double k_register[K_Value];    
    double entropy = 0;
    
    uint32_t hash_result_a[K_Value];
    uint32_t hash_result_b[K_Value];

    uint32_t total_item_cnt = 0;
    uint32_t distinct = 0;
    
    //alpha stable 
   
    double ran =0;
    // head & tail
    
    memset(k_register, 0, sizeof(double) * K_Value);
    memset(hash_result_a, 0, sizeof(uint32_t) * K_Value);
    memset(hash_result_b, 0, sizeof(uint32_t) * K_Value);

    node_t* current_node;

    current_node  = item->root;
    
    while (current_node)
    {
        total_item_cnt+=current_node->cnt;
        distinct++;
        hash_affine_20_2para(current_node->data,Table_Size,hash_result_a,hash_result_b);
        for(int i = 0;i<K_Value;i++)
        {
            if(hash_result_a[i]>=Table_Size)
            {
                hash_result_a[i]-=1;
            }
            else if(hash_result_a[i]<=0)
            {
                hash_result_a[i]+=1;
            }

            
            if(hash_result_a[i]<=274)ran = Head_Value[hash_result_a[i]];
            else
            {
                for(int sp =275;sp<HT_Table_Entry;sp++)
                {
                    if(hash_result_a[i]<Stage_Point[sp])
                    {
                        ran = Head_Value[sp];
                        break;
                    }
                    if(sp==(HT_Table_Entry-1) && hash_result_a[i]>Stage_Point[HT_Table_Entry-1])
                    {
                        ran = Head_Value[sp];
                    }
                }
            
            }
            
            
            
            int table_end_correction = 0;
            uint32_t table_end_key;
            
            
            if( hash_result_b[i]==Table_Size)
            {
                table_end_key = hash_result_b[i]-1;
            }
            else
            {
                table_end_key = hash_result_b[i];
            }
            

            if (table_end_key   <= Tail_Threshold[0])table_end_correction = 0;
            else if (table_end_key <= Tail_Threshold[1])table_end_correction = -1;
            else if (table_end_key <= Tail_Threshold[2])table_end_correction = -2;
            else if (table_end_key <= Tail_Threshold[3])table_end_correction = -3;
            else if (table_end_key <= Tail_Threshold[4])table_end_correction = -4;
            else if (table_end_key <= Tail_Threshold[5])table_end_correction = -5;
            else if (table_end_key <= Tail_Threshold[6])table_end_correction = -6;
            else if (table_end_key <= Tail_Threshold[7])table_end_correction = -7;
            else if (table_end_key <= Tail_Threshold[8])table_end_correction = -8;
            else if (table_end_key <= Tail_Threshold[9])table_end_correction = -9;
            else if (table_end_key <= Tail_Threshold[10])table_end_correction = -10;
            else if (table_end_key <= Tail_Threshold[11])table_end_correction = -11;
            else if (table_end_key <= Tail_Threshold[12])table_end_correction = -12;
        
            k_register[i] += (double)(ran+table_end_correction)*current_node->cnt; 
            //printf("k_i=%lf\n",k_register[i]);


        }
        current_node = current_node->right;

    }

    if (total_item_cnt == 0 || total_item_cnt == 1)return NULL;
    else{
        for(uint32_t i=0;i<K_Value;i++){
            k_register[i] /= total_item_cnt;
            
            entropy += exp(k_register[i]);
            
        }
        
        entropy /= K_Value;
        
        
        entropy = -log(entropy);
    }

    // set info member
    info->entropy = entropy;
    info->total_count = total_item_cnt;
    info->distinct = distinct;

    return info;

    }




