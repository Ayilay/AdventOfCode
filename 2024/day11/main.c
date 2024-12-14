#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#include "problem_solver.h"

const char* filename = "input.txt";

char lineBuf[ BUFSIZ ];

void unit_tests()
{
    assert( ndigs(0) == 1 );
    assert( ndigs(1) == 1 );
    assert( ndigs(9) == 1 );
    assert( ndigs(10) == 2 );
    assert( ndigs(99) == 2 );
    assert( ndigs(100) == 3 );
    assert( ndigs(101) == 3 );

    assert( ndigs(10000000000000000000UL) == 20 );
}

int main( int argc, char* argv[] )
{
    unit_tests();

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

/** Calculate how many digits in a (positive) number
 */
int ndigs( unsigned long long n )
{
    if( n == 0 ){
        return 1;
    }

    int result = 0;
    while( n > 0 ){
        result ++;
        n /= 10;
    }
    //return (int) floor( log10( n )) + 1;

    return result;
}

/** Calculate 10^exp for positive exponents
 */
unsigned long long ullpow10( int exp )
{
    assert( exp >= 0 );

    unsigned long long result = 1;
    while( exp-- ){
        result *= 10;
    }

    return result;
}

