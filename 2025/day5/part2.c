#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "sc_list.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX 2000

typedef struct {
    ull begin;
    ull end;

    struct sc_list next;
} range;

struct sc_list all_ranges;

int isHealthy( ull id, range* list, int size )
{
    for( int i = 0; i < size; i++ ){
        if( id >= list[i].begin && id <= list[i].end ){
            return 1;
        }
    }

    return 0;
}

/**
 * \precondition: r1 and r2 MUST be intersecting
 */
void mergeIntersecting( range* r1, range* r2, range* out )
{
    /*
     * See test.c file for all edge cases
     */
    assert( r1 );
    assert( r2 );
    assert( out );

    ull bnew, enew;

    if( r1->begin <= r2->begin )
        bnew = r1->begin;
    else
        bnew = r2->begin;

    if( r1->end >= r2->end )
        enew = r1->end;
    else
        enew = r2->end;

    out->begin = bnew;
    out->end   = enew;
}

int isIntersecting( range* r1, range* r2 )
{
    /*
     * See test.c file for all edge cases
     */
    assert( r1 );
    assert( r2 );

    if( r1->begin <= r2->end && r1->end >= r2->begin )
        return true;

    if( r2->begin <= r1->end && r2->end >= r1->begin )
        return true;

    return false;
}

void parseElem( const char* line, range* out )
{
    char* eptr;
    ull begin = strtoll( line, &eptr, 10);
    assert( *eptr == '-' );

    line = eptr+1;
    ull end = strtoll( line, NULL, 10);

    out->begin = begin;
    out->end = end;
}

void processRange( const char* line )
{
    range* elem = malloc( sizeof( range ));
    sc_list_init( &elem->next );
    parseElem( line, elem );

    VERBOSE( "Parsed Range %lld - %lld\n", elem->begin, elem->end );

    struct sc_list *it;
    struct sc_list *tmp;
    range *itElem;

    // Now scan EVERY range we've seen so far, and merge it with the new range
    // if applicable
    sc_list_foreach_safe (&all_ranges, tmp, it) {
        itElem = sc_list_entry( it, range, next);

        VERBOSE( "Compare {%lld - %lld} to {%lld - %lld}\n",
                elem->begin, elem->end,
                itElem->begin, itElem->end
               );

        /* If two ranges intersect:
         *  - Merge the intersecting-range into the working range element
         *  - Remove the intersecting-range from the list
         */
        if( isIntersecting( elem, itElem )){
            mergeIntersecting( elem, itElem, elem );
            sc_list_del( &all_ranges, &itElem->next );
            DEBUG( "intersect; new range: %lld - %lld\n", elem->begin, elem->end );
        }
    }

    DEBUG( "Add new range: %lld - %lld\n", elem->begin, elem->end );
    sc_list_add_tail( &all_ranges, &elem->next );
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* unused )
{
    sc_list_init( &all_ranges );
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    static int doingRange = 1;

    if( strlen( line ) == 0 ){
        doingRange = 0;
        return;
    }

    if( doingRange )
        processRange( line );
    else {
        // we ignore the ID for part2
        return;
    }
}

void SOLVER_PrintSolution( void )
{
    ull sum = 0;

    struct sc_list *it;
    range *elem;

    INFO( "Final joined ranges:\n");
    sc_list_foreach (&all_ranges, it) {
        elem = sc_list_entry( it, range, next);

        INFO( "{%lld - %lld}\n", elem->begin, elem->end);

        sum += (elem->end - elem->begin + 1);
    }
    INFO( "Total:\n%lld\n", sum );
}
