#include <stdint.h>
#include <string.h>

#include "problem_solver.h"

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

/** Count trailing zeros in binary number.
 *
 * e.g. ctz( 0b1000 ) returns 3
 */
int ctz(ull n)  {
    // Compiles with gcc on x86. Haven't tested others
    return __builtin_ctzll(n);
}

/** Greatest Common Divisor
 *  Binary Euclid's (aka Stein's) algorithm from wikipedia.
 *  
 *  Inputs MUST be positive
 */
ull gcd(ull u, ull v)  {
    if (u == 0)  return v;                 // Identity #1
    int k = ctz(u | v);                    // Identity #2
    u >>= k;  v >>= k;

    // d will replace u if d>0, and v if d<0.  Normally, this is
    // the difference (u-v)/2 and so the larger of u or v will be
    // replaced, but the first iteration, it is set to the even
    // one (if any).  Care is required to avoid signed overflow.
    int64_t d = (v % 2 ? (int64_t)(u/2) : 0)
              - (u % 2 ? (int64_t)(v/2) : 0);
    while (d != 0) {
        d >>= ctz(d);                      // Identity #3
        if (d > 0)  u = d;  else  v = -d;  // Identity #4
        // Since u and v are both odd, each loses a bit in the
        // division, which cancels out in the subtraction.
        d = (int64_t)(u/2) - (int64_t)(v/2);
    }
    return u << k;                         // Identity #2
}


/** Least Common Multiple
 *
 * Uses gcd above.
 * Inputs MUST be positive
 */
ull lcm(ull u, ull v)  {
    if( u == 0 && v == 0 )
        return 0;

    // Note: overflow might still occur if both u,v are huge
    // and gcd is very small
    // This division always yields integer
    return (u / gcd(u,v)) * v;
}
