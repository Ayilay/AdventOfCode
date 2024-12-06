#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <assert.h>

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

const char* ACCEPT_DIGITS = "0123456789";

//#define DEBUG( ... )  printf( __VA_ARGS__ )
//#define INFO( ... )  printf( __VA_ARGS__ )

#define DEBUG( ... ) // NOTHING
#define INFO( ... ) // NOTHING

unsigned long long total = 0;

char* ptrDisable = 0;


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
}

void processMatch( char* line )
{
    char* ptr;
    int n1, n2;
    int ret;
    int nDigs;

    // quick sanity check
    assert( line[0] == 'm' );

    ptr = strpbrk( line, ACCEPT_DIGITS );
    if( ptr != &line[4] ){
        DEBUG( "Expected digit, got %c. False alarm\n", line[4] );
        return;
    }

    // Scan the 1st number. It must
    ret = sscanf( ptr, "%d", &n1 );
    assert( ret == 1 );

    // Advance the char pointer past the 1st number
    nDigs = ndigs( n1 );
    assert( nDigs <= 3);
    ptr += nDigs;

    DEBUG( "Found num %d with %d digits\n", n1, nDigs );

    // Following the number MUST be a comma
    if( *ptr != ',' ){
        DEBUG( "No comma after 1st number, found %c. Abort\n", *ptr );
        return;
    }
    ptr ++;

    // Next char after comma MUST be digit
    line = ptr;
    ptr = strpbrk( line, ACCEPT_DIGITS );
    if( ptr != line ){
        DEBUG( "No digit after comma, found %c. Abort\n", *ptr );
    }

    // Scan the 2nd number
    ret = sscanf( ptr, "%d", &n2 );
    assert( ret == 1 );

    // Advance the char pointer past the 2nd number
    nDigs = ndigs( n2 );
    assert( nDigs <= 3);
    ptr += nDigs;

    DEBUG( "Found num %d with %d digits\n", n2, nDigs );

    if( *ptr != ')' ){
        DEBUG("No closing ')', found %c. Abort\n", *ptr );
        return;
    }

    // All checks pass, we have VALID mul() instruction. Execute it
    // and add to global total.
    int result = n1 * n2;
    INFO( "Mul Result: %d", result );

    total += result;
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    char* ptr = line;

    ptr = strstr( ptr, "mul(" );
    while( ptr ){
        DEBUG( "Digesting: %s\n", ptr );
        processMatch( ptr );

        ptr++;
        ptr = strstr( ptr, "mul(" );
    }
    DEBUG( "Done\n");
}

void SOLVER_PrintSolution( void )
{
    printf( "TOTAL: %lld\n", total );
}
