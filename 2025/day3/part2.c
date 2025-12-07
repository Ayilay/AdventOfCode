#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

//#define NVERBOSE
#define NDEBUG
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

ull sum = 0;

ull max( ull a, ull b ){
    return (a > b) ? a : b;
}


/* Find the FIRST occurence of the highest digit
 *
 * Return index into 'input' of first occurence of the highest digit
 */
int highestDigitPos( const char* input ) {
    int highestPos = 0;

    for( int i = 1; input[i] != '\0'; i++ ){
        if( input[i] > input[highestPos] )
            highestPos = i;
    }


    return highestPos;
}

/* Find the highest digit itself
 */
int highestDigit( const char* input ) {
    int highest = 0;

    for( int i = 0; input[i] != '\0'; i++ ){
        if( input[i] > highest )
            highest = input[i];
    }


    return highest - '0';
}

ull optimal( const char* str, int ndigits )
{
    int slen = strlen( str );
    VERBOSE( "Recurse: %s (%d): ", str, ndigits );

    // String is too small for requested number of digits
    if( slen < ndigits ) {
        VERBOSE( "str too small, ret 0\n" );
        return 0;
    } 

    // String is exactly the requested length; it is optimal
    if( slen == ndigits ) {
        ull result = atoll( str );
        VERBOSE( "str is exact, return %lld\n", result);
        return result;
    } 

    // String has 2 or more digits, but we're looking for exactly one:
    // Find and return the single highest digit in the substring
    if( ndigits == 1 ) {
        ull result = highestDigit( str );
        VERBOSE( "need only 1 highest digit, return %lld\n", result);

        return result;
    }

    // None of the boundary cases hit; proceed with recurse logic
    VERBOSE( "...\n" );

    ull number = str[0] - '0';
    number *= pow( 10, ndigits-1 );
    DEBUG( "first digit for %s is %lld\n", str, number );

    ull chosen    = number + optimal( str+1, ndigits-1);
    ull notchosen = optimal( str+1, ndigits );

    DEBUG( "Full String: %s\n", str );
    DEBUG( "Try Chosen: %lld + optimal( %s, %d)\n", number, str+1, ndigits-1 );
    DEBUG( "Try NotChosen: optimal( %s, %d)\n", str+1, ndigits );
    ull selected = max( chosen, notchosen );

    DEBUG( "with %s (%d) -> 1st chosen = %lld, notchosen = %lld. We select %s\n",
            str,
            ndigits,
            chosen,
            notchosen,
            selected == chosen ? "CHOSEN" : "NOT chosen"
           );

    return selected;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* unused )
{
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    int banklen = strlen(line);

    ull battery = optimal( line, 12 );

    sum += battery;

    INFO( "Optimal battery is %lld\n", battery );
}

void SOLVER_PrintSolution( void )
{
    printf( "%lld\n", sum );
}
