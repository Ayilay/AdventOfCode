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

#define VISITED  'o'
#define OBSTACLE '#'

#define MAX 200

int N = 0;
char map[MAX][MAX];

struct pos {
    int r;
    int c;
    char dir; // One of: <>^V
};

// Func prototypes
void moveExplorer( char map[][MAX], int n, struct pos* expl );


void printMap( const char input[][MAX], int n )
{
    for( int i = 0; i < n; i++ ){
        printf( "%s\n", input[i] );
    }
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

    strncpy( map[N], line, MAX );

    N++;
    assert( N < MAX );

}


// If out of bounds, we are done
int inBounds( const char input[][MAX], int n, struct pos* expl )
{
    assert( expl );
    int r = expl->r;
    int c = expl->c;

    if( r < 0 || r >= n ) return 0;
    if( c < 0 || c >= n ) return 0;

    return 1;
}


void findExplorer( const char input[][MAX], int n, struct pos* expl )
{
    for( int r = 0; r < n; r++ ){
        char* pos = strpbrk( input[r], "<>^V" );
        if( pos ){
            int c = pos - input[r];
            VERBOSE( "FOUND at (%d, %d): %c\n", r, c, *pos);

            expl->r = r;
            expl->c = c;
            expl->dir = *pos;
            return;
        }
    }

    // Not found
    assert( 0 );
}

void moveUp( char map[][MAX], int n, struct pos* expl )
{
    // Walk off the map; do NOT mark
    if( expl->r == 0 ){
        expl->r --;
        return;
    }

    if( map[expl->r - 1][expl->c] == OBSTACLE ){
        VERBOSE( "OBSTACLE @ (%d, %d), set dir=RIGHT\n", expl->r-1, expl->c );
        expl->dir = '>';
        moveExplorer( map, n, expl );
    } else {
        expl->r --;
        map[ expl->r ][ expl->c ] = VISITED;
        VERBOSE( "MOVED to (%d, %d)\n", expl->r, expl->c );
    }
}

void moveRight( char map[][MAX], int n, struct pos* expl )
{
    // Walk off the map; do NOT mark
    if( expl->c == n-1 ){
        expl->c ++;
        return;
    }

    if( map[expl->r][expl->c+1] == OBSTACLE ){
        VERBOSE( "OBSTACLE @ (%d, %d), set dir=DOWN\n", expl->r, expl->c+1 );
        expl->dir = 'V';
        moveExplorer( map, n, expl );
    } else {
        expl->c ++;
        map[ expl->r ][ expl->c ] = VISITED;
        VERBOSE( "MOVED to (%d, %d)\n", expl->r, expl->c );
    }
}

void moveDown( char map[][MAX], int n, struct pos* expl )
{
    // Walk off the map; do NOT mark
    if( expl->r == n-1 ){
        expl->r ++;
        return;
    }

    if( map[expl->r+1][expl->c] == OBSTACLE ){
        VERBOSE( "OBSTACLE @ (%d, %d), set dir=LEFT\n", expl->r+1, expl->c );
        expl->dir = '<';
        moveExplorer( map, n, expl );
    } else {
        expl->r ++;
        map[ expl->r ][ expl->c ] = VISITED;
        VERBOSE( "MOVED to (%d, %d)\n", expl->r, expl->c );
    }
}

void moveLeft( char map[][MAX], int n, struct pos* expl )
{
    // Walk off the map; do NOT mark
    if( expl->c == 0 ){
        expl->c --;
        return;
    }

    if( map[expl->r][expl->c-1] == OBSTACLE ){
        VERBOSE( "OBSTACLE @ (%d, %d), set dir=UP\n", expl->r, expl->c-1 );
        expl->dir = '^';
        moveExplorer( map, n, expl );
    } else {
        expl->c --;
        map[ expl->r ][ expl->c ] = VISITED;
        VERBOSE( "MOVED to (%d, %d)\n", expl->r, expl->c );
    }
}

void moveExplorer( char map[][MAX], int n, struct pos* expl )
{
    assert( expl );

    switch( expl->dir ){
        case '^':
            moveUp( map, n, expl );
            break;
        case 'V':
            moveDown( map, n, expl );
            break;
        case '<':
            moveLeft( map, n, expl );
            break;
        case '>':
            moveRight( map, n, expl );
            break;

        default:
            printMap( map, n );
            // invalid
            assert( 0 );
            break;
    }
}

int countVisited( const char map[][MAX], int n )
{
    int total = 0;

    for( int r = 0; r < n; r++ ){
        for( int c = 0; c < n; c++ ){
            if( map[r][c] == VISITED ){
                total ++;
            }
        }
    }

    return total;
}

void SOLVER_PrintSolution( void )
{
    int deadloop = 10000;

    struct pos explorer;
    findExplorer( map, N, &explorer );

    map[ explorer.r ][ explorer.c ] = VISITED;

    while( inBounds( map, N, &explorer ))
    {
        moveExplorer( map, N, &explorer );

        deadloop --;
        assert( deadloop > 0 );
    }

    printMap( map, N );

    int nVisited = countVisited( map, N );
    printf( "nVisited: %d\n", nVisited );
}
