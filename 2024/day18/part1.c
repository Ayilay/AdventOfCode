#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "sc_heap.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

// forward decl for struct reference
typedef struct node node;

typedef struct node {
    int isVisited;
    int isObstacle;

    node* prev;
} node;

#define MAX 7

node grid[MAX][MAX] = {0};

// Per problam: Scan only finite number of lines
#define MAX_N 12
int N = 0;

void printGrid( node grid[MAX][MAX], int n )
{
    for( int r = 0; r < n; r++ ){
        for( int c = 0; c < n; c++ ){
            node* e = &grid[r][c];
            if( e->isObstacle ){
                printf( "#" );
            } else {
                printf( "." );
            }
        }
        printf( "\n");
    }
}

int isUnvisited( node* n )
{
    assert( n );

    if( n->isObstacle )
        return 0;

    if( n->isVisited )
        return 0;

    return 1;
}

int getUnvisitedNeighbors( node in[MAX][MAX], int x, int y, node* neigh[4] )
{
    int nneigh = 0;
    node* e;

    if( x > 0 ){
        e = &in[y][x-1];
        if( isUnvisited( e ) ){
            neigh[ nneigh++ ] = e;
        }

    }
    if( x < MAX-1 ){
        e = &in[y][x+1];
        if( isUnvisited( e ) ){
            neigh[ nneigh++ ] = e;
        }

    }
    if( y > 0 ){
        e = &in[y-1][x];
        if( isUnvisited( e ) ){
            neigh[ nneigh++ ] = e;
        }

    }
    if( y < MAX-1 ){
        e = &in[y+1][x];
        if( isUnvisited( e ) ){
            neigh[ nneigh++ ] = e;
        }

    }

    return nneigh;
}


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
}

void SOLVER_ProcessLine( char* line )
{
    if( N >= MAX_N )
        return;

    rmnewl( line );

    int x, y;

    int nscan = sscanf( line, "%d,%d\n", &x, &y );
    assert( nscan == 2 );
    assert( x < MAX );
    assert( y < MAX );

    node* elem = &(grid[y][x]);
    elem->isObstacle = 1;

    N++;
}

void SOLVER_PrintSolution( void )
{
    printGrid( grid, MAX );
}
