#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// https://github.com/tezc/sc
#include "sc_map.h"

#define NVERBOSE
#define NDEBUG
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

// Memoization cache to prevent needless dynamic-programming recursion
struct sc_map_s64  memo;

ull sum = 0;

ull max( ull a, ull b ){
    return (a > b) ? a : b;
}

const char* makeKey( const char* str, int ndigits ) {
    int totlen =  strlen(str) + 3;
    char* key = malloc( totlen );

    snprintf( key, totlen, "%02d%s", ndigits, str );
    VERBOSE( "Made key %s from %s and %d\n", key, str, ndigits );

    return key;
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

    // MEMO-IZATION: Check if we have a saved sol for this string+ndigits combo
    // If we do, use it and return it
    const char* key = makeKey( str, ndigits );
    ull value = sc_map_get_s64( &memo, key );
    if( sc_map_found( &memo ) ) {

        VERBOSE( "Found memoized value %lld for key %s\n", value, key );

        free( (void*) key );
        return value;
    } else {
        // key is STILL ALLOC'd here; we will NOT free it. Instead, we insert
        // the solution into the memo map
    }

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

    // MEMO-IZATION: Store the computed value into the memo cache
    sc_map_put_s64( &memo, key, selected );

    return selected;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* unused )
{
    sc_map_init_s64( &memo, 1024 * 8, 0 );
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
    
    printf("Memo map stats:\n");
    printf("memo.cap: %d\n", memo.cap);
    printf("memo.size: %d\n", memo.size);
    printf("memo.load_fac: %d\n", memo.load_fac);
    printf("memo.remap: %d\n", memo.remap);

    printf("memo.used: %d\n", memo.used);
    printf("memo.oom: %d\n", memo.oom);
    printf("memo.found: %d\n", memo.found);
}
