#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NDEBUG
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX 1024

// ASSUME that DIMX == DIMY
int DIM = 0;
char grid[MAX][MAX];

int isNeigh( char g[][MAX], int dim, int r, int c )
{
    if( r < 0 ) return 0;
    if( c < 0 ) return 0;

    if( r >= dim ) return 0;
    if( c >= dim ) return 0;

    return g[r][c] == '@';
}

int countNeighbors( char g[][MAX], int dim, int r, int c )
{
    int neigh = 0;

    neigh += isNeigh( g, dim, r-1, c-1 );
    neigh += isNeigh( g, dim, r-1, c   );
    neigh += isNeigh( g, dim, r-1, c+1 );

    neigh += isNeigh( g, dim, r  , c-1 );
    neigh += isNeigh( g, dim, r  , c+1 );

    neigh += isNeigh( g, dim, r+1, c-1 );
    neigh += isNeigh( g, dim, r+1, c   );
    neigh += isNeigh( g, dim, r+1, c+1 );

    return neigh;
}

// Return 1 is true, 0 otherwise
int isCandidate( char g[][MAX], int dim, int r, int c )
{
    if( g[r][c] != '@' ) return 0;

    int ans = countNeighbors( g, dim, r, c);
    if( ans < 4 ){
        DEBUG( "Cell @ [%2d][%2d] has %d neigh\n", r, c, ans );
    }
    return ans < 4;
}

int FindAllCandidates( char g[][MAX], int dim)
{
    int total = 0;
    int yes;
    for( int r = 0; r < dim; r++ ) {
        for( int c = 0; c < dim; c++ ) {
            yes = isCandidate( g, dim, r, c );
            if( yes ) {
                INFO( "Candidate @ [%2d][%2d]\n", r, c );
            }
            total += yes;
        }
    }

    return total;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    static int N = 0;
    rmnewl( line );

    DIM = strlen( line );
    strncpy( grid[N], line, DIM );
    N++;
}

void SOLVER_PrintSolution( void )
{
    int total = FindAllCandidates( grid, DIM );
    printf( "%d\n", total );
}
