#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#include "problem_solver.h"

const char* filename = "input.txt";

char lineBuf[ BUFSIZ ];


int main( int argc, char* argv[] )
{
    if( argc > 1 ){
        filename = argv[1];
    }

    SOLVER_Init();

    FILE* inFile;
    inFile = fopen( filename, "r" );

    if( inFile == NULL ){
        fprintf( stderr, "%s: file \"%s\" not found\n", argv[0], filename );
        exit(1);
    }

    char* line;
    line = fgets( lineBuf, BUFSIZ, inFile );
    while( line != NULL )
    {
        //printf( "%s", line );

        SOLVER_ProcessLine( line );


        line = fgets( lineBuf, BUFSIZ, inFile );
    }

    SOLVER_PrintSolution();

cleanup:
    fclose( inFile );
    return 0;
}

//------------------------------------------------------------------------------
//  Various Utilities
//------------------------------------------------------------------------------

/** Find the last newline of a string, and replace it with a null character.
 */
void rmnewl( char* s )
{
    char* ptr = strrchr( s, '\n' );

    if( ptr ) {
        *ptr = '\0';
    }
}

/** Calculate how many digits in a (positive) number
 */
int ndigs( ull n )
{
    if( n == 0 ){
        return 1;
    }

    int digs = 0;
    while( n > 0 ){
        digs ++;
        n /= 10;
    }

    return digs;
}

// This one is OPTIONAL
__attribute__((weak))
void SOLVER_Init( void )
{
}

