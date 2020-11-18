#include "cal_method.h"
#include "trace_analysis.h"


double RandInRnage()
{   
    
    int rand_tmp = rand();
    if(rand_tmp==0)
    {
        rand_tmp +=1;
    }
    else if (rand_tmp == RAND_MAX)
    {
        rand_tmp -=1;
    }
    

    return (double)rand_tmp /RAND_MAX;
}

trace_info_t *exact(tree_t *item,uint32_t k_value){
    
    K_Value = k_value;
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


trace_info_t *Clifford_est(tree_t *item,uint32_t k_value)
{   
    K_Value = k_value;
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

    

    memset(k_register, 0, sizeof(int) * K_Value);


    while(current_node)
    {
        total_item_cnt += current_node->cnt;
        distinct++;
        srand(current_node->data);

        for(int i=0;i<K_Value;i++)
        {
            

            u1 = RandInRnage();
            
            
            u2 = RandInRnage();

            //avoid boundary
            /*
            if (u1<=0)u1=1.0e-6;
            else if(u1>=1)u1=1.0-1.0e-6;
            
            if (u2<=0)u1=1.0e-006;
            else if(u2>=1)u1=1.0-1.0e-6;
            */

            // Alpha stable 
            w1 = PI*(u1-0.5);
            w2 = -log(u2);
           
            ran1 = tan(w1) * (PI/2 - w1);
            ran2 = log(w2 * cos(w1) / (PI/2-w1) );
            if(Current_time == 510 && (w2 * cos(w1) / (PI/2-w1))< 0 )
            {
                printf("-----%d Second------\n",i); 
                printf("%f,%f\n",u2,u1);
                printf("%f,%f\n",w2,w1);
            }
            ran = ran1 + ran2;
            
            // store k value
            
            k_register[i] += ran * current_node->cnt;
        }
	    current_node = current_node->right;

    }

    if (total_item_cnt == 0 || total_item_cnt == 1)return NULL;
    else{
        for(int i=0;i<K_Value;i++){
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

