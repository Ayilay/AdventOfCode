#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE
#define NDEBUG
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX 400

int N = 0;
char table[ MAX ][ MAX ] = {0};

unsigned int total = 0;

void printTable( const char table[][ MAX ], int n )
{
    printf( "{\n" );
    for( int r = 0; r < n; r++ ){
        printf("  ");
        for( int c = 0; c < n; c++ ){
            printf( "%c, ", table[r][c] );
        }
        printf( "\n" );
    }
    printf( "}\n" );

}

//--------------------------------------------------------------------------------
//  Kernels
//--------------------------------------------------------------------------------

__device__ char arrayGet( const char* array, const int n, const int r, const int c )
{
    return array[ (r*n) + c ];
}

__global__ void basicKernel()
{
    int x = threadIdx.x;
    int y = blockIdx.x;
    printf( "hi from (%d,%d)\n", x, y );

}

/** Note: input is 2D square matrix of dim 'n'
 */
__global__ void Xcompare_1( const char* input, char* output, const int n )
{
    int centerX = 1;
    int centerY = 1;
    // TODO obtain these from threadId

    // If center is NOT A then we DEFINITELY don't have a X-MAS
    int a  = 'A' - arrayGet( input, n, centerX, centerY );
    if( a ){
        return;
    }

    int m1 = 'M' - arrayGet( input, n, centerX-1, centerY-1 );
    int m2 = 'M' - arrayGet( input, n, centerX-1, centerY+1 );
    int s1 = 'S' - arrayGet( input, n, centerX+1, centerY-1 );
    int s2 = 'S' - arrayGet( input, n, centerX+1, centerY+1 );

    int result = m1 + m2 + s1 + s2;
    // TODO store result
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    int len = strlen( line );
    assert( len < MAX );

    strncpy( table[N], line, MAX );
    N++;
}


void SOLVER_PrintSolution( void )
{
    char transformed[ MAX ][ MAX ];
    memset( transformed, 0, sizeof( transformed ) );

    basicKernel<<< 1, 1 >>> ();

    printf( "Matches: %u\n", total );
}
