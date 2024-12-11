#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <math.h>

#define NVERBOSE
#define NDEBUG
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX 20

int numSolvable = 0;
unsigned long long totalCorrectAccum = 0;

int isSolvable( unsigned long long sol, unsigned int* inputs, int n )
{
    assert( n > 1 );

    int opVector = (1 << n-1) - 1;

    VERBOSE( "Solution: %d, numbers: ", sol );
    for( int i = 0; i < n; i++) {
        printf( "%u ", inputs[i] );
    }
    printf("\n");

    unsigned long long accum;
    while( opVector >= 0 )
    {
        VERBOSE( "OpVector: 0x%x (%d inputs) \n", opVector, n );
        accum = inputs[0];
        DEBUG( "Starting with %d\n", accum );
        for( int i = 1; i < n; i++ ){
            long num = inputs[i];

            if( opVector & (1 << (i-1)) ){
                accum += num;
                DEBUG( "Adding %d\n", num );
            } else {
                accum *= num;
                DEBUG( "Multiplying %d\n", num );
            }
            DEBUG( "Result so far: %d\n", accum );
        }

        if( sol == accum ){
            INFO( "Found solution for %llu\n", sol );
            return 1;
        }
        DEBUG( "Trying next\n\n" );

        opVector --;
    }

    return 0;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    unsigned long long sol;
    char* endptr;

    unsigned int inputs[ MAX ];

    errno = 0;
    sol = strtoull( line, &endptr, 10 );
    if( errno ){
        perror( "Error during strtol\n" );
        printf( "Input that caused error: '%s'\n", line );
        assert( 0 );
    }
    assert( errno == 0 );

    // Skip the colon and whitespace
    line = endptr;
    line += 2;

    unsigned int num;
    int n = 0;
    do {
        num = strtoul( line, &endptr, 10 );

        inputs[ n++ ] = num;
        assert( n < MAX );

        // Advance to end and skip the newline
        line = endptr;
        line++;
    }
    while( *endptr );

    if( isSolvable( sol, inputs, n ) ){
        totalCorrectAccum += sol;
    }
}

void SOLVER_PrintSolution( void )
{
    printf( "totalAccum: %llu\n", totalCorrectAccum );
}
