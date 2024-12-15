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

#define N_ITER  100

int q1 = 0;
int q2 = 0;
int q3 = 0;
int q4 = 0;


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
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
    printf( "TheP: %d,%d, theV: %d,%d\n", px,py,vx,vy);

    long finx, finy;
    finx = px + (vx * N_ITER);
    finx = signedmod( finx, DIMX);
    finy = py + (vy * N_ITER);
    finy = signedmod( finy, DIMY);

    DEBUG( "finx: %ld, finy: %ld\n", finx, finy );
    incQuadrant( finx, finy );
}

void SOLVER_PrintSolution( void )
{
    long total = q1 * q2 * q3 * q4;
    printf( "total: %ld\n", total );
}
