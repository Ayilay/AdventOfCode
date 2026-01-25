#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAXDIM 51
#define MAXTOWER (MAXDIM*MAXDIM/4)


typedef struct dim_t {
    uint8_t row;
    uint8_t col;

    char val;
} Dim;

int nRows = 0;
int ntowers = 0;

Dim towers[MAXTOWER];
char grid[MAXDIM][MAXDIM];

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    memset( &grid[nRows], '.', strlen(line) );

    for( int i = 0; i < strlen(line); i++ ){
        if( line[i] == '.' ) continue;

        towers[ntowers++] = (Dim) { .row = nRows, .col = i, .val = line[i] };
        grid[nRows][i] = line[i];

        assert( ntowers < MAXTOWER );
    }

    nRows++;
}

inline int inbound( int coord )
{
    // Square Grid: nRows == nCols
    return coord >= 0 && coord < nRows;
}

void MarkAntinodes( Dim* a, Dim* b )
{
    INFO( "Calc antinodes %c from (%d,%d) and (%d,%d)\n",
            a->val,
            a->row, a->col,
            b->row, b->col );

    int drow = b->row - a->row;
    int dcol = b->col - a->col;
    VERBOSE( "  drow %d, dcol %d\n", drow, dcol );

    // We're looking for unique locations with antinodes, so it's ok to overwrite
    // grid location with latest-found antinode
    for( int i = a->row, j = a->col;
            inbound(i) && inbound(j);
            i += drow, j+=dcol )
    {
        VERBOSE( "  at (%2d,%2d)\n", i, j );
        grid[i][j] = a->val;
    }

    for( int i = a->row, j = a->col;
            inbound(i) && inbound(j);
            i -= drow, j-=dcol )
    {
        VERBOSE( "  at (%2d,%2d)\n", i, j );
        grid[i][j] = a->val;
    }
}

void render()
{
    for( int i = 0; i < nRows; i++ )
            printf( "%s\n", grid[i] );
}

int nunique()
{
    int count = 0;
    for( int i = 0; i < nRows; i++ )
        for( int j = 0; j < nRows; j++ )
            if( grid[i][j] != '.' )
                count++;

    return count;
}

void SOLVER_PrintSolution( void )
{
    for( int i = 0; i < ntowers; i++ )
    {
        for( int j = i+1; j < ntowers; j++ )
        {
            Dim* ti = &towers[i];
            Dim* tj = &towers[j];

            if( ti->val != tj->val ) continue;

            MarkAntinodes( ti, tj );
        }
    }

    render();
    printf("%d\n", nunique() );
}
