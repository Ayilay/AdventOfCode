#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <math.h>

#define NDEBUG
#define NVERBOSE
#define NINFO

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX 20

#define ADD     0
#define MUL     1
#define CONCAT  2

int numSolvable = 0;
unsigned long long totalCorrectAccum = 0;

unsigned long long accum_results[ 400 ];
int accum_n = 0;

int getDigitBase3( unsigned int opVec, int index )
{
    if( index > 0 )
        opVec /= (index * 3);

    opVec %= 3;

    return opVec;
}

unsigned long long intpow10( int exp )
{
    unsigned long long result = 1;
    while( exp-- ){
        result *= 10;
    }

    return result;
}

int isSolvable( unsigned long long sol, unsigned long long* inputs, int n )
{
    assert( n > 1 );

    int opVector = (3 * n-1) - 1;

    VERBOSE( "Solution: %lld, numbers: ", sol );
    for( int i = 0; i < n; i++) {
        printf( "%llu ", inputs[i] );
    }
    printf("\n");

    unsigned long long accum;
    while( opVector >= 0 )
    {
        VERBOSE( "OpVector: %d (%d inputs) \n", opVector, n );

        accum = inputs[0];
        DEBUG( "Starting with %llu\n", accum );

        // For each operation in the operations vector
        for( int i = 0; i < n-1; i++ )
        {
            unsigned long long num = inputs[ i+1 ];

            switch( getDigitBase3( opVector, i ) ){
                case CONCAT:
                    DEBUG( "Concatenating %llu\n", num );
                    accum *= intpow10( ndigs(num) );
                    accum += num;
                    break;

                case ADD:
                    DEBUG( "Adding %llu\n", num );
                    accum += num;
                    break;

                case MUL:
                    accum *= num;
                    DEBUG( "Multiplying %llu\n", num );
                    break;
            }

            DEBUG( "Result so far: %llu\n", accum );
        }

        if( sol == accum ){
            INFO( "Found solution for %llu\n", sol );
            return 1;
        }
        DEBUG( "Not Solvable, Trying next OpVec\n\n" );

        opVector --;
    }

    DEBUG( "Not Solvable at all\n\n" );
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

    unsigned long long inputs[ MAX ];

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

    unsigned long long num;
    int n = 0;
    do {
        num = strtoull( line, &endptr, 10 );

        inputs[ n++ ] = num;
        assert( n < MAX );

        // Advance to end and skip the newline
        line = endptr;
        line++;
    }
    while( *endptr );

    if( isSolvable( sol, inputs, n ) ){
        totalCorrectAccum += sol;
        INFO( "ACCUM: %llu\n\n", sol );

        accum_results[ accum_n ] = sol;
        accum_n ++;
    }
}

void SOLVER_PrintSolution( void )
{
    printf( "totalAccum: %llu\n\n", totalCorrectAccum );

    printf( "nCorrect: %d\n", accum_n );
    for( int i = 0; i < accum_n; i++ ){
        printf( "%llu\n", accum_results[i]);
    }

}
