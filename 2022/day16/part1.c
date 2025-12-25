#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE

#define USE_MEMO

#include "sc_queue.h"
#include "sc_map.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAXNEIGH 20

struct node {
    char name[4];   // 2chars + null + alignment padding
    int flow;

    // Direct neighbors
    int nneigh;
    struct node* neigh[MAXNEIGH];

    // Distance to ALL other nodes that have non-zero flows
    struct sc_map_sll distances;

    // Traversal Metadata
    int dist;
};

struct sc_map_sv allNodes;
struct sc_map_sv nonZeroNodes;

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void )
{
    sc_map_init_sv( &allNodes, 0, 0 );
    sc_map_init_sv( &nonZeroNodes, 0, 0 );
}

struct node* InitNode( const char* name )
{
    struct node* node;
    node = malloc( sizeof(struct node) );
    assert( node );

    memset( node, 0, sizeof(*node));
    memcpy( node->name, name, strlen(name) );

    return node;
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    // Use strtok but with various delimiters to skip over most junk
    // line == Name of first node
    line = strtok( line, " " );
    line = strtok( NULL, " " );

    // Node with this name might already be initialized
    struct node* node = sc_map_get_sv( &allNodes, line );
    if( ! sc_map_found(&allNodes) ){
        node = InitNode( line );
    }

    // We insert the node after we have parsed the name
    if( ! sc_map_found(&allNodes) ){
        sc_map_put_sv( &allNodes, node->name, node );
    }

    // line == flow amount
    line = strtok( NULL, "=" );
    line = strtok( NULL, ";" );
    node->flow = atoi( line );
    if( node->flow != 0 ){
        // Store separate map of all non-zero-flow nodes
        sc_map_put_sv( &nonZeroNodes, node->name, node );
    }

    // Hack: Replace the null terminator with valid dummy character
    // to skip forwards a bunch, then restart strtok parsing
    line[ strlen(line) ] = 'a';
    line = strstr( line, "valve" );
    line = strtok( line, " ");
    line = strtok( NULL, " ,");
    while( line != NULL ){
        if( strlen(line) == 0 ){
            line = strtok( NULL, " ,");
            continue;
        }

        if( 0 == strcmp(line, "FF") ){
            // GDB hook
            int found = 1;
        }

        // line == name of node neighbor
        struct node* neigh = sc_map_get_sv( &allNodes, line );
        if( ! sc_map_found(&allNodes) ){
            neigh = InitNode( line );
            sc_map_put_sv( &allNodes, neigh->name, neigh );
        }

        node->neigh[ node->nneigh ] = neigh;
        node->nneigh ++;
        assert( node->nneigh < MAXNEIGH );

        line = strtok( NULL, " ,");
    }
}

void ResetMetadata()
{
    struct node* node;
    const char* key;

    // Initialize traversal metadata
    sc_map_foreach( &allNodes, key, node ){
        node->dist = 0;
    }
}

void CalcDistances( struct node* node )
{
    ResetMetadata();

    struct node* origNode = node;

    struct sc_queue_ptr nodeq;
    sc_queue_init( &nodeq );

    // We do BFS starting at origNode
    node->dist = 0;
    sc_queue_add_last( &nodeq, node );
    while( ! sc_queue_empty( &nodeq ) ){

        node = sc_queue_del_first( &nodeq );
        for( int i = 0; i < node->nneigh; i++ ){
            struct node* neigh;
            neigh = node->neigh[i];
            if( neigh->dist != 0 )
                continue;

            neigh->dist = node->dist + 1;
            sc_queue_add_last( &nodeq, neigh );

            // Only store this node in the known-distances of the original node
            // IF it has a non-zero flow
            sc_map_get_sv( &nonZeroNodes, neigh->name );
            if( sc_map_found( &nonZeroNodes )){
                sc_map_put_sll( &origNode->distances, neigh->name, neigh->dist );
            }
        }
    }
}

void CalcAllDistances( void )
{
    struct node* node;
    const char* key;

    // Initialize traversal storage for ALL nodes
    sc_map_foreach( &allNodes, key, node ){
        sc_map_init_sll( &node->distances, 0, 0 );
    }

    // Calculate distances from each node to every other node
    sc_map_foreach( &allNodes, key, node ){
        CalcDistances( node );
    }
}

void InitCopySet( struct sc_map_sv* dst, const struct sc_map_sv* src )
{
    const char* key;
    void* val;

    sc_map_init_sv( dst, src->cap, src->load_fac );
    sc_map_foreach( src, key, val ){
        sc_map_put_sv( dst, key, val );
    }
}

ull MemoHashNodes( const char* origin, int mins, const struct sc_map_sv* options )
{
    /*
     * Each unique combination of origin + remaining minutes +
     * remaining options must have a unique hash
     */

    ull hash = 0;
    const char* key;

    // Order of options within set MUST NOT matter
    sc_map_foreach_key( options, key ){
        // Treat each name as a 2-char number
        ull val = *((uint16_t*) key);

        hash += val;
    }

    hash *= 1000;
    hash += *((uint16_t*) origin);

    hash *= 1000;
    hash += mins;

    return hash;
}

struct sc_map_64 memo;
ull memoHits = 0;

