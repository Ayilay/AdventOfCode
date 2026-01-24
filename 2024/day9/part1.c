#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

typedef enum type_t {
    BLOCK = 0,
    BLANK = 1,
} Type;

#define DATA_BLANK ((DiskEntry) {.id=-1, .len=1})

typedef struct disk_entry_t {
    int16_t id;     // -1 means blank
    int     len;    // Redundant but convenient
} DiskEntry;

#define DISKCAP 200000
DiskEntry DISK[ DISKCAP ];
int wIndex = 0;

void processBlock( int blockID, char diskEntry )
{
    assert( isdigit(diskEntry ));
    int nBlock = diskEntry - '0';
    for( int i = 0; i < nBlock; i++ ){
        DISK[wIndex++] = (DiskEntry) {
            .id=blockID,
            .len=ndigs(blockID)};

        assert( wIndex < DISKCAP );
    }
}

void processBlank( char diskEntry )
{
    assert( isdigit(diskEntry ));

    int nBlank = diskEntry - '0'; 
    for( int i = 0; i < nBlank; i++ ){
        DISK[wIndex++] = DATA_BLANK;

        assert( wIndex < DISKCAP );
    }
}

void printDisk( DiskEntry* disk, int nBlocks )
{
    // Hope it's null-terminated
    for( int i = 0; i < nBlocks; i++ ){
        if( disk[i].id == -1 )
            printf( ".");
        else
            printf( "%c", '0' + (disk[i].id % 10) );
    }

    printf("\n");
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    static int type = BLOCK;
    static int blockID = 0;
    int blockSize = 1;

    while( *line ){
        if( type == BLOCK ){
            processBlock( blockID, *line );
        } else {
            processBlank( *line );
        }

        if( type == BLOCK ){
            type = BLANK;
            blockID ++;
        }
        else
            type = BLOCK;
        line++;
    }

}

int findBlank( DiskEntry* disk, int index )
{
    while( DISK[index].id != -1 )
        index++;

    return index;
}

int findData( DiskEntry* disk, int index )
{
    // Search BACKWARDS for data
    while( DISK[index].id == -1 && index > 0 )
        index--;

    return index;
}

ull checksum( DiskEntry* disk, int nBlocks )
{
    ull cksum = 0;
    for( int i = 0; i < nBlocks; i++ ){
        if( disk[i].id == -1 )
            continue;

        cksum += disk[i].id * i;
    }

    return cksum;
}

void SOLVER_PrintSolution( void )
{
    int blankIndex = findBlank( DISK, 0 );
    int dataIndex = findData( DISK, wIndex-1 );

    VERBOSE( "blankIndex begin = %d\n", blankIndex );
    VERBOSE( "dataIndex begin = %d\n", dataIndex );

    while( blankIndex < dataIndex ){
        VERBOSE( "move dataEntry %d @ pos %d to blank @ pos %d\n",
                DISK[dataIndex].id,
                dataIndex,
                blankIndex);
        DISK[blankIndex] = DISK[dataIndex];
        DISK[dataIndex] = DATA_BLANK;

        blankIndex = findBlank( DISK, blankIndex );
        dataIndex = findData( DISK, dataIndex );
    }

    printDisk( DISK, blankIndex );

    printf( "%lld\n", checksum( DISK, wIndex ));
}
