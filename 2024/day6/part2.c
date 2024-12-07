#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE
#define NDEBUG
#define NINFO
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define VISITED  'o'
#define OBSTACLE '#'

// These are printable characters BUT also "bitmasks"
// We OR these when visiting a square to indicate
// which directions we have traversed in
#define EMPTY    0x30       // 0
#define UP       0x31       // 1
#define DOWN     0x32       // 2
#define LEFT     0x34       // 4
#define RIGHT    0x38       // 8

#define UPMASK    0x01
#define DOWNMASK  0x02
#define LEFTMASK  0x04
#define RIGHTMASK 0x08

#define MAX 200

int totalLoops = 0;

int N = 0;
char map[MAX][MAX];

struct pos {
    int r;
    int c;
    char dir; // One of: <>^V

    int loopDiscovered;
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

    //strncpy( map[N], line, MAX );
    for( int i = 0; i < len; i++ )
    {
        if( line[i] == '.' ) {
            line[i] = EMPTY;
        }

        map[N][i] = line[i];
    }

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
        // Assume always facing up.
        char* pos = strchr( input[r], '^' );
        if( pos ){
            int c = pos - input[r];

            expl->r = r;
            expl->c = c;
            expl->dir = UP;
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
        expl->dir = RIGHT;
        moveExplorer( map, n, expl );
    } else {
        expl->r --;

        // We've already visited this spot AND going in the same direction
        // This is a LOOP
        if( map[ expl->r ][ expl->c ] & UPMASK ){
            DEBUG( "Loop found @ (%d, %d)\n", expl->r, expl->c );
            expl->loopDiscovered = 1;
            return;
        }

        map[ expl->r ][ expl->c ] |= UP;
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
        expl->dir = DOWN;
        moveExplorer( map, n, expl );
    } else {
        expl->c ++;

        // We've already visited this spot AND going in the same direction
        // This is a LOOP
        if( map[ expl->r ][ expl->c ] & RIGHTMASK ){
            DEBUG( "Loop found @ (%d, %d)\n", expl->r, expl->c );
            expl->loopDiscovered = 1;
            return;
        }

        map[ expl->r ][ expl->c ] |= RIGHT;
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
        expl->dir = LEFT;
        moveExplorer( map, n, expl );
    } else {
        expl->r ++;

        // We've already visited this spot AND going in the same direction
        // This is a LOOP
        if( map[ expl->r ][ expl->c ] & DOWNMASK ){
            DEBUG( "Loop found @ (%d, %d)\n", expl->r, expl->c );
            expl->loopDiscovered = 1;
            return;
        }
        map[ expl->r ][ expl->c ] |= DOWN;
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
        expl->dir = UP;
        moveExplorer( map, n, expl );
    } else {
        expl->c --;

        // We've already visited this spot AND going in the same direction
        // This is a LOOP
        if( map[ expl->r ][ expl->c ] & LEFTMASK ){
            DEBUG( "Loop found @ (%d, %d)\n", expl->r, expl->c );
            expl->loopDiscovered = 1;
            return;
        }

        map[ expl->r ][ expl->c ] |= LEFT;
        VERBOSE( "MOVED to (%d, %d)\n", expl->r, expl->c );
    }
}

void moveExplorer( char map[][MAX], int n, struct pos* expl )
{
    assert( expl );

    switch( expl->dir ){
        case UP:
            moveUp( map, n, expl );
            break;
        case DOWN:
            moveDown( map, n, expl );
            break;
        case LEFT:
            moveLeft( map, n, expl );
            break;
        case RIGHT:
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

void solverBody( const char orig_map[][MAX], int n, int r, int c )
{
    int deadloop = 10000;

    char item = orig_map[r][c];
    if( item == OBSTACLE || item == '^' ){
        // Cannot place obstruction if obstacle or explorer already there
        return;
    }

    char map[MAX][MAX];
    memcpy( map, orig_map, sizeof(map) );

    VERBOSE( "Placing obstruction @ (%d,%d)\n", r, c );
    map[r][c] = OBSTACLE;

    struct pos explorer;
    memset( &explorer, 0, sizeof( explorer ) );

    // Initial direction is always UP
    findExplorer( orig_map, N, &explorer );
    map[ explorer.r ][ explorer.c ] = UP;

    while( !explorer.loopDiscovered ){
        moveExplorer( map, n, &explorer );

        if( ! inBounds( map, N, &explorer )){
            DEBUG( "No-loop for obstacle @ (%d,%d)\n", r, c );
            return;
        }

        deadloop --;
        assert( deadloop > 0 );
    }

    INFO( "Found loop-causing obstacle @ (%d,%d)\n", r, c );
    totalLoops ++;
    //printMap( map, n );
}

void SOLVER_PrintSolution( void )
{
    for( int r = 0; r < N; r++ )
        for( int c = 0; c < N; c++ )
            solverBody( map, N, r, c );

    //solverBody( map, N, 6, 3 );

    //int nVisited = countVisited( map, N );
    printf( "num loops: %d\n", totalLoops );
}

