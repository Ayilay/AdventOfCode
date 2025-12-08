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

#define MAX 2000

typedef struct {
    ull begin;
    ull end;
} range;

int totalHealthy = 0;
int N = 0;
range ranges[ MAX ];

int isHealthy( ull id, range* list, int size )
{
    for( int i = 0; i < size; i++ ){
        if( id >= list[i].begin && id <= list[i].end ){
            return 1;
        }
    }

    return 0;
}

void processID( const char* line, range* list, int size, int* total)
{
    ull id = atoll( line );
    if( isHealthy( id, list, size ) ){
        INFO( "Healthy ID %lld\n", id );
        (*total) ++;
    }
}

void processRange( const char* line, range* list, int* size)
{
    char* eptr;
    ull begin = strtoll( line, &eptr, 10);
    assert( *eptr == '-' );

    line = eptr+1;
    ull end = strtoll( line, NULL, 10);

    list[*size].begin = begin;
    list[*size].end = end;

    (*size) ++;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    static int doingRange = 1;

    if( strlen( line ) == 0 ){
        doingRange = 0;
        return;
    }

    if( doingRange )
        processRange( line, ranges, &N );
    else
        processID( line, ranges, N, &totalHealthy );

    N ++;
}

void SOLVER_PrintSolution( void )
{
    printf( "Total Healthy:\n%d\n", totalHealthy );
}
