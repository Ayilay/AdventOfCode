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

#define MAX     400

unsigned int total = 0;

int N = 0;
char table[ MAX ][ MAX ] = {0};

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
}

/** Helper function
 */
unsigned int searchLR_str( const char table[][ MAX ], int n, const char* needle )
{
    int matches = 0;

    const char* line;
    char* ptr;

    for( int row = 0; row < n; row++ )
    {
        DEBUG( "Row: %d\n", row );

        line = table[row];
        ptr = strstr( line, needle );

        DEBUG( "Line: %s\n", line );

        while( ptr ){
            DEBUG( "Match, index %d\n", (ptr-line) );
            ptr ++;
            matches ++;
            ptr = strstr( ptr, needle );
        }
    }

    INFO( "searchLR_str: Found %d matches\n", matches );
    return matches;
}


unsigned int searchLR( const char table[][ MAX ], int n )
{
    return searchLR_str( table, n, "XMAS" );
}

unsigned int searchRL( const char table[][ MAX ], int n )
{
    return searchLR_str( table, n, "SAMX" );
}


void transpose( const char in[][ MAX ], char out[][ MAX ], int n )
{
    for( int r = 0; r < n; r++ ){
        for( int c = 0; c < n; c++ ){
            out[r][c] = in[c][r];
        }
    }
}

void rotateCW( const char in[][ MAX ], char out[][ MAX ], int n )
{
    // Transforms matrix as follows:

    // A  B2 C3          A
    // B1 C2 D2     ==>  B1 B2
    // C1 D1 E           C1 C2 C3
    //                   D1 D2
    //                   E

    for( int r = 0; r < n; r++ ){
        for( int c = 0; c <= r; c++ ){
            out[r][c] = in[r-c][c];
        }
    }

    for( int r = 0; r < n-1; r++){
        for( int c = 0; c < n-r-1; c++ ){
            out[r+n][c] = in[n-c-1][c+r+1];
        }
    }
}

void rotateCCW( const char in[][ MAX ], char out[][ MAX ], int n )
{
    for( int r = 0; r < n; r++ ){
        VERBOSE( "r = %d\n", r );
        for( int c = 0; c <= r; c++ ){
            int ir,ic,or,oc;
            or = r;
            oc = c;
            ir = c;
            ic = c+(n-r)-1;

            out[or][oc] = in[ir][ic];
            VERBOSE( "c = %d\n", c );
            VERBOSE( "processing out[%d][%d] = in[%d][%d]\n", or, oc, ir, ic );
        }
    }

    for( int r = 0; r < n-1; r++){
        VERBOSE( "r = %d\n", r );
        for( int c = 0; c < n-r-1; c++ ){
            int ir,ic,or,oc;
            or = r+n;
            oc = c;
            ir = n-c-1;
            ic = n-c-r-2;

            out[or][oc] = in[ir][ic];
            VERBOSE( "c = %d\n", c );
            VERBOSE( "processing out[%d][%d] = in[%d][%d]\n", or, oc, ir, ic );
        }
    }
}
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

    // Horizantal + backwards
    total += searchLR( table, N );
    total += searchRL( table, N );

    // printTable( table, N );

    // Vertical + backwards
    memset( transformed, 0, sizeof( transformed ) );
    transpose( table, transformed, N );
    INFO( "Doing Vertical\n");
    printTable( transformed, N );
    total += searchLR( transformed, N );
    total += searchRL( transformed, N );

    // Diagonal TopLeft <-> DownRight + backwards
    int diagN = N + N - 1;
    memset( transformed, 0, sizeof( transformed ) );
    rotateCW( table, transformed, N );
    INFO( "Doing Diag TopLeft -> DownRight\n");
    printTable( transformed, diagN );
    total += searchLR( transformed, diagN );
    total += searchRL( transformed, diagN );

    // Diagonal TopRight <-> DownLeft + backwards
    memset( transformed, 0, sizeof( transformed ) );
    rotateCCW( table, transformed, N );
    INFO( "Doing Diag TopRight -> DownLeft\n");
    printTable( transformed, diagN );
    total += searchLR( transformed, diagN );
    total += searchRL( transformed, diagN );


    printf( "Matches: %u\n", total );
}
