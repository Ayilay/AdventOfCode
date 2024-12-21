#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "cpu.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  Parsing Functions
//--------------------------------------------------------------------------------

void parseReg( const char* input, int* output )
{
    assert( input );
    assert( output );

    char* ptr = strpbrk( input, "0123456789" );
    assert( ptr );

    int num = atoi( ptr );

    (*output) = num;
}

void parseProgram( context* ctx, const char* input )
{
    assert( input );
    assert( ctx );

    char* ptr = strchr( input, ',' );
    while( ptr ){
        // Extract the single-digit number before the comma
        int num = *(ptr - 1) - '0';
        ctx->program[ ctx->nProgram ] = num;
        ctx->nProgram ++;

        assert( ctx->nProgram < MAX );

        ptr++;
        ptr = strchr( ptr, ',' );
    }
}

void printSol( context* ctx )
{
    assert( ctx );

    printf( "Outputs:\n");
    for( int i = 0; i < ctx->nOut; i++ ){
        printf( "%d", ctx->output[i] );
        if( i < ctx->nOut-1)
            printf( ",");
    }
    printf("\n");
}
//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void unitTests()
{
    #define arraysize( A ) \
        sizeof( A ) / sizeof( A[0] )

    #define assertarray( act, nact, exp, nexp ) \
        assert( nact == nexp );                 \
        for( int i = 0; i < nexp; i++ )         \
            assert( act[i] == exp[i]);

    for( int i = 0; i < 64; i++ ){
        assert( (i & 0b111) == (i % 8) );
    }
}

context cpu;

void SOLVER_Init( void* arg )
{
    unitTests();

    memset( &cpu, 0, sizeof( cpu ) );
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    static char whichParse = 'A';

    if(whichParse == 'A'){
        parseReg( line, &(cpu.A) );
        whichParse = 'B';
    }
    else if(whichParse == 'B'){
        parseReg( line, &(cpu.B) );
        whichParse = 'C';
    }
    else if(whichParse == 'C'){
        parseReg( line, &(cpu.C) );
        whichParse = 'N';
    }
    else if(whichParse == 'N'){
        // nothing
        whichParse = 'P';
    }
    else if(whichParse == 'P'){
        parseProgram( &cpu, line );
        whichParse = 'D';
    }
    else{
        printf( "unexpected state: %c\n", whichParse );
        assert( 0 );
    }
}

void SOLVER_PrintSolution( void )
{
    runProgram( &cpu );

    //printContext( &cpu );
    printSol( &cpu );
}
