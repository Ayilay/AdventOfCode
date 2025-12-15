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

// How many numbers on the dial
// 100 means numbers 0-99
#define CIRCUMFERENCE   100

int dialPosition = 50;
int totNumZeros = 0;

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

int RotateL( int *dial, int amount )
{
    int numZeros = 0;

    if( *dial == 0 ){
        *dial = 100;
    }

    if( amount == 0 ){
        return 0;
    }

    // Rotate to reach exaclty zero
    else if( amount == *dial ){
        *dial = 0;
        numZeros++;
    } 

    // Rotate, but not enough to reach a zero
    else if( amount < *dial ){
        *dial -= amount;
    }

    // Rotate a full revolution; we definitely pass a zero
    else if( amount >= CIRCUMFERENCE ){
        numZeros++;
        numZeros += RotateL( dial, amount - CIRCUMFERENCE );
    }

    // Rotate enough to pass the zero, but not enough for a full revolution
    else {
        numZeros++;

        int delta = *dial;
        *dial = 100;
        amount -= delta;

        // Note: This will always return zero
        RotateL( dial, amount );
    }

    return numZeros;
}

int RotateR( int* dial, int amount )
{
    int numZeros = 0;

    if( *dial == 100 ){
        *dial = 0;
    }

    if( amount == 0 ){
        return 0;
    }

    // Rotate to reach exaclty zero
    else if( amount == (CIRCUMFERENCE-*dial) ){
        *dial = 0;
        numZeros++;
    } 

    // Rotate, but not enough to reach a zero
    else if( amount < (CIRCUMFERENCE-*dial) ){
        *dial += amount;
    }

    // Rotate a full revolution; we definitely pass a zero
    else if( amount >= CIRCUMFERENCE ){
        numZeros++;
        numZeros += RotateR( dial, amount - CIRCUMFERENCE );
    }

    // Rotate enough to pass the zero, but not enough for a full revolution
    else {
        numZeros++;

        int delta = (CIRCUMFERENCE - *dial);
        *dial = 0;
        amount -= delta;

        // Note: This will always return zero
        RotateR( dial, amount );
    }

    return numZeros;
}

int rotateDial( int* dial, int dir, int amount )
{
    char* header = "";
    int numZeros;

    if( dir == 'L' ) {
        numZeros = RotateL( dial, amount );
    } else if( dir == 'R' ) {
        numZeros = RotateR( dial, amount );
    } else {
        printf( "Unexpected direction %c, aborting", dir );
        abort();
    }

    return numZeros;
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    int dir;
    int amount;

    dir = line[0];
    amount = atoi( &line[1] );

    totNumZeros += rotateDial( &dialPosition, dir, amount );
}

void SOLVER_PrintSolution( void )
{
    printf( "Number of zeros:\n%d\n", totNumZeros );
}


//------------------------------------------------------------------------------
//  Various Utilities
//------------------------------------------------------------------------------

/** Find the last newline of a string, and replace it with a null character.
 */
void rmnewl( char* s )
{
    char* ptr = strrchr( s, '\n' );

    if( ptr ) {
        *ptr = '\0';
    }
}

/** Calculate how many digits in a (positive) number
 */
int ndigs( ull n )
{
    if( n == 0 ){
        return 1;
    }

    int digs = 0;
    while( n > 0 ){
        digs ++;
        n /= 10;
    }

    return digs;
}
