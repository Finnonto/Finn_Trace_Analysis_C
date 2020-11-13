#include<stdint.h>
#include<stdbool.h>
#include<stddef.h>
#include<stdlib.h>
#include<assert.h>
#ifndef _TREE_H
#define _TREE_H

#define NODE_SWAP(a,b){node_t *t;t=a;a=b;b=t;}



typedef struct node
{
    uint32_t data;
    uint32_t cnt;
    size_t bf;
    struct node *left;
    struct node *right;
}node_t;

typedef struct 
{
    node_t *root;
    
}tree_t;

node_t *node_create(uint32_t value);

tree_t  *tree_create();


void tree_delete(void *self);



bool tree_is_empty(void* self);

size_t tree_height(tree_t *self);



node_t* tree_insert(tree_t *self,uint32_t value);



bool tree_return_all(tree_t *self);



















bool tree_to_list(tree_t *self);



bool tree_flatten_check(tree_t *self);


#endif