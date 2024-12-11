#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

void expect_withline( int exp, int actual, int lineNo )
{
    if( exp != actual ){
        printf( "Assertion Fail @ %d: %d != %d\n",
                lineNo,
                exp,
                actual 
                );

        exit( 1 );
    }
}

#define expect( E, A )  \
    expect_withline( E, A, __LINE__ )


/** Essential a "get nth digit from base-3 number"
 */
int getOp( unsigned int opVec, int index )
{
    if( index > 0 )
        opVec /= (index * 3);

    opVec %= 3;

    return opVec;
}

unsigned long long intpow10( int exp )
{
    unsigned long long result = 1;
    while( exp-- ){
        result *= 10;
    }

    return result;
}

int unitTestMain()
{
    expect( 0, getOp( 0, 0 ) );
    expect( 0, getOp( 0, 1 ) );

    expect( 1, getOp( 1, 0 ) );
    expect( 0, getOp( 1, 1 ) );

    expect( 2, getOp( 2, 0 ) );
    expect( 0, getOp( 2, 1 ) );

    expect( 0, getOp( 3, 0 ) );
    expect( 1, getOp( 3, 1 ) );

    expect( 1, getOp( 4, 0 ) );
    expect( 1, getOp( 4, 1 ) );

    expect( 2, getOp( 5, 0 ) );
    expect( 1, getOp( 5, 1 ) );

    expect( 0, getOp( 6, 0 ) );
    expect( 2, getOp( 6, 1 ) );

    expect( 1, getOp( 7, 0 ) );
    expect( 2, getOp( 7, 1 ) );

    expect( 2, getOp( 8, 0 ) );
    expect( 2, getOp( 8, 1 ) );


}

/** Calculate how many digits in a (positive) number
 */
int ndigs( unsigned int n )
{
    if( n == 0 ){
        return 1;
    }

    return (int) floor( log10( n )) + 1;
}

void unitTestNDigs()
{
    expect( 1, ndigs(0) );
    expect( 1, ndigs(1) );
    expect( 1, ndigs(9) );
    expect( 2, ndigs(10) );
    expect( 2, ndigs(11) );
    expect( 2, ndigs(50) );
    expect( 2, ndigs(99) );

    expect( 3, ndigs(100) );
    expect( 3, ndigs(999) );

    expect( 6, ndigs(100000) );
    expect( 6, ndigs(999999) );

    expect( 2, ndigs(17) );
}

void unitTestPow()
{
    expect( 1,    intpow10(0) );
    expect( 10,   intpow10(1) );
    expect( 100,  intpow10(2) );
    expect( 1000, intpow10(3) );
}

int main()
{
    unitTestMain();

    unitTestNDigs();

    printf( "All tests pass\n");

    return 0;
}
