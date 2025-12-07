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

ull sum = 0;

void findSols( ull begin, ull end )
{
    ull next;

    next = nextIllegal( begin, end );
    while( next != 0 ) {
        VERBOSE( "Found illegal (%lld) in range [%lld, %lld]\n", next, begin, end  );
        sum += next;
        next = nextIllegal( next+1, end );
    }
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    ull begin, end;

    char* ptr = line;
    char* eptr = NULL;

    while( 1 ) {
        begin = strtoll( ptr, &eptr, 10 );
        assert( *eptr == '-' );
        ptr = eptr+1;

        end = strtoll( ptr, &eptr, 10 );
        assert( eptr && (*eptr == ',' || *eptr == '\0'));

        findSols( begin, end );

        // Edge case: last range ends with null terminator
        if( ! *eptr ){
            break;
        }
        ptr = eptr+1;

    }

    printf( "done\n");
}

void SOLVER_PrintSolution( void )
{
    printf( "Sum of all is:\n%lld\n", sum );
}
