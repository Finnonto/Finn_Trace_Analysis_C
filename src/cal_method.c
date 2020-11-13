#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "Tree.h"
#include "cal_method.h"



trace_info_t *exact(tree_t *item,uint32_t k_value){
    
    K_Value = k_value;
    //return info init
    trace_info_t *info = (trace_info_t*)malloc(sizeof(trace_info_t));
    info->entropy = 0;
    info->total_count = 0;
    info->distinct = 0;
    //cal value init
    uint32_t total_item_cnt = 0;
    float entropy = 0;
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