struct node* GetNode( const char* name )
{
    struct node* n = sc_map_get_sv( &allNodes, name );
    assert( sc_map_found( &allNodes ));

    return n;
}

int GetDist( const char* orig, const char* dest )
{
    struct node* nOrig = GetNode(orig);

    int dist = sc_map_get_sll( &nOrig->distances, dest );
    assert( sc_map_found( &nOrig->distances ));

    return dist;
}

int GetFlow( const char* orig)
{
    struct node* nOrig = GetNode(orig);

    return nOrig->flow;
}

void PrintState (
        const char* origin,
        int mins,
        const struct sc_map_sv* availNodes,
        ull maxFlow )
{
    printf( "@ node %s w/ %d mins remain. Options: ",
            origin, mins );

    const char* neigh;
    sc_map_foreach_key( availNodes, neigh ){
        int dist = GetDist(origin, neigh);
        printf( "%s(%d) ", neigh, dist );
    }
    printf( "-> MaxFlow is %lld\n", maxFlow );
}


/* Recursive + Caching function
 */
ull CalcMaxFlow(
        const char* origin,
        int minutesRemain,
        const struct sc_map_sv* availNodes,
        int recurseDepth
){
    ull maxFlow = -1;

    #ifdef USE_MEMO
    ull hash = MemoHashNodes( origin, minutesRemain, availNodes );
    // Search for cached solution first. Return it if found
    maxFlow = sc_map_get_64( &memo, hash );
    if( sc_map_found( &memo ) ) {
        VERBOSE( "%*s[%2d] Cached MaxFlow %lld for %s\n",
                recurseDepth, "", minutesRemain,
                maxFlow, origin );

        memoHits ++;

        return maxFlow;
    }
    #endif

    // It takes one minute to open a valve. With 1 minute remaining,
    // opening this valve has no effect
    if( minutesRemain <= 1 )
        return 0;

    // No available nodes to explore. We open this valve only
    if( 0 == sc_map_size_sv(availNodes) ) {
        minutesRemain --;
        ull flow = GetFlow(origin) * (minutesRemain);
        VERBOSE( "%*s[%2d] None reachable from %s (flow %d). Flow is %lld\n",
                recurseDepth, "",
                minutesRemain, origin, GetFlow(origin), flow );
        return flow;
    }

    maxFlow = -1;

    // IF this valve is worth opening, open it; costs one minute
    if( GetFlow(origin) > 0)
        minutesRemain --;

    const char* neigh;
    sc_map_foreach_key( availNodes, neigh ){

        int timeDelta = minutesRemain - GetDist(origin, neigh);
        // If neighbor is not reachable given time remaining, skip it
        if( timeDelta < 1 )
            continue;

        // Copy the availableNodes set, and remove the neighbor from it
        // Recursively descend into the neighbour
        struct sc_map_sv neighAvailNodes;
        InitCopySet( &neighAvailNodes, availNodes );
        sc_map_del_sv( &neighAvailNodes, neigh );

        VERBOSE( "%*s[%2d] Recurse: from %s, try %s (dist %d, flow %d)\n",
                    recurseDepth, "",
                    minutesRemain, origin, neigh, GetDist(origin,neigh), GetFlow(neigh) );

        ull thisNeighMax = CalcMaxFlow( neigh, timeDelta, &neighAvailNodes, recurseDepth+1 );
        if( maxFlow == -1 || thisNeighMax > maxFlow ){
            maxFlow = thisNeighMax;
            VERBOSE("%*s[%2d] new max from neighbor %s: %lld\n",
                    recurseDepth, "", minutesRemain,
                    neigh, maxFlow);
        }
    }

    // maxFlow contains only maxFlow from neighbor. So we add the flow amount
    // from opening the current node
    ull self = GetFlow(origin) * (minutesRemain);
    maxFlow += self;

    #ifdef USE_MEMO
    sc_map_put_64( &memo, hash, maxFlow );
    #endif
    return maxFlow;
}

void SOLVER_PrintSolution( void )
{
    struct node* val;
    const char* key;

    CalcAllDistances();

    const char* nextOptimalNode;
    struct sc_map_sv availNodes;

    InitCopySet( &availNodes, &nonZeroNodes );
    sc_map_del_sv( &availNodes, "AA" );
    
    sc_map_init_64( &memo, 0, 0);
    ull maxFlow = CalcMaxFlow( "AA", 30, &availNodes, 1 );

    INFO( "maxFlow: %lld\n", maxFlow );

    // Stats
    INFO( "Memo hits: %lld\n", memoHits );
}

void PrintNodes()
{
    struct node* val;
    const char* key;

    INFO( "\nDumping all nodes:\n" );
    sc_map_foreach( &allNodes, key, val ){
        INFO( "Node %s, flow %2d. Neigh: ", val->name, val->flow );
        for( int i = 0; i < val->nneigh; i++ ){
            struct node* neigh = val->neigh[i];
            INFO( "-%s- ", neigh->name );
        }
        INFO( "\n");
    }
}

void PrintNeighDist( struct node* node )
{
    const char* name;
    ull dist;

    printf( "Neighbors of %s:\n", node->name );
    sc_map_foreach( &node->distances, name, dist ){
        printf( "  %s: %2lld\n", name, dist );
    }
}

