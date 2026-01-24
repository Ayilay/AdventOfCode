#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE

#include "sc_list.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

typedef struct disk_entry_t {
    int16_t id;     // -1 means blank
    int32_t pos;    // Position in Disk
    int     size;   // Size in blocks

    struct sc_list next;
} DiskEntry;

struct sc_list files;
struct sc_list blanks;

// Disk Write Index
int wIndex = 0;

void processFile( int fileID, char diskEntry )
{
    assert( isdigit(diskEntry ));
    int nBlock = diskEntry - '0';

    DiskEntry* dent = calloc(1, sizeof(DiskEntry));

    dent->id   = fileID;
    dent->pos  = wIndex;
    dent->size = nBlock;
    sc_list_init( &dent->next );

    sc_list_add_tail( &files, &dent->next );

    wIndex += nBlock;
}

void processBlank( char diskEntry )
{
    assert( isdigit(diskEntry ));

    int nBlank = diskEntry - '0'; 

    DiskEntry* dent = calloc(1, sizeof(DiskEntry));

    dent->id   = -1;
    dent->pos  = wIndex;
    dent->size = nBlank;
    sc_list_init( &dent->next );

    sc_list_add_tail( &blanks, &dent->next );

    wIndex += nBlank;
}

void printList( struct sc_list* l )
{
    DiskEntry* dent;
    struct sc_list* iter;

    // See header file for iterator syntax
    sc_list_foreach( l, iter ){
        dent = sc_list_entry( iter, DiskEntry, next);
        assert(dent);

        if( dent->id == -1 ){
            printf( "Blank of size %d @ %d\n",
                    dent->size,
                    dent->pos );
        } else {
            printf( "File %3d of size %d @ %d\n",
                    dent->id,
                    dent->size,
                    dent->pos );
        }
    }
}

ull checksum(struct sc_list* files )
{
    ull cksum = 0;  

    struct sc_list* iter;
    sc_list_foreach( files,  iter ){
        DiskEntry* file = sc_list_entry( iter, DiskEntry, next );

        int size = file->size;
        for( int i = 0; i < file->size; i++ ){
            cksum += (file->pos+i) * file->id;
        }
    }

    return cksum;
}


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init(void)
{
    sc_list_init( &files );
    sc_list_init( &blanks );
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );
    
    #define BLOCK 0
    #define BLANK 1

    static int type = BLOCK;
    static int fileID = 0;
    int blockSize = 1;

    while( *line ){
        if( type == BLOCK ){
            processFile( fileID, *line );
        } else {
            processBlank( *line );
        }

        if( type == BLOCK ){
            type = BLANK;
            fileID ++;
        }
        else
            type = BLOCK;
        line++;
    }

}

/* Search from the beginning of 'blanks' list until we find
 * a blank large enough to fit the file, OR we surpass the location
 * of the file (we want to move the file backwards in the disk, not forwards)
 */
DiskEntry* findBlank( struct sc_list* blanks, DiskEntry* file )
{
    struct sc_list* iter;

    sc_list_foreach( blanks,  iter ){
        DiskEntry* nextBlank = sc_list_entry( iter, DiskEntry, next );

        if( nextBlank->pos > file->pos )
            return NULL;

        if( nextBlank->size >= file->size )
            return nextBlank;
    }

    return NULL;
}

void defrag( struct sc_list* files, struct sc_list* blanks )
{
    struct sc_list* tmp;
    struct sc_list* iter;

    sc_list_foreach_safe_r( files,  tmp, iter ){
        DiskEntry* file = sc_list_entry( iter, DiskEntry, next );

        DiskEntry* blank = findBlank( blanks, file );
        if( !blank ){
            // We didn't find a space large enough; leave it in its place
            VERBOSE( "Could not move file %d of size %d\n", file->id, file->size  );
            continue;
        }

        // If the blank is larger than the file, then SPLIT it:
        //  First half will be replaced by file
        //  Second half will go back into free list
        if( blank->size > file->size ){
            int newBlankPos = blank->pos + file->size;
            int newBlankSize = blank->size - file->size;

            DiskEntry* leftover = calloc( 1, sizeof( DiskEntry ));
            leftover->id = -1;
            leftover->pos = newBlankPos;
            leftover->size = newBlankSize;
            sc_list_init( &leftover->next );

            // Insert the "new blank" into the list
            sc_list_add_after( blanks, &blank->next, &leftover->next);
        }

        // Regardless whether we split the blank into two pieces or not, we
        // remove it from the list of free blocks. It is now "occupied"
        sc_list_del( blanks, &blank->next );

        int oldPos = file->pos;
        file->pos = blank->pos;
        VERBOSE( "Moved file %d from %d to %d\n", file->id, oldPos, file->pos );
    }
}

void SOLVER_PrintSolution( void )
{
    defrag( &files, &blanks );

    printf( "%lld\n", checksum( &files ));
}
