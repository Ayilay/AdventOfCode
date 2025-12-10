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

typedef struct {
    ull row;
    ull col;
} coord;

#define MAX 2000

int nred = 0;
coord tiles[MAX];


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    // Row-Col order doesn't matter for this problem
    ull r = atoll( line );
    line = strchr( line, ',' );
    line++;

    ull c = atoll( line );

    tiles[ nred ] = (coord) { r, c };
    nred ++;
}

ull area( coord* t1, coord* t2 )
{
    assert( t1 );
    assert( t2 );

    ull dr = llabs( t1->row - t2->row) + 1;
    ull dc = llabs( t1->col - t2->col) + 1;

    return dr * dc;
}

void SOLVER_PrintSolution( void )
{
    ull max = 0;

    for( int i = 0; i < nred; i++ ){
        for( int j = i+1; j < nred; j++ ){
            coord* t1 = &tiles[i];
            coord* t2 = &tiles[j];

            ull a = area( t1, t2 );

            printf( "[%3lld,%3lld] x [%3lld,%3lld] --> %3lld\n", 
                    t1->row, t1->col,
                    t2->row, t2->col,
                    a
                  );

            if ( a > max ){
                max = a;
            }
        }
    }

    printf( "max: \n%lld\n", max );
}
