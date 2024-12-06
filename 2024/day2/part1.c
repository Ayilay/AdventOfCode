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

#define VERBOSE( ... )  printf( __VA_ARGS__ )
#define DEBUG( ... )  printf( __VA_ARGS__ )
#define INFO( ... )  printf( __VA_ARGS__ )

//#define VERBOSE( ... ) // NOTHING
//#define DEBUG( ... ) // NOTHING
//#define INFO( ... ) // NOTHING

int totNumSafe = 0;

static void eatWhitespace( char** pStr )
{
    while( **pStr == ' '){
        (*pStr)++;
    }
}

static int nDigits( int n )
{
    return (int) ceil( log10( n ));
}

/** sscanf is dumb and only extracts the number but DOESN'T
 *  advance the string pointer for us. We want the pointer to advance.
 */
static char* getNumber( char* str, int* num )
{
    char* ptr = str;

    // First, skip the first non-whitespace char
    eatWhitespace( &ptr );

    // The next item MUST be a digit; extract it and advance the pointer
    int ret = sscanf( ptr, "%d", num );
    if( ret == 0 || ret == EOF ){
        return NULL;
    }

    ptr += nDigits( *num ) + 1; 

    // Finally, remove any possible trailing whitespace
    eatWhitespace( &ptr );

    return ptr;
}

int dist( int a, int b )
{
    int res = a - b;

    if( res < 0 )
        res = -res;

    return res;
}

int isGood( int lastNum, int nextNum, int increasing )
{
    if( lastNum == nextNum )
        return 0;

    if( nextNum < lastNum && increasing )
        return 0;

    if( nextNum > lastNum && !increasing )
        return 0;

    if( dist( lastNum, nextNum ) > 3 )
        return 0;

    return 1;
}

int isSafe( char* line )
{
    int lastNum;
    int nextNum;

    line = getNumber( line, &lastNum );
    assert( line );

    line = getNumber( line, &nextNum );
    assert( line );

    DEBUG( "Got %d\n", lastNum );
    DEBUG( "Got %d\n", nextNum );

    // Cond 1: Sequence must be STRICTLY increasing or decreasing
    int increasing = 0;

    if( lastNum == nextNum ){
        // Neither increasing nor decreasing. NOT Safe.
        DEBUG( "Aborting\n" );
        return 0;
    } else if( nextNum > lastNum ){
        increasing = 1;
    }
    if( !isGood( lastNum, nextNum, increasing )){
        DEBUG( "Aborting\n" );
        return 0;
    }

    // 1. Read each number
    // 2. Ensure it's strictly monotonically inc/decreasing
    // 3. Ensure it's within +/- [1,3] of last number
    lastNum = nextNum;
    line = getNumber( line, &nextNum );
    while( line ){
        VERBOSE( "Got %d\n", nextNum );

        if( !isGood( lastNum, nextNum, increasing )){
            DEBUG( "Aborting\n" );
            return 0;
        }

        lastNum = nextNum;
        line = getNumber( line, &nextNum );
    }
    DEBUG( "Safe\n" );

    return 1;
}


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
}

void SOLVER_ProcessLine( char* line )
{
    remove_newline( line );

    totNumSafe += isSafe( line );
}

void SOLVER_PrintSolution( void )
{
    printf( "Total safe: %d\n", totNumSafe );
}
