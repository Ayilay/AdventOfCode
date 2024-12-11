#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <math.h>

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

int isConcat( int vec, int ind )
{
    return( getDigitBase3( vec, ind ) == CONCAT );
}

// OLD LOGIC
// unsigned long long n1, n2, concatResult;
// n1 = inputs[i];
// n2 = inputs[i+1];

// concatResult = n1 * intpow10(ndigs(n2));
// concatResult += n2;
// VERBOSE( "Concatenating %llu and %llu\n", n1, n2 );
// VERBOSE( "Result: %llu\n", concatResult );

// numConcats ++;

int doConcatenations(
        unsigned long long* inputs,
        int                 nInputs, 
        unsigned long long* outputs,
        int                 opVector
        )
{
    assert( nInputs > 1 );

    VERBOSE( "Concatenating list: ");
    for( int i = 0; i < nInputs; i++) {
        VERBOSE( "%llu ", inputs[i] );
    }
    printf("\n");

    int iOut = 0;
    unsigned long long n1, n2, concatResult;

    outputs[0] = inputs[0];

    // For each input in the opVec, do CONCAT if indicated
    for( int i = 0; i < nInputs-1; i++ ){
        if( isConcat( opVector, i )){
            n1 = outputs[iOut];
            n2 = inputs[i+1];

            concatResult = n1 * intpow10(ndigs(n2));
            concatResult += n2;

            outputs[iOut] = concatResult;
        } 
        else {
            // no concat; copy input to output
            // i is opVec index, but i+1 is input index
            iOut ++;
            outputs[iOut] = inputs[i+1];
        }
    }

    iOut ++;
    VERBOSE( "Done Concatenating: ");
    for( int i = 0; i < iOut; i++) {
        VERBOSE( "%llu ", outputs[i] );
    }
    printf("\n");

    return iOut;
}

int isSolvable( unsigned long long sol, unsigned long long* inputs, int n )
{
    assert( n > 1 );

    int nConcatInputs = 0;
    unsigned long long concatInputs[ MAX ];

    int opVector = (3 * n-1) - 1;

    VERBOSE( "Solution: %lld, numbers: ", sol );
    for( int i = 0; i < n; i++) {
        printf( "%llu ", inputs[i] );
    }
    printf("\n");

    unsigned long long accum;
    while( opVector >= 0 )
    {
        memset( concatInputs, 0, sizeof(concatInputs) );
        nConcatInputs = 0;

        VERBOSE( "OpVector: %d (%d inputs) \n", opVector, n );

        nConcatInputs = doConcatenations( inputs, n, concatInputs, opVector );

        accum = concatInputs[0];
        DEBUG( "Starting with %llu\n", accum );
        for( int i = 1; i < nConcatInputs; i++ ){
            unsigned long long num = concatInputs[i];

            switch( getDigitBase3( opVector, i ) ){
                case CONCAT:
                    // SKIP; we just did these
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
        printf( "ACCUM: %llu\n\n", sol );
    }
}

void SOLVER_PrintSolution( void )
{
    printf( "totalAccum: %llu\n\n", totalCorrectAccum );
}
