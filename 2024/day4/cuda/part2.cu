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

__device__ 
char arrayGet( const char* array, const int n, const int r, const int c )
{
    return array[ (r*n) + c ];
}

__device__ 
void arraySet( char* array, const int n, const int r, const int c, char val )
{
    array[ (r*n) + c ] = val;
}

const char kernel_1[] = 
    "M.M"
    ".A."
    "S.S";

const char kernel_2[] = 
    "S.S"
    ".A."
    "M.M";

const char kernel_3[] = 
    "M.S"
    ".A."
    "M.S";

const char kernel_4[] = 
    "S.M"
    ".A."
    "S.M";


/** Note: input is 2D square matrix of dim 'n'
 */
__global__ void Xcompare(
                    const char* input,
                    const char* kernel,
                    char* output,
                    const int n
)
{
    // 2D Convolution goes from 1 to n-1
    int centerX = threadIdx.x + (blockIdx.x * blockDim.x) + 1;
    int centerY = threadIdx.y + (blockIdx.y * blockDim.y) + 1;

    // If center is NOT A then we DEFINITELY don't have a X-MAS
    int a  = 'A' - arrayGet( input, n, centerX, centerY );
    if( a ){
        return;
    }

    // Compare the 4 'corners' of the input to those of the kernel
    int i1 = arrayGet( input, n, centerX-1, centerY-1 );
    int i2 = arrayGet( input, n, centerX-1, centerY+1 );
    int i3 = arrayGet( input, n, centerX+1, centerY-1 );
    int i4 = arrayGet( input, n, centerX+1, centerY+1 );

    int k1 = arrayGet( kernel, 3, 0, 0 );
    int k2 = arrayGet( kernel, 3, 0, 2 );
    int k3 = arrayGet( kernel, 3, 2, 0 );
    int k4 = arrayGet( kernel, 3, 2, 2 );

    int m1 = k1 - i1;
    int m2 = k2 - i2;
    int s1 = k3 - i3;
    int s2 = k4 - i4;

    int result = m1 + m2 + s1 + s2;
    //printf( "Result from (%3d,%3d): %d\n", centerX, centerY, result );
    //printf( "Result from (%3d,%3d): compare %c to %c\n", centerX, centerY, k1, i1 );
    //printf( "Result from (%3d,%3d): compare %c to %c\n", centerX, centerY, k2, i2 );
    //printf( "Result from (%3d,%3d): compare %c to %c\n", centerX, centerY, k3, i3 );
    //printf( "Result from (%3d,%3d): compare %c to %c\n", centerX, centerY, k4, i4 );

    if( result == 0 )
        arraySet( output, n, centerX, centerY, result );
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

int countMatches( const char* table, int n )
{
    assert( n > 2 );

    int match = 0;

    for( int r = 1; r < n-1; r++ ){
        for( int c = 1; c < n-1; c++ ){ 
            if( table[ (r*n) + c ] == 0 ){
                match++;
            }
        }
    }

    return match;
}

void printTable2( const char* table, int n )
{
    for( int r = 0; r < n; r++ ){
        for( int c = 0; c < n; c++ ){ 
            int elem = table[ (r*n) + c ];
            printf( "%3d ", elem );
        }
        printf("\n");
    }
}

void SOLVER_PrintSolution( void )
{
    char transformed[ MAX ][ MAX ];
    memset( transformed, 0, sizeof( transformed ) );

    char outputs[N*N];

    char* d_inputs;
    char* d_outputs;
    char* d_kernel;
    cudaError_t err;

    err = cudaMalloc( &d_kernel, 10);
    assert( err == cudaSuccess );

    err = cudaMalloc( &d_inputs, N * N + 1 );
    assert( err == cudaSuccess );

    err = cudaMalloc( &d_outputs, N * N );
    cudaMemset( d_outputs, 1, N * N );
    assert( err == cudaSuccess );

    // Convert standard 2D array to linear array into device memory
    cudaMemset( d_inputs, 0, N*N+1 );
    for( int r = 0; r < N; r++){
        int row = r*N;
        cudaMemcpy( &d_inputs[row], &table[r][0], N, cudaMemcpyHostToDevice );
    }


    assert( N > 2 );
    dim3 problemDim( N-2, N-2 );

    #define LAUNCH( K ) \
        cudaMemcpy( d_kernel, kernel_##K, 9, cudaMemcpyHostToDevice );  \
        Xcompare<<< problemDim, 1 >>> (d_inputs, d_kernel, d_outputs, N );  \
        cudaDeviceSynchronize();

    LAUNCH( 1 )
    LAUNCH( 2 )
    LAUNCH( 3 )
    LAUNCH( 4 )


    cudaMemcpy( outputs, d_outputs, N*N, cudaMemcpyDeviceToHost );
    total = countMatches( outputs, N );

    printf( "ouitput: \n");
    printTable2( outputs, N );

    printf( "Matches: %u\n", total );

}
