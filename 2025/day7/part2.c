#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE

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

    ull upstreamcount;
} coord;

#define MAXSPLITTERS    2000
#define MAXRANGE        (MAXSPLITTERS * MAXSPLITTERS)

// Shortcuts
struct sc_map_64v allSplitters;
coord firstSource;

#define MAXDIM 200
int ndim = 0;

// The whole graphical grid
char grid[MAXDIM][MAXDIM];

// For part2, we sort the splitters by their row
coord* splittersSorted[ MAXSPLITTERS ];

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
                memset( splitter, 0, sizeof(coord) );

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

            //VERBOSE( "Found split for source {%d,%d} at {%d,%d}\n",
            //      source->c.row, source->c.col,
            //      split->c.row, split->c.col
            //      );

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

    if( hit )
        existing_hit ++;
    else
        existing_miss ++;

    return hit ;

}

int coordCompareByRow( const void* c1, const void* c2 )
{
    assert( c1 );
    assert( c2 );

    return (*(coord**)c1)->c.row - (*(coord**)c2)->c.row;
}

void SolveQuantum( struct sc_map_64v* splitters)
{
    const int ncoord = sc_map_size_64v( splitters );

    ull key;
    coord* val;

    int i = 0;
    sc_map_foreach( splitters, key, val ){
        splittersSorted[i] = val;

        i++;
    }

    qsort( splittersSorted,
           ncoord,
           sizeof( coord* ),
           coordCompareByRow );

    // The first (topmost) splitter has exactly one upstream source: The original source
    splittersSorted[0]->upstreamcount = 1;

    ull totalStreams = 0;

    for( int i = 0; i < ncoord; i++ ){
        coord* splitter = splittersSorted[i];

        VERBOSE( "Beams upstream of {%d,%d}: %lld\n",
           splitter->c.row, splitter->c.col,
           splitter->upstreamcount );

        // Find each downstream splitter, and add the current splitter's
        // upstream beams to the downstream splitter
        coord* src = malloc(sizeof(coord));
        memcpy( src, splitter, sizeof(coord));

        // Try left column first
        src->c.col --;
        coord* down = FindDownstreamSplitter( src );
        if( down ){
            down->upstreamcount += splitter->upstreamcount;
        } else {
            // No downstream nodes left; we ADD the number of streams to the total
            VERBOSE( "Exiting manifold @ {%d,%d} with %lld beams\n",
                     src->c.row, src->c.col,
                     splitter->upstreamcount );
            totalStreams += splitter->upstreamcount;
        }

        // Now try right column
        src->c.col += 2;
        down = FindDownstreamSplitter( src );
        if( down ){
            down->upstreamcount += splitter->upstreamcount;
        } else {
            // No downstream nodes left; we ADD the number of streams to the total
            VERBOSE( "Exiting manifold @ {%d,%d} with %lld beams\n",
                     src->c.row, src->c.col,
                     splitter->upstreamcount );
            totalStreams += splitter->upstreamcount;
        }
    }

    INFO( "Total Streams:\n%lld\n", totalStreams );
}

ull bytes_allocd = 0;

void SOLVER_PrintSolution( void )
{
    // For each source, follow it downstream until it splits.
    // Add the split point to a set.
    // Spawn two new sources, and add them to the list until all sources are resolved
    struct sc_queue_ptr sources;
    sc_queue_init( &sources );

    // We count all splitPoints (i.e. all splitters with an upstream beam)
    // for our solution
    struct sc_map_64v splitPoints;
    sc_map_init_64v( &splitPoints, 0, 0 );

    // Add a malloc'd copy of the first source so we can free it later
    coord* fs = malloc(sizeof(coord));
    memcpy( fs, &firstSource, sizeof(coord));
    sc_queue_add_last( &sources, fs );

    while( ! sc_queue_empty( &sources ) ){
        coord* source = sc_queue_del_first( &sources );

        coord* split = FindDownstreamSplitter( source );
        if( split ){

            coord* s1 = malloc(sizeof(coord));
            coord* s2 = malloc(sizeof(coord));
            memcpy( s1, split, sizeof(coord));
            memcpy( s2, split, sizeof(coord));

            bytes_allocd += 2*sizeof(coord);

            // Spawn two new sources at either side of the split
            s1->c.col --;
            s2->c.col ++;

            if( ! InExistingBeam( s1 ) ){
                sc_queue_add_last( &sources, s1 );
            } else {
                free(s1);
            }

            if( ! InExistingBeam( s2 ) ){
                sc_queue_add_last( &sources, s2 );
            } else {
                free(s2);
            }

            sc_map_put_64v( &splitPoints, split->key, split );
        }

        // Per statistics, we alloc total of ~9MB, yikes!
        free( source );
    }

    // Print Tree
    for( int i = 0; i < ndim; i++ ){
        INFO( "%3d: %s\n", i, grid[i] );
    }

    // Stats
    printf( "Stats from finding splitters:\n" );
    printf( "existing hits: %d\n", existing_hit );
    printf( "existing miss: %d\n", existing_miss );
    printf( "Find Downstram calls: %d\n", calls_downstream );
    printf( "Bytes allocd: %lld\n", bytes_allocd );

    /* We FOUND all splitPoints (splitters w/ upstream beam). From top
     * to bottom, we follow each splitter (twice) downstream, and add its own total
     * upstream beams to the downstream splitters. This should be O(n)
     *
     *   
     *    .......S.......
     *    .......1.......
     *    .......^.......
     *    ......1.1......
     *    ......^.^......
     *    .....1.2.1.....
     *    .....^.^.^.....
     *    ....1.3...1....
     *    ....^.^...^....
     *    ...1.4.331.1... Total: 13
     *          
     */
    SolveQuantum( &splitPoints );
}
