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


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

typedef struct {
    int heights[5];
} keylock;

void printCommon( keylock* item, char* what )
{
    printf( "%s of pin lengths: ", what );
    for( int i = 0; i < 5; i++) {
        printf( "%d,", item->heights[i] );
    }
    printf("\n");
}

void printLock( keylock* lock )
{
    printCommon( lock, "lock" );
}

void printKey( keylock* key )
{
    printCommon( key, "key" );
}



#define MAX 500

int nkeys = 0;
int nlocks = 0;
keylock keys[MAX];
keylock locks[MAX];

void processKey( char scanbuf[][6] )
{
    keylock* key = &keys[nkeys];

    for( int pin = 0; pin < 5; pin++ ){
        int height = 0;
        for( int i = 0; i < 5; i++ ){

            char c = scanbuf[4-i][pin];
            if( c == '.' )
                break;

            assert( c == '#' );
            height++;
        }

        key->heights[pin] = height;
    }

    //printKey( key );

    nkeys++;
}

void processLock( char scanbuf[][6] )
{
    keylock* lock = &locks[nlocks];

    for( int pin = 0; pin < 5; pin++ ){
        int height = 0;
        for( int i = 0; i < 5; i++ ){

            char c = scanbuf[i][pin];
            if( c == '.' )
                break;

            assert( c == '#' );
            height++;
        }

        lock->heights[pin] = height;
    }

    //printLock( lock );

    nlocks++;
}

void commit_scan( char scanbuf[][6], int isKey )
{
    if( isKey ){
        processKey( scanbuf );
    } else {
        processLock( scanbuf );
    }


}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    #define ALLHASH    "#####"
    #define ALLDOT     "....."

    enum {
        SCAN_BEGIN,
        SCAN_PROCESS,
        SKIP_NL,
    };

    // keep trailing null for printf convenience
    static char scanbuf[5][6] = {0};

    static int state = SCAN_BEGIN;
    static int nscan = 0;
    static int isKey = 0;

    switch( state ){
        case SCAN_BEGIN:
            if( 0 == strncmp( ALLHASH, line, 5 )){
                isKey = 0;
            }
            else if( 0 == strncmp( ALLDOT, line, 5 )){
                isKey = 1;
            }
            else {
                printf( "Invalid first line: %s\n", line );
                abort();
            }

            memset( scanbuf, 0, sizeof(scanbuf) );
            state = SCAN_PROCESS;
            nscan = 0;
            break;

        case SCAN_PROCESS:
            assert( strlen(line) == 5 );
            strncpy( scanbuf[nscan], line, 5 );

            nscan ++;
            if( nscan > 5 ){
                commit_scan( scanbuf, isKey );

                state = SKIP_NL;
            }
            break;

        case SKIP_NL:
            state = SCAN_BEGIN;
            break;

    }
}

int tryKeyLock( keylock* key, keylock* lock )
{
    assert(key);
    assert(lock);

    for( int i = 0; i < 5; i++ ){
        if( key->heights[i] + lock->heights[i] > 5 )
            return 0;
    }

    return 1;
}

void SOLVER_PrintSolution( void )
{
    keylock* lock = &locks[0];

    int totalfits = 0;

    for( int ilock = 0; ilock < nlocks; lock = &locks[++ilock])
    {
        keylock* key = &keys[0];
        for( int ikey = 0; ikey < nkeys; key = &keys[++ikey])
        {
            totalfits += tryKeyLock( key, lock );

        }
    }

    printf( "%d combos fit\n", totalfits );
}
