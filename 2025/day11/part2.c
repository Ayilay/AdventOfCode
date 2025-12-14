#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE
#define NDEBUG

#include "sc_disjoint.h"
#include "sc_map.h"
#include "sc_queue.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

typedef struct sc_disjoint_node ufn;

#define MAXNEIGH 40

typedef struct node {
    char name[4];

    int nout;
    struct node* out[ MAXNEIGH ];

    int nin;
    struct node* in[ MAXNEIGH ];
    ufn set;
} node;

struct sc_map_sv nodes;

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( int argc, char* argv[] )
{
    sc_map_init_sv( &nodes, 0, 50 );
}

node* InitNode( const char* name )
{
    node* n;

    n = malloc( sizeof(node) );
    memset( n, 0, sizeof(*n) );

    strcpy( n->name, name );
    sc_disjoint_init( &n->set, &n );

    return n;
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );
    
    line = strtok( line, ":" );

    node* n = sc_map_get_sv( &nodes, line );
    if( ! sc_map_found( &nodes ) ){
        n = InitNode( line );
        sc_map_put_sv( &nodes, n->name, n );
    }

    line = strtok( NULL, " " );
    while( line != NULL ){
        node* out = sc_map_get_sv( &nodes, line );
        if( ! sc_map_found( &nodes ) ){
            out = InitNode( line );
            sc_map_put_sv( &nodes, out->name, out );
        }

        // Connect adjacent (outbound) neighbor
        n->out[n->nout] = out;
        n->nout ++;
        assert( n->nout < MAXNEIGH );

        // Connect reverse neighbor
        out->in[ out->nin ] = n;
        out->nin ++;
        assert( n->nin < MAXNEIGH );


        line = strtok( NULL, " " );
    }
}

struct sc_map_s64 pathMemo;

ull CalcNPaths( node* this, node* fin )
{
    assert( this );
    assert( fin );

    // This case should not be possible; We should check before we recurse
    if( this == fin )
        return 0;

    VERBOSE( "Counting npaths for %s->%s\n", this->name, fin->name);
    ull npaths = sc_map_get_s64( &pathMemo, this->name );
    if( sc_map_found( &pathMemo ) ){
        VERBOSE( "Memo'd npaths for %s: %lld\n", this->name, npaths );
        return npaths;
    }

    for( int i = 0; i < this->nout; i++ ){
        node* out = this->out[i];

        if( out == fin ){
            VERBOSE( "  Found fin(%s), incrementing\n", fin->name );
            npaths ++;
        } else {
            VERBOSE( "  Recurse BEGIN %s->%s\n", out->name, fin->name );
            npaths += CalcNPaths( out, fin );
            VERBOSE( "  Recurse END %s->%s\n", out->name, fin->name );
        }
    }

    DEBUG( "Found %lld paths for %s->%s\n", npaths, this->name, fin->name );
    sc_map_put_s64( &pathMemo, this->name, npaths );
    return npaths;
}

ull DoSolution( char* nameBegin, char* nameEnd )
{
    node* nodeBegin;
    node* nodeEnd;

    nodeBegin = sc_map_get_sv( &nodes, nameBegin );
    assert( sc_map_found(&nodes) );

    nodeEnd = sc_map_get_sv( &nodes, nameEnd );
    assert( sc_map_found(&nodes) );

    // Ultra-low load factor, keep this one efficient
    sc_map_init_s64( &pathMemo, MAX_INPUT, 30 );
    ull npaths = CalcNPaths( nodeBegin, nodeEnd);
    sc_map_term_s64( &pathMemo );

    return npaths;
}

void SOLVER_PrintSolution( void )
{

    ull npaths = 1;
    char* beg;
    char* end;

    //////////////////////////////////////////////////////////////////////////////////
    /// Path one (it's actually BOGUS for my input)
    //////////////////////////////////////////////////////////////////////////////////

    beg = "svr";
    end = "dac";
    npaths *= DoSolution( beg, end );
    INFO( "Running total %lld paths for %s, %s\n", npaths, beg, end );

    beg = "dac";
    end = "fft";
    npaths *= DoSolution( beg, end );
    INFO( "Running total %lld paths for %s, %s\n", npaths, beg, end );

    beg = "fft";
    end = "out";
    npaths *= DoSolution( beg, end );
    INFO( "Running total %lld paths for %s, %s\n", npaths, beg, end );

    //////////////////////////////////////////////////////////////////////////////////
    /// Path two
    //////////////////////////////////////////////////////////////////////////////////
    npaths = 1;

    beg = "svr";
    end = "fft";
    npaths *= DoSolution( beg, end );
    INFO( "Running total %lld paths for %s, %s\n", npaths, beg, end );

    beg = "fft";
    end = "dac";
    npaths *= DoSolution( beg, end );
    INFO( "Running total %lld paths for %s, %s\n", npaths, beg, end );

    beg = "dac";
    end = "out";
    npaths *= DoSolution( beg, end );
    INFO( "Running total %lld paths for %s, %s\n", npaths, beg, end );

    //////////////////////////////////////////////////////////////////////////////////
    /// For fun, count all paths from svr to out
    //////////////////////////////////////////////////////////////////////////////////
    npaths = 1;
    beg = "svr";
    end = "out";
    npaths *= DoSolution( beg, end );
    INFO( "Running total %lld paths for %s, %s\n", npaths, beg, end );
}

