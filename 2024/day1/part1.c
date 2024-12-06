#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX_LEN 2048
uint32_t list1[ MAX_LEN ];
uint32_t list2[ MAX_LEN ];

int N = 0;

/** Find the last newline of a string, and replace it with a null character.
 */
static void remove_newline( char* s )
{
    char* ptr = strrchr( s, '\n' );

    if( ptr ) {
        *ptr = '\0';
    }
}

int list_compar( const void* a, const void* b )
{
    assert( a );
    assert( b );

    if ( *((uint32_t*) a) < *((uint32_t*) b) ) {
        return -1;
    } 
    else if ( *((uint32_t*) a) == *((uint32_t*) b) ) {
        return 0;
    }
    else {
        return 1;
    }
}

void sortList( uint32_t* list, int nItems )
{
    qsort( list, nItems, sizeof(uint32_t), list_compar );
}

uint32_t getDistance( uint32_t* list1, uint32_t* list2, int n )
{
    uint32_t total = 0;

    for( int i = 0; i < n; i++ ){
        int d = list1[ i ] - list2[ i ];
        if( d < 0 ){
            d = -d;
        }

        total += d;
    }

    return total;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
}

void SOLVER_ProcessLine( char* line )
{
    remove_newline( line );
    uint32_t n1 = 0, n2 = 0;

    int res = sscanf(line, "%d %d", &n1, &n2);
    assert( res == 2 );

    list1[ N ] = n1;
    list2[ N ] = n2;
    N++;

    assert( N < MAX_LEN );
}

void SOLVER_PrintSolution( void )
{
    sortList( list1, N );
    sortList( list2, N );

    //printf("The lists (sorted):\n");
    //for( int i = 0; i < N; i++ ){
    //    printf( "%d %d\n", list1[i], list2[i]);
    //}

    uint32_t dist = getDistance( list1, list2, N );
    printf( "distance: %d\n", dist );
}
