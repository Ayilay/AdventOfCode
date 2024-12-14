#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NDEBUG
#define NVERBOSE
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX (1024*200)
#define NUM_BLINKS 25

int N = 0;
unsigned long long stones[ MAX ];

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
}

void processStone( unsigned long long stone, unsigned long long* buf, int* nBuf )
{
    /* - Process the stone according to the rules
     * - Insert into buf
     * - Modify the nBuf index appropriately
     */

    VERBOSE( "Processing stone %u\n", stone );
    if( stone == 0 ){
        VERBOSE( "Setting to 1\n");
        stone = 1;
        buf[ (*nBuf)++ ] = stone;
        return;
    }

    int digits = ndigs( stone );
    if( digits % 2 == 0 ){
        digits /= 2;
        unsigned long long divisor = ullpow10( digits );
        unsigned long long s1 = stone / divisor;
        unsigned long long s2 = stone % divisor;

        INFO( "Splitting stone %llu into %llu and %llu\n", stone, s1, s2 );
        buf[ (*nBuf)++ ] = s1;
        buf[ (*nBuf)++ ] = s2;
        return;
    }

    stone *= 2024;
    buf[ (*nBuf)++ ] = stone;
    VERBOSE( "Multiplying into stone %u\n", stone );
}

void printStones( unsigned long long* stones, int n )
{
    for( int i = 0; i < n; i++ ){
        printf("%llu ", stones[i]);
    }
    printf("\n");
}

void doBlink( unsigned long long* stones, int* n )
{
    // Buffer to hold stones during processing
    int nBuf = 0;
    unsigned long long buf[ MAX ];
    memset( buf, 0, sizeof(buf) );

    for( int i = 0; i < *n; i++ ){
        unsigned long long stone = stones[i];
        processStone( stone, buf, &nBuf );

        assert( nBuf < MAX );
    }

    // Copy temp buffer into main buffer
    memcpy( stones, buf, sizeof(unsigned long long)*nBuf );
    *n = nBuf;
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    char* endptr;

    int num = 0;
    do {
        errno = 0;
        num = strtoull( line, &endptr, 10 );
        assert( errno == 0 );

        stones[ N ] = num;
        N ++;
        assert( N < MAX );

        line = endptr;
        line++;
    } while( *endptr );
}

void SOLVER_PrintSolution( void )
{
    for( int i = 0; i < NUM_BLINKS; i++ ){
        doBlink( stones, &N );

        printf( "After Blink %2d: ", i );
        printStones( stones, N );
        printf("\n");
    }

    printf( "nStones: %u\n", N );
}
