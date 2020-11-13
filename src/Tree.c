#include "Tree.h"
#include <stdio.h>




static void _tree_delete(node_t *node);
static size_t _tree_height(const node_t *node);
static node_t* _tree_insert(node_t **node,uint32_t value);
static bool _tree_return_all(const node_t * node);
static node_t* tree_LL(node_t *node);
static node_t* tree_RR(node_t *node);
static node_t* tree_LR(node_t *node);
static node_t* tree_RL(node_t *node);
static node_t* right_side(node_t *node);
static bool _tree_to_list(node_t* node);
static bool _tree_flatten_check(node_t *node);



static node_t * tree_LL(node_t *node)
{
    node_t *node_tmp = node->left;
    node->left = node_tmp->right;
    node_tmp->right = node;
    node_tmp->bf = 0;
    return node_tmp;
}

static node_t* tree_RR(node_t *node)
{
    node_t *node_tmp = node->right;
    node->right = node_tmp->left;
    node_tmp->left = node;
    node_tmp->bf = 0;
    return node_tmp;
    
}

static node_t* tree_LR(node_t *node)
{
    node->left = tree_RR(node->left);
    node_t *node_tmp = tree_LL(node);
    node_tmp->bf = 0 ;
    return node_tmp;

}

static node_t* tree_RL(node_t *node)
{
    node->right = tree_LL(node->right);
    
    node_t *node_tmp = tree_RR(node);
    node_tmp->bf = 0 ;
    return node_tmp;
}






node_t *node_create(uint32_t value)
{
    node_t *nd = (node_t*)malloc(sizeof(node_t));

    if(!nd)return nd;
    nd->data = value;
    nd->cnt = 1;
    nd->bf = 0;
    nd->left =NULL;
    nd->right =NULL;
    return nd;
}

tree_t  *tree_create()
{
    tree_t *tr = (tree_t*)malloc(sizeof(tree_t));


    if(!tr)return tr;

    tr->root = NULL;
    
    return tr;
}

static void _tree_delete(node_t *node)
{
    if(!node)return;


    _tree_delete(node->left);
    _tree_delete(node->right);
    
    free(node);
}

void tree_delete(void *self)
{

    if(!self)return;

    node_t* root = ((tree_t*)self)->root;

    if(root)_tree_delete(root);

    free(self);
}

bool tree_is_empty(void* self)
{
    assert(self);

    return ((tree_t*)self)->root ? false : true;
}

static size_t _tree_height(const node_t *node)
{
    if(!node)return 0;

    size_t left = (!(node->left))?0:_tree_height(node->left);
    size_t right = (!(node->right))?0:_tree_height(node->right);

    size_t h = left>right?left:right;

    return h+1;
}

size_t tree_height(tree_t *self)
{
    assert(self);

    return _tree_height(self->root);
}

static node_t * _tree_insert(node_t **node,uint32_t value)
{
    size_t lh = 0;
    size_t rh = 0;
    
    

    if(!(*node))
    {
        *node = node_create(value);
        
        if(!(*node))return NULL;
        else return (*node);
    }

    if((*node)->data == value)
    {
        (*node)->cnt += 1;
        return  (*node);
    }
    else
    {
        if((*node)->data < value)
        {
            (*node)->right =  _tree_insert(&((*node)->right),value);    
        }    
        if((*node)->data > value)
        {
            (*node)->left = _tree_insert(&((*node)->left),value);    
        }
        if(!(*node)->left){lh = 0;}else{lh = _tree_height((*node)->left);}
        if(!(*node)->right){rh = 0;}else{rh = _tree_height((*node)->right);}
        
        (*node)->bf = lh-rh;
        
        if((*node)->bf == 2) 
        {
            if ((*node)->left->bf==1)
            {
                return tree_LL((*node));
            }
            else return tree_LR( (*node) );
        }
        if((*node)->bf == -2)
        {
            if ((*node)->right->bf == 1)
            {
             
                return tree_RL((*node));
            }
            else return tree_RR((*node));
        }
        
        return *node;
    }
    
    
}

node_t* tree_insert(tree_t *self,uint32_t value)
{
    assert(self);

    return _tree_insert(&(self->root),value);
}

static bool _tree_return_all(const node_t * node)
{
    if(!node)return true;
/*
    printf("data = %d , cnt = %d ,bf =%d \n",
    node->data,
    node->cnt,
    node->bf);
*/

    _tree_return_all(node->left);
    
    _tree_return_all(node->right);
    
    return false;
}

bool tree_return_all(tree_t *self)
{
    assert(self);
    return _tree_return_all(self->root);
}

static node_t* right_side(node_t *node)
{
    if(!node->right)
    {   
        return node;
    }
    return right_side(node->right);
}

static bool _tree_to_list(node_t* node)
{
    if(node->left)
    {
        node_t *node_tmp;
        if(node->left->right)
        {
            node_tmp = right_side(node->left);
        }
        else 
        {
            node_tmp = node->left;
        }
        node_tmp->right = node->right;
        node->right = node->left;
        node->left = NULL;
        right_side(node);
    }
    if(!node->left)
    {
        if(node->right)_tree_to_list(node->right);
        return false;
    }
    return true;
    

}

bool tree_to_list(tree_t *self)
{
    if(!self)return true;
    return _tree_to_list(self->root);

}

static bool _tree_flatten_check(node_t *node)
{   
    printf("node(%d)->",node->data);
    if(node->left)
    {
        printf("left(%d)->",node->left->data);
        _tree_flatten_check(node->left);
    }
    if(node->right)
    {
        printf("right(%d)\n",node->right->data);
        _tree_flatten_check(node->right);
    }
    if(!node->left && !node->right)printf("none\n");
    return false;
    
    
}

bool tree_flatten_check(tree_t *self)
{

    assert(self);
    
    printf("root(%d)\n",self->root->data);
    
    return _tree_flatten_check(self->root);

}



