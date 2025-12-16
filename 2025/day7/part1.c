#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE
//#define NDEBUG

#include "sc_queue.h"
#include "sc_map.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

typedef uint8_t u8;

// Each coordinate is small enough to join into a 64bit int
// Use this to create a "key" for storing them in a hashmap
typedef struct {
    union {
        struct pair{
            ui row;
            ui col;
        } c;
        ull key;
    };
} coord;

#define MAXSPLITTERS    2000
#define MAXRANGE        (MAXSPLITTERS * MAXSPLITTERS)

// Shortcuts
struct sc_map_64v allSplitters;
coord firstSource;

#define MAXDIM 200
int ndim = 0;

char grid[MAXDIM][MAXDIM];

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* n ){
    sc_map_init_64v( &allSplitters, MAXSPLITTERS, 0 );
}

void SOLVER_ProcessLine( char* line )
{
    static int curRow = 0;
    rmnewl( line );

    ndim = strlen( line );
    strcpy( grid[curRow], line );

    for( int c = 0; c < strlen(line); c++ ) {
        switch( line[c] ){
            case 'S':
                firstSource.c = (struct pair) {curRow, c};
                break;
            case '^':
                coord* splitter = malloc(sizeof(coord));
                splitter->c = (struct pair) {curRow, c};
                sc_map_put_64v( &allSplitters, splitter->key, splitter );

                assert( ! sc_map_oom( &allSplitters ) );

                break;
        }
    }

    curRow ++;
}

int calls_downstream = 0;

coord* FindDownstreamSplitter( coord* source )
{
    calls_downstream++;

    assert( source );

    int row = source->c.row;
    int col = source->c.col;

    for( ; row < ndim; row++ ){
        if( grid[row][col] == '^' ){
            coord key = (coord) { .c = (struct pair) {row, col} };
            coord* split = sc_map_get_64v( &allSplitters, key.key );
            assert( sc_map_found( &allSplitters ));

            VERBOSE( "Found split for source {%d,%d} at {%d,%d}\n",
                  source->c.row, source->c.col,
                  split->c.row, split->c.col
                  );

            return split;
        } else {
            // Draw the beam on the grid. This will help us quickly determine if
            // any arbitrary point is in a beam or not
            grid[row][col] = '|';
        }
    }

    return NULL;
}

int existing_hit = 0;
int existing_miss = 0;

int InExistingBeam( coord* point )
{
    int hit =  grid[point->c.row][point->c.col] == '|';

    // TEMP: Sabotage
    //hit = 0;

    if( hit )
        existing_hit ++;
    else
        existing_miss ++;

    return hit ;

}

void PrintSplitters() {
    coord* val;
    ull key;
    
    sc_map_foreach( &allSplitters, key, val ){
        printf( "Splitter @ {%d,%d}\n", val->c.row, val->c.col );
    }
}

void SOLVER_PrintSolution( void )
{

    // PrintSplitters();

    // For each source, follow it downstream until it splits.
    // Add the split point to a set.
    // Spawn two new sources, and add them to the list until all sources are resolved
    struct sc_queue_ptr sources;
    sc_queue_init( &sources );

    struct sc_map_64v splitPoints;
    sc_map_init_64v( &splitPoints, 0, 0 );

    sc_queue_add_last( &sources, &firstSource );
    while( ! sc_queue_empty( &sources ) ){
        coord* source = sc_queue_del_first( &sources );

        VERBOSE( "Try source at {%d,%d}\n", source->c.row, source->c.col );
        coord* split = FindDownstreamSplitter( source );
        if( split ){

            coord* s1 = malloc(sizeof(coord));
            coord* s2 = malloc(sizeof(coord));
            memcpy( s1, split, sizeof(coord));
            memcpy( s2, split, sizeof(coord));

            // Spawn two new sources at either side of the split
            s1->c.col --;
            s2->c.col ++;

            if( ! InExistingBeam( s1 ) ){
                sc_queue_add_last( &sources, s1 );
            } else {
                VERBOSE( "Found memo\n");
            }

            if( ! InExistingBeam( s2 ) )
                sc_queue_add_last( &sources, s2 );

            VERBOSE( "Workqueue size: %ld\n", sc_queue_size( &sources ));

            sc_map_put_64v( &splitPoints, split->key, split );
        }
    }

    for( int i = 0; i < ndim; i++ ){
        INFO( "%3d: %s\n", i, grid[i] );
    }

    //INFO( "Beam splits %d times\n", sc_map_size_64v( &splitPoints ) );

    // Stats
    printf( "existing hits: %d\n", existing_hit );
    printf( "existing miss: %d\n", existing_miss );
    printf( "Find Downstram calls: %d\n", calls_downstream );
}

