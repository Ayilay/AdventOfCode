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

#define MAX 64

int NROW = 0;
char grid[MAX][MAX] = {0};

void PrintGrid( char grid[MAX][MAX], int nrow )
{
    for( int i = 0; i < nrow; i++ ){
        printf( "%s\n", grid[i] );
    }
}

typedef struct {
    int r;
    int c;
} Vec;

Vec robotPos;

#define WALL  '#'
#define SPACE '.'
#define BOX   'O'
#define ROBOT '@'

//--------------------------------------------------------------------------------
//  Instruction Parse and Movement
//--------------------------------------------------------------------------------

// Shift-Box Optimization: Copy adjacent block (block OR robot) into space,
// and replace robot with space

void TryLeft()
{
    int row = robotPos.r;
    for( int c = robotPos.c; c > 0; c-- ){
        char tile = grid[row][c];
        if( tile == WALL ) return;

        if( tile == SPACE ){
            grid[row][c] = grid[row][c+1];

            grid[row][robotPos.c-1] = ROBOT;
            grid[row][robotPos.c] = SPACE;
            robotPos.c --;
            return;
        }
    }
}

void TryRight()
{
    int row = robotPos.r;
    for( int c = robotPos.c; c < MAX; c++ ){
        char tile = grid[row][c];
        if( tile == WALL ) return;

        if( tile == SPACE ){
            grid[row][c] = grid[row][c-1];

            grid[row][robotPos.c+1] = ROBOT;
            grid[row][robotPos.c] = SPACE;
            robotPos.c ++;
            return;
        }
    }
}

void TryUp()
{
    int col = robotPos.c;
    for( int r = robotPos.r; r > 0; r-- ){
        char tile = grid[r][col];
        if( tile == WALL ) return;

        if( tile == SPACE ){
            grid[r][col] = grid[r+1][col];

            grid[robotPos.r-1][col] = ROBOT;
            grid[robotPos.r][col] = SPACE;
            robotPos.r --;
            return;
        }
    }
}

void TryDown()
{
    int col = robotPos.c;
    for( int r = robotPos.r; r < MAX; r++ ){
        char tile = grid[r][col];
        if( tile == WALL ) return;

        if( tile == SPACE ){
            grid[r][col] = grid[r-1][col];

            grid[robotPos.r+1][col] = ROBOT;
            grid[robotPos.r][col] = SPACE;
            robotPos.r ++;
            return;
        }
    }
}

void TryMove( const char inst )
{
    //VERBOSE( "Moving '%c'\n", inst );
    switch( inst ){
        case '<':
            TryLeft();
            break;
        case '>':
            TryRight();
            break;
        case 'v':
            TryDown();
            break;
        case '^':
            TryUp();
            break;
        default:
            printf( "Unknown move '%c'\n", inst );
            abort();
    }

    //#ifdef VERBOSE 
    //PrintGrid( grid, NROW );
    //#endif
}

void ProcessInstructions( const char* line )
{
    const char* inst = line;
    while( *inst )
        TryMove( *inst++ );
}

//--------------------------------------------------------------------------------
//  Final Sol Calc
//--------------------------------------------------------------------------------

ull CalcGPS( char grid[MAX][MAX], int nrow )
{
    ull total = 0;
    for( int r = 0; r < nrow; r++ ){
        for( int c = 0; c < strlen(grid[r]); c++ ){
            if( grid[r][c] != BOX )
                continue;

            ull val = (r * 100) + c;
            total += val;
        }
    }

    return total;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );
    static int initialzed = 0;

    if( *line == '\0' )
        return;

    if( *line == WALL ){
        assert( strlen(line) < MAX );
        strcpy( grid[ NROW ], line );

        char* robot = strchr( line, ROBOT );
        if( robot ){
            robotPos.r = NROW;
            robotPos.c = (robot - line) / sizeof(char);
        }

        NROW++;
    } else {
        if( !initialzed ){
            //PrintGrid( grid, NROW );
            initialzed = 1;
        }
        ProcessInstructions( line );
    }

}

void SOLVER_PrintSolution( void )
{
    printf("\n");
    PrintGrid( grid, NROW );

    ull total = CalcGPS( grid, NROW );
    printf( "total:\n%lld\n", total );
}
