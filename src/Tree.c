#include "Tree.h"
#include <stdio.h>
#include <inttypes.h>


static void _tree_delete(node_t *node);
static size_t _tree_height(const node_t *node);
static uint32_t _tree_insert(node_t **node,uint32_t value);
static bool _tree_return_all(const node_t * node);
static bool _tree_flatten_check(node_t *node);
static void tree_LL(node_t **node);
static void tree_RR(node_t **node);
static void tree_LR(node_t **node);
static void tree_RL(node_t **node);



static void tree_LL(node_t **node)
{
    

    if((*node)->left->right && (*node)->right)
    {
        (*node)->bf =0;
    }
    else if(!((*node)->left->right) && (*node)->right)
    {
        (*node)->bf = -1;
    }
    else if(!(*node)->right)
    {
        (*node)->bf =0;
    }

    (*node)->height-=2;
    
    node_t *node_tmp = (*node)->left;
    (*node)->left = node_tmp->right;
    node_tmp->right =(*node);
    node_tmp->bf =0;
    (*node)= node_tmp;
}

static void tree_RR(node_t **node)
{
    if((*node)->right->left && (*node)->left)
    {
        (*node)->bf =0;
    }
    else if(!((*node)->right->left) && (*node)->left)
    {
        (*node)->bf =1;
    }
    else if(!(*node)->left)
    {
        (*node)->bf =0;
    }


    (*node)->height-=2;
    node_t *node_tmp = (*node)->right;
    (*node)->right = node_tmp->left;
    node_tmp->left =(*node);
    node_tmp->bf =0;
    (*node)= node_tmp;
    
}

static void tree_LR(node_t **node)
{
        
    tree_RR(&((*node)->left));
    (*node)->left->left->height+=1;
    (*node)->left->height+=1;
    tree_LL(node);
}

static void tree_RL(node_t **node)
{
    
    tree_LL(&((*node)->right));
    (*node)->right->right->height+=1;
    (*node)->right->height+=1;
    tree_RR(node);
}






node_t *node_create(uint32_t value)
{
    node_t *nd = (node_t*)malloc(sizeof(node_t));

    if(!nd)return nd;
    nd->data = value;
    nd->cnt = 1;
    nd->height = 1;
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

static uint32_t _tree_insert(node_t **node,uint32_t value)
{
    size_t lh = 0;
    size_t rh = 0;
    
    if(!(*node))
    {   
        
        *node = node_create(value);
        if(!(*node))return true;
        else return 1;
    }

    if((*node)->data == value)
    {
        
        (*node)->cnt += 1;
        return  0;
    }
    else
    {
        
        if((*node)->data < value)
        {
            
            rh = _tree_insert(&((*node)->right),value);
            lh = (*node)->left?(*node)->left->height:0;
            if(rh>0)
            {
                (*node)->height = rh>lh?rh+1:lh+1;  
            }
            else
            {
                rh = (*node)->right->height;
            }
            
        }    
        if((*node)->data > value)
        {
            
            lh = _tree_insert(&((*node)->left),value);
            rh = (*node)->right?(*node)->right->height:0; 
            if(lh>0)
            {
                (*node)->height = lh>rh?lh+1:rh+1;     
            }
            else
            {
                lh = (*node)->left->height;
            }
            
        }
        
        
        (*node)->bf = lh -rh;
        
        
        if((*node)->bf == 2) 
        {
            if ((*node)->left->bf==1)
            {
                tree_LL(node);
            }
            else tree_LR( node );
        }
        if((*node)->bf == -2)
        {
           
            if ((*node)->right->bf == 1)
            {
                tree_RL(node);
            }
            else  tree_RR(node);
        }
        
        
        return (*node)->height;
    }
    
    
}

uint32_t tree_insert(tree_t *self,uint32_t value)
{
    assert(self);

    return _tree_insert(&(self->root),value);
}

static bool _tree_return_all(const node_t * node)
{
    if(!node)return true;
    
    printf("data = %u , height = %zu ,bf =%zu \n",node->data,node->height,node->bf);
    

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
    if(!(node->left) && !(node->right))
    {
        return false;
    }
    printf("node = %"PRIu32" \n",node->data);
    if(node->left)
    {
        printf("node->left = %"PRIu32"\n",node->left->data);
        
        printf("check1\n");
        node_t *node_tmp;
        if(node->left->right)
        {   
            printf("node->left->right = %"PRIu32"\n",node->left->right->data);
            printf("check2-1\n");
            node_tmp = right_side(node->left);
            printf("node_tmp = %"PRIu32"\n",node_tmp->data);
        }
        else 
        {
            printf("check2-2\n");
            node_tmp = node->left;
            printf("node_tmp = %"PRIu32"\n",node_tmp->data);
        }
        if(node->right)
        {
            printf("node->right = %"PRIu32"\n",node->right->data);
            printf("check3\n");
            node_tmp->right = node->right;    
        }
        printf("check4\n");
        node->right = node->left;
        node->left = NULL;
        
    }
    printf("check5\n");
    if(node->right)
    {
        printf("node->right = %"PRIu32"\n",node->right->data);
        if(node->right->right)printf("node->right->right = %"PRIu32"\n",node->right->right->data);
        printf("check6\n");
        _tree_to_list(node->right);
    }
    printf("check7\n");
    return false;
}

bool tree_to_list(tree_t *self)
{
    assert(self);
    return _tree_to_list(self->root);

}

static bool _tree_flatten_check(node_t *node)
{   
    
    if(node->left)
    {
        printf("left(%u)->",node->left->data);
        _tree_flatten_check(node->left);
    }
    if(node->right)
    {
        printf("right(%u)\n",node->right->data);
        _tree_flatten_check(node->right);
    }
    if(!node->left && !node->right)printf("none\n");
    return false;
}

bool tree_flatten_check(tree_t *self)
{

    assert(self);
    
    printf("root(%u)\n",self->root->data);
    
    return _tree_flatten_check(self->root);

}



