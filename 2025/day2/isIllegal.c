#define NVERBOSE
#include "problem_solver.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

/** Calculate how many digits in a (positive) number
 */
int ndigs( unsigned int n )
{
    if( n == 0 ){
        return 1;
    }

    return (int) floor( log10( n )) + 1;
}

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

// Index is number to check. Array is terminated by 0 always
int NumMultiplesTable[][10] = {

  /* 0  */  { 0 },
  /* 1  */  { 0 },
  /* 2  */  { 1, 0 },
  /* 3  */  { 1, 0 },
  /* 4  */  { 1, 2, 0 },
  /* 5  */  { 1, 0 },
  /* 6  */  { 1, 2, 3, 0 },
  /* 7  */  { 1, 0 },
  /* 8  */  { 1, 2, 4, 0 },
  /* 9  */  { 1, 3, 0 },
  /* 10 */  { 1, 2, 5, 0 },
  /*    */  { 1, 0 },
  /*    */  { 1, 2, 3, 4, 6, 0 },
  /*    */  { 1, 0 },
  /*    */  { 1, 2, 7, 0 },
  /* 15 */  { 1, 3, 5, 0 },
  /*    */  { 1, 2, 4, 8, 0 },
  /*    */  { 1, 0 },
  /*    */  { 1, 2, 3, 6, 9, 0 },
  /*    */  { 1, 0 },
  /* 20 */  { 1, 2, 4, 5, 10, 0 },
};

int digit(ull num, int which)
{
    ull mask = (ull) pow( 10, which );

    num /= mask;
    return num % 10;
}

/*
 * Is this sequence illegal, assuming a repeating sequence length of lenSeq
 *
 * num MUST be a multiple of lenSeq
 */
int isIllegalN(ull num, int seqlen)
{
    int digs = ndigs(num);
    int nseq = digs / seqlen;

    // Lookup table failure
    assert( nseq > 1 );

    // For each potential repeating subsequence
    for( int i = 0; i < nseq; i++){
        // For each digit in the subsequence
        for( int j = 0; j < seqlen; j++ ){
            int which = i*seqlen + j;
            if( digit(num,which) != digit(num,j) )
                return 0;
        }
    }

    return 1;
}

// For 1-to-N digit repeating sequences
int isIllegal2(ull num)
{
    int digs = ndigs(num);

    int* multEntry = NumMultiplesTable[digs];
    while( *multEntry != 0 ){
        VERBOSE( "trying multiple %d for sequence %lld\n", *multEntry, num );
        if( isIllegalN( num, *multEntry )){
            return 1;
        }
        multEntry ++;
    }

    return 0;
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

ull nextIllegal2(ull begin, ull end) {
    while( begin <= end ){
        if( isIllegal2(begin) )
            return begin;

        begin++;
    }

    // No illegal numbers found in range [begin,end]
    return 0;
}
