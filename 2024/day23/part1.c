#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE

#include "sc_map.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

uint32_t STR_TO_HASH( const char* name )
{
    uint16_t name16 = *((uint16_t*) name);
    return (uint32_t) name16;
}

struct node {
    // 2-char name is super quick to hash if treated as 32-bit number
    union {
        char str[4];
        ui   hash;
    } name;

    struct sc_map_32v neigh;
};
struct sc_map_32v allNodes;

struct node* MakeNode( const char* name )
{
    assert( name );

    struct node* n = malloc(sizeof(struct node));
    memset( n, 0, sizeof(*n));

    strcpy( n->name.str, name );
    sc_map_init_32v( &n->neigh, 0, 0 );

    return n;
}

/*
 * Node with 'name' MUST be initialized and inserted in
 * the global nodes
 */
struct node* GetNode( const char* name )
{
    struct node* n = sc_map_get_32v( &allNodes, STR_TO_HASH(name) );
    assert( sc_map_found(&allNodes) );

    return n;
}

/*
 * Initialize nodes n1 and n2 if they don't exist.
 * Then connect them to each other
 */
void ConnectNeigh( const char* n1, const char* n2 )
{
    assert( n1 );
    assert( n2 );

    struct node* node1;
    struct node* node2;

    node1 = sc_map_get_32v( &allNodes, STR_TO_HASH(n1) );
    if( ! sc_map_found( &allNodes ) ){
        node1 = MakeNode( n1 );
        sc_map_put_32v( &allNodes, node1->name.hash, node1 );
    }

    node2 = sc_map_get_32v( &allNodes, STR_TO_HASH(n2) );
    if( ! sc_map_found( &allNodes ) ){
        node2 = MakeNode( n2 );
        sc_map_put_32v( &allNodes, node2->name.hash, node2 );
    }

    sc_map_put_32v( &node1->neigh, node2->name.hash, node2 );
    sc_map_put_32v( &node2->neigh, node1->name.hash, node1 );
}

int HasNeighbor( struct node* n1, struct node* n2 )
{
    struct node* n = sc_map_get_32v( &n1->neigh, n2->name.hash );
    return sc_map_found(&n1->neigh);
}

void PrintNode( const struct node* n )
{
    assert( n );
    struct node* neigh;

    printf( "Node %s, %d neigh: ", n->name.str, sc_map_size_32v(&n->neigh) );
    sc_map_foreach_value( &n->neigh, neigh ) {
        printf( "%s ", neigh->name.str );
    }
    printf( "\n");
}

void PrintAllNodes()
{
    struct node* node;

    sc_map_foreach_value( &allNodes, node ) {
        PrintNode( node );
    }
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* n )
{
    sc_map_init_32v( &allNodes, 0, 0 );
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    char* temp = strchr( line, '-' );
    *temp = '\0';

    char* n1 = line;
    char* n2 = temp+1;

    ConnectNeigh( n1, n2 );
}

void InitClique( struct sc_map_32v* clique )
{
    sc_map_init_32v( clique, 0, 0 );
}

int IsInClique( struct sc_map_32v* clique, const char* name )
{
    assert( clique );
    assert( name );

    struct node* node = GetNode( name );

    struct node* iternode;
    sc_map_foreach_value( clique, iternode ){
        if( ! HasNeighbor( node, iternode ) ){
            return 0;
        }
    }

    // Node of name 'name' must be neighbor of EVERY single node in clique
    return 1;
}

int cmpnode( const void* n1, const void* n2 )
{
    // QSort uses pointers to elements...
    return strcmp( *((char**) n1), *((char**) n2) );
}

void PrintPassword( struct sc_map_32v* clique )
{
    #define MAXNUM 13
    char* names[MAXNUM];
    int i = 0;

    struct node* node;

    sc_map_foreach_value( clique, node ){
        names[i] = node->name.str;

        i++;
        assert( i <= MAXNUM );
    }

    qsort( names,
            i,
            sizeof( names[0] ),
            cmpnode
         );

    INFO( "Password:\n");
    for( int n = 0; n < i; n++ ){
        INFO( "%s", names[n] );
        if( n != i-1 ){
            INFO( ",");
        }
    }
    INFO( "\n");
}

void SOLVER_PrintSolution( void )
{
    int maxsize = 0;

    struct node* node;


    sc_map_foreach_value( &allNodes, node ){

        struct sc_map_32v clique;
        InitClique( &clique );

        sc_map_put_32v( &clique, node->name.hash, node );

        VERBOSE( "Init Clique with %s\n", node->name.str );
        struct node* iternode;
        sc_map_foreach_value( &allNodes, iternode ){
            if( iternode == node )
                continue;

            if( IsInClique( &clique, iternode->name.str ) ){
                sc_map_put_32v( &clique, iternode->name.hash, iternode );
                VERBOSE( "  put node %s\n", iternode->name.str );
            }
        }

        int size = sc_map_size_32v( &clique );
        if( size > maxsize ) {
            maxsize = size;
            INFO( "new maxclique of size %d:\n", size );
            sc_map_foreach_value( &clique, iternode ){
                INFO( "  node %s\n", iternode->name.str );
            }

            PrintPassword( &clique );
        }

        // Destroy
        sc_map_term_32v( &clique );
    }
}
