#include <string.h>

#include "problem_solver.h"

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

