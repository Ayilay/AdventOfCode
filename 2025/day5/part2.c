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

#define MAX 2000


ull totalHealthy = 0;

void processRange( const char* line, ull* totHealthy)
{
    char* eptr;
    ull begin = strtoll( line, &eptr, 10);
    assert( *eptr == '-' );

    line = eptr+1;
    ull end = strtoll( line, NULL, 10);

    ull ans = end - begin + 1;
    INFO( "%lld healthy for %lld-%lld\n", ans, begin, end );

    (*totHealthy) += ans;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    static int doingRange = 1;

    if( strlen( line ) == 0 ){
        doingRange = 0;
        return;
    }

    // We no longer process IDs for part2
    if( doingRange )
        processRange( line, &totalHealthy );
    else
        return;
}

void SOLVER_PrintSolution( void )
{
    printf( "Total Healthy:\n%lld\n", totalHealthy );
}
