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

    SOLVER_Init( NULL );

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


/** Find the last newline of a string, and replace it with a null character.
 */
void rmnewl( char* s )
{
    char* ptr = strrchr( s, '\n' );

    if( ptr ) {
        *ptr = '\0';
    }
}

void SOLVER_Init(void* n){

}

