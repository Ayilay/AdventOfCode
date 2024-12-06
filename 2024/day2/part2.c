#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX_NUMS 20

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

int listIsSafe( int* list, int n )
{
    int lastNum;
    int nextNum;

    lastNum = list[0];
    nextNum = list[1];

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
    int i = 2;
    nextNum = list[i];
    while( i < n ){
        VERBOSE( "Got %d\n", nextNum );

        if( !isGood( lastNum, nextNum, increasing )){
            DEBUG( "Aborting\n" );
            return 0;
        }

        i++;
        lastNum = nextNum;
        nextNum = list[i];
    }
    DEBUG( "Safe\n" );

    return 1;
}

void printList( const int* list, int n )
{
    printf( "{ " );
    for( int i = 0; i < n; i++ ){
        printf( "%d%s", list[i], i+1==n ? "" : ", " );
    }
    printf( " }\n" );
}

void removeIndex( int* list, int n, unsigned int index )
{
    assert( index < n );

    // Shift each element in list one to the left, starting with index
    for( int i = index; i < n-1; i++ ){
        list[i] = list[i+1];
    }
}

int isAnyPermSafe( const int* list, int n )
{
    int perm[ MAX_NUMS ];

    assert( n <= MAX_NUMS );

    printf( "Orig list: ");
    printList( list, n );

    for( int i = 0; i < n; i++ ){
        memcpy( perm, list, sizeof(*list) * n );
        removeIndex( perm, n, i );

        // Note: new list is of len n-1

        // We just need ONE permutation to be safe to count it
        if( listIsSafe( perm, n-1 ) ){
            return 1;
        }
    }

    return 0;
}

int rowIsSafe( char* line )
{
    int list[ MAX_NUMS ];

    int num;
    int n = -1;
    while( line ) {
        line = getNumber( line, &num );

        list[ ++n ] = num;
        assert( n < MAX_NUMS );
    }

    return isAnyPermSafe( list, n );
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

    totNumSafe += rowIsSafe( line );
}

void SOLVER_PrintSolution( void )
{
    printf( "Total safe: %d\n", totNumSafe );
}
