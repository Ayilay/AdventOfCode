#include "problem_solver.h"
#include <stdio.h>
#include <math.h>

int isIllegal(ull num)
{
    int digs = ndigs(num);
    // Odd number of digits is never illegal
    if( digs % 2 != 0 )
        return 0;
    
    ull halfpow = pow( 10, digs/2);

    ull lowhalf = num % halfpow;
    ull hihalf  = num / halfpow;

    //printf( "num %lld, halfpow %lld, lowhalf %lld, hihalf %lld\n", num, halfpow, lowhalf, hihalf);

    return lowhalf == hihalf;
}

/*  To find each illegal number, call function with
 *  begin and end params.
 *
 *  If non-zero returned, then increment the returned value
 *  and call again but set begin = retval+1.
 *
 *  Repeat until return val is zero (no more illegal nums)
 */
ull nextIllegal(ull begin, ull end) {
    while( begin <= end ){
        if( isIllegal(begin) )
            return begin;

        begin++;
    }

    // No illegal numbers found in range [begin,end]
    return 0;
}
