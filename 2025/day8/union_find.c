#include "union_find.h"

void
uf_init( struct ufnode* node, const void* data )
{
    node->parent = node;
    node->rank   = 0;
    node->size   = 1;
    node->data   = data;
}

struct ufnode*
uf_parent( struct ufnode* node )
{
    struct ufnode* root = node;

    while( root != root->parent )
        root = root->parent;

    // Optimization for find:  Update EACH parent to point to root
    while( node != node->parent ){
        struct ufnode* p = node->parent;
        node->parent = root;
        node = p;
    }

    return root;
}

void
uf_merge( struct ufnode* a, struct ufnode* b )
{
    struct ufnode* pa = uf_parent( a );
    struct ufnode* pb = uf_parent( b );

    if( pa == pb )
        return;

    if( pa->rank < pb->rank ){
        struct ufnode* temp = pa;
        pa = pb;
        pb = temp;
    }
    pb->parent = pa;

    if( pa->rank == pb->rank ){
        pa->rank ++;
    }

    pa->size += pb->size;
}
