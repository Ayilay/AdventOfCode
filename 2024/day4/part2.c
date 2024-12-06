#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX     400

int N = 0;
char table[ MAX ][ MAX ] = {0};

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
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

void assert_bounds( int n, int r, int c )
{
    assert( r+1 < n );
    assert( c+1 < n );
    assert( r-1 >= 0 );
    assert( c-1 >= 0 );
}

void match1( const char input[][MAX], char result[][MAX], int n, int r, int c )
{
    assert_bounds( n, r, c );

    // X-MAS has 'A' at center always
    if( input[r][c] != 'A' )
        return;

    if( input[r+1][c+1] != 'M' )
        return;

    if( input[r-1][c+1] != 'M' )
        return;

    if( input[r+1][c-1] != 'S' )
        return;

    if( input[r-1][c-1] != 'S' )
        return;

    result[r][c] = 1;
}

void match2( const char input[][MAX], char result[][MAX], int n, int r, int c )
{
    assert_bounds( n, r, c );

    // X-MAS has 'A' at center always
    if( input[r][c] != 'A' )
        return;

    if( input[r+1][c-1] != 'M' )
        return;

    if( input[r-1][c-1] != 'M' )
        return;

    if( input[r+1][c+1] != 'S' )
        return;

    if( input[r-1][c+1] != 'S' )
        return;

    result[r][c] = 1;
}

void match3( const char input[][MAX], char result[][MAX], int n, int r, int c )
{
    assert_bounds( n, r, c );

    // X-MAS has 'A' at center always
    if( input[r][c] != 'A' )
        return;

    if( input[r+1][c+1] != 'M' )
        return;

    if( input[r+1][c-1] != 'M' )
        return;

    if( input[r-1][c+1] != 'S' )
        return;

    if( input[r-1][c-1] != 'S' )
        return;

    result[r][c] = 1;
}

void match4( const char input[][MAX], char result[][MAX], int n, int r, int c )
{
    assert_bounds( n, r, c );

    // X-MAS has 'A' at center always
    if( input[r][c] != 'A' )
        return;

    if( input[r-1][c+1] != 'M' )
        return;

    if( input[r-1][c-1] != 'M' )
        return;

    if( input[r+1][c+1] != 'S' )
        return;

    if( input[r+1][c-1] != 'S' )
        return;

    result[r][c] = 1;
}

int countMatches( const char input[][MAX], int n )
{
    int matches = 0;

    for( int r = 0; r < n; r++ ){
        for( int c = 0; c < n; c++ ){
            if( input[r][c] ){
                matches ++;
            }
        }
    }

    return matches;
}


void SOLVER_PrintSolution( void )
{
    char matches[ MAX ][ MAX ];
    memset( matches, 0, sizeof( matches ) );

    for( int r = 1; r < N-1; r++ ){
        for( int c = 1; c < N-1; c++ ){
            match1( table, matches, N, r, c );
            match2( table, matches, N, r, c );
            match3( table, matches, N, r, c );
            match4( table, matches, N, r, c );
        }
    }

    int total = countMatches( matches, N );

    printf( "Matches: %u\n", total );
}
