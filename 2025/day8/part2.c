#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE
#define NDEBUG

#include "sc_heap.h"
#include "sc_map.h"
#include "union_find.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

typedef struct {
    sll x;
    sll y;
    sll z;

    // For tracking connections
    struct ufnode n;
} box;

#define NLARGEST  3
#define MAX (1001)
#define NUM_CONNS 1000

int nboxes = 0;
box boxes[MAX];

typedef struct {
    box* a;
    box* b;
} boxpair;

// We store each boxpair here so that the priority-queue can reference them
boxpair allpairs[MAX * MAX];

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    sll x = atoll( line );

    line = strchr( line, ',' );
    line++;
    sll y = atoll( line );

    line = strchr( line, ',' );
    line++;
    sll z = atoll( line );

    boxes[ nboxes ] = (box) { x, y, z};

    // Data pointer of union-find node points to self
    uf_init( &boxes[nboxes].n, &boxes[nboxes] );
    nboxes ++;
}

ull distance( box* a, box* b )
{
    assert( a );
    assert( b );

    ull dist  = (b->x-a->x) * (b->x-a->x);
        dist += (b->y-a->y) * (b->y-a->y);
        dist += (b->z-a->z) * (b->z-a->z);

    return dist;
}

void printBox( box* b ){
    printf( "[%3lld,%3lld,%3lld]", b->x, b->y, b->z );

}

void connect( box* a, box* b )
{
    // Pretty print
    //DEBUG( "Connecting boxes " );
    //printBox( a );
    //DEBUG( ", " );
    //printBox( b );
    //DEBUG( "\n" );

    uf_merge( &a->n, &b->n );
}

int AreAllConnected()
{
    struct ufnode* parent = uf_parent( &boxes[0].n );

    return parent->size == nboxes;
}

void FindLargest()
{
    // This map will contain a small subset so it can begin small
    struct sc_map_intv circmap;
    sc_map_init_intv( &circmap, MAX/2, 0 );

    // First create set of UNIQUE circuits
    for( int circ = 0; circ < nboxes; circ++ ){
        box* b = &boxes[circ];
        struct ufnode* node = uf_parent(  &b->n );
        int size = node->size;
        VERBOSE( "Circuit size of box %3d: %d\n", circ, size );

        sc_map_put_intv( &circmap, size, node );
    }
    VERBOSE("\n");

    // Then, sort the largest circuits by size
    struct sc_heap sizeheap;
    sc_heap_init( &sizeheap, 0 );

    int key;
    struct ufnode* node;
    sc_map_foreach( &circmap, key, node ){
        int size = node->size;
        VERBOSE( "Unique circuit of size %3d\n", size );

        sc_heap_add( &sizeheap, size, node );
    }

    // The heap is a MIN heap, but we want the LARGEST elements
    // So pop it until it has only the number of elems we want
    while( sc_heap_size( &sizeheap ) > NLARGEST ){
        (void) sc_heap_pop( &sizeheap );
    }

    // Finally, pop the smallest N circuits and report their sizes
    ull total = 1;
    while( sc_heap_size( &sizeheap ) != 0 ){
        int size = ((struct ufnode*)sc_heap_pop(&sizeheap)->data)->size;
        INFO( "Next largest size: %3d\n", size );

        total *= size;
    }
    INFO( "Total size:\n%lld\n", total );

}

void SOLVER_PrintSolution( void )
{
    struct sc_heap distanceHeap;

    sc_heap_init( &distanceHeap, 0);

    int npairs = 0;
    for( int i = 0; i < nboxes; i++ ){
        for( int j = i+1; j < nboxes; j++ ){
            allpairs[ npairs ] = (boxpair) { &boxes[i], &boxes[j] };

            sll d = distance( allpairs[npairs].a, allpairs[npairs].b);
            sc_heap_add( &distanceHeap, d, &allpairs[npairs] );

            npairs++;
        }
    }

	struct sc_heap_data *elem;
    int i = 0;
    box* first;
	while ((elem = sc_heap_pop(&distanceHeap)) != NULL) {
        box* a = ((boxpair*)elem->data)->a;
        box* b = ((boxpair*)elem->data)->b;

        connect( a, b );

        if( AreAllConnected() ){
            INFO ( "All connected after %d conns\n", i );

            int xf = a->x;
            int xl = b->x;
            int dist = xf * xl;
            INFO( "%d * %d gives:\n%d\n", xf, xl, dist );
            break;
        }

        // Keep connecting until ALL of them are connected together
        i++;
        if( i == NUM_CONNS*NUM_CONNS ){
            INFO("What? Failed to connect all circuit boxes\n");
            break;
        }
    }

    //INFO( "Connected %d boxes\n", NUM_CONNS );

    //FindLargest();
}


