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

#define adv     0
#define bxl     1
#define bst     2
#define jnz     3
#define bxc     4
#define out     5
#define bdv     6
#define cdv     7

// Programs are very small
#define MAX     20

#define OUTMAX  200

typedef struct {
    int A;
    int B;
    int C;

    int PC;

    int nProgram;
    int program[ MAX ];

    int nOut;
    int output[ OUTMAX ];
} context;


//--------------------------------------------------------------------------------
//  OPCODE Handlers
//--------------------------------------------------------------------------------

void handle_adv( context* ctx, int arg )
{
    // TODO
}

void handle_bxl( context* ctx, int arg )
{
    // TODO
}

void handle_bst( context* ctx, int arg )
{
    // TODO
}

void handle_jnz( context* ctx, int arg )
{
    // TODO
}

void handle_bxc( context* ctx, int arg )
{
    // TODO
}

void handle_out( context* ctx, int arg )
{
    // TODO
}

void handle_bdv( context* ctx, int arg )
{
    // TODO
}

void handle_cdv( context* ctx, int arg )
{
    // TODO
}

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

void runProgram( context* cpu )
{
    while( cpu->PC < cpu->nProgram ){
        int op  = cpu->program[ cpu->PC ];
        int arg = cpu->program[ cpu->PC + 1 ];

        #define HANDLE( CTX, OP )        \
            case( OP ):                  \
                handle_##OP( CTX, arg ); \
                break;

        switch( op ){
            HANDLE( cpu, adv )
            HANDLE( cpu, bxl )
            HANDLE( cpu, bst )
            HANDLE( cpu, jnz )
            HANDLE( cpu, bxc )
            HANDLE( cpu, out )
            HANDLE( cpu, bdv )
            HANDLE( cpu, cdv )
        }
    }
}

void printContext( context* ctx )
{
    assert( ctx );

    printf( "A:  %d\n", ctx->A );
    printf( "B:  %d\n", ctx->B );
    printf( "C:  %d\n", ctx->C );
    printf( "PC: %d\n", ctx->PC );

    printf( "Program: ");
    for( int i = 0; i < ctx->nProgram; i++ ){
        printf( "%d, ", ctx->program[i] );
    }
    printf("\n");
}

void printSol( context* ctx )
{
    assert( ctx );
    for( int i = 0; i < ctx->nOut; i++ ){
        printf( "%d", ctx->output[i] );
    }
    printf("\n");
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

context cpu;

void SOLVER_Init( void* arg )
{
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

    printContext( &cpu );
    printSol( &cpu );
}
