#ifndef GT_UNION_FIND
#define GT_UNION_FIND

#include <stddef.h>

struct ufnode {
    struct ufnode* parent;
    size_t         rank;
    size_t         size;

    const void*    data;
};

void
uf_init( struct ufnode* node, const void* data );

struct ufnode*
uf_parent( struct ufnode* node );

void
uf_merge( struct ufnode* a, struct ufnode* b );

#endif
