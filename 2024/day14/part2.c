#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

// ACTUAL
#define DIMX    101
#define DIMY    103

// SAMPLE
//#define DIMX    11
//#define DIMY    7

#define CDIMX    50
#define CDIMY    51

int q1 = 0;
int q2 = 0;
int q3 = 0;
int q4 = 0;

int N_ITER = 100;

#define MAX 501
int N = 0;
int robX[ MAX ];
int robY[ MAX ];

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
    if( arg ){
        N_ITER = (int) arg;
    }

    //printf( "NITER: %d\n", N_ITER );
}

void incQuadrant( long finx, long finy )
{
    if( finx == CDIMX || finy == CDIMY )
        return;

    // Doesn't matter "which quadrand is which"

    if( finx <= CDIMX && finy <= CDIMY )
        q1 ++;
    else if( finx <= CDIMX && finy >= CDIMY )
        q2 ++;
    else if( finx >= CDIMX && finy <= CDIMY )
        q3 ++;
    else if( finx >= CDIMX && finy >= CDIMY )
        q4 ++;
    else{
        printf( "ERROR: %ld, %ld\n", finx, finy );
        assert(0);
    }
}

long signedmod( long a, long m )
{
    while( a < 0 ){
        a += m;
    }

    return a % m;
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    int px, py, vx, vy;
    int nscanned;

    nscanned = sscanf( line, "p=%d,%d v=%d,%d",
            &px,
            &py,
            &vx,
            &vy
          );

    assert( nscanned == 4 );
    //printf( "TheP: %d,%d, theV: %d,%d\n", px,py,vx,vy);

    long finx, finy;
    finx = px + (vx * N_ITER);
    finx = signedmod( finx, DIMX);
    finy = py + (vy * N_ITER);
    finy = signedmod( finy, DIMY);

    robX[ N ] = finx;
    robY[ N ] = finy;
    N++;
    assert( N < MAX );

    //DEBUG( "finx: %ld, finy: %ld\n", finx, finy );
    incQuadrant( finx, finy );
}

void printRobots()
{
    char grid[DIMY][DIMX];
    memset( grid, ' ', sizeof(grid) );

    for( int i = 0; i < N; i++ ){
        int x = robX[i];
        int y = robY[i];
        assert( x < DIMX );
        assert( y < DIMY );

        grid[y][x] = 'x';
    }

    for( int r = 0; r < DIMY; r++ ){
        for( int c = 0; c < DIMX; c++ ){
            printf( "%c", grid[c][r] );
        }
        printf( "\n");
    }
}

void SOLVER_PrintSolution( void )
{
    printRobots();

    long total = q1 * q2 * q3 * q4;
    printf( "danger level %012ld for %d iter\n", total, N_ITER );
}
