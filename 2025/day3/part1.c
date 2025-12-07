#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

ull sum = 0;


/* Find the FIRST occurence of the highest digit
 */
int highestDigit( const char* input, int limit ) {
    int highestPos = 0;

    VERBOSE( "Searching %s for highest w/ limit %d\n", input, limit );

    for( int i = 1; input[i] != '\0'; i++ ){
        if( input[i] > input[highestPos] && (input[i] - '0') <= limit )
            highestPos = i;
    }

    return highestPos;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    int banklen = strlen(line);

    int pos = highestDigit( line, 9 );
    int firstDigit = line[pos] - '0';

    // Edge Case: largest digit is at end of string
    // So locate the next-largest digit instead
    if( pos == banklen-1 ){
        int limit = firstDigit - 1;
        pos = highestDigit( line, limit );
        firstDigit = line[pos] - '0';
    }

    // Search for 2nd digit within string after the 1st digit
    // Guaranteed to have 1 or more digits in that position
    int secondPos = highestDigit( &line[pos] + 1, 9 );

    // Retval is relative to string beginning, so we add the offset to compensate
    secondPos += pos + 1;

    int secondDigit = line[secondPos] - '0';

    DEBUG( "%s: highest is str[%d] = %d, then str[%d] = %d\n",
            line,
            pos,
            firstDigit,
            secondPos,
            secondDigit 
            );

    int battery = (firstDigit * 10) + secondDigit;
    sum += battery;

    INFO( "%s: found battery %d\n", line, battery );
}

void SOLVER_PrintSolution( void )
{
    printf( "%lld\n", sum );
}
