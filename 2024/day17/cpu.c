#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NDEBUG

#include "cpu.h"
#include "problem_solver.h"

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

    printf( "Outputs:\n");
    for( int i = 0; i < ctx->nOut; i++ ){
        printf( "%d, ", ctx->output[i] );
    }
    printf("\n");
}



int COMBO( context* ctx, int arg )
{
    assert( ctx );
    assert( arg >= 0 );
    assert( arg != 7 );     // Reserved

    if( arg <= 3 )
        return arg;

    switch( arg ){
        case 4:     return ctx->A; break;
        case 5:     return ctx->B; break;
        case 6:     return ctx->C; break;

        default:
            printf( "unknown op: %d\n", arg );
            assert(0);
            break;
    }

}

void handle_DIV( context* ctx, int* dst, int arg )
{
    assert( ctx );
    assert( dst );

    // Numerator ALWAYS read from A
    int num = ctx->A;
    int exp = COMBO( ctx, arg );

    // Divide by 2^exp
    int res = num >> exp;
    DEBUG( "div %d / 2^%d = %d\n", num, exp, res );

    *dst = res;
}



//--------------------------------------------------------------------------------
//  OPCODE Handlers
//--------------------------------------------------------------------------------

void handle_adv( context* ctx, int arg )
{
    DEBUG( "divA, arg: %d\n", arg );
    handle_DIV( ctx, &ctx->A, arg );
}

void handle_bxl( context* ctx, int arg )
{
    int a = ctx->B;
    int b = arg;

    ctx->B = ctx->B ^ arg;
}

void handle_bst( context* ctx, int arg )
{
    ctx->B = COMBO( ctx, arg ) & 0b111;
}

void handle_jnz( context* ctx, int arg )
{
    // Main dispatch loop adds 2, so we subtract 2 here
    if( ctx->A )
        ctx->PC = arg - 2;
}

void handle_bxc( context* ctx, int arg )
{
    // OpCode consumes arg for legacy reasons, but is unused
    (void) arg;

    ctx->B = ctx->B ^ ctx->C;
}

void handle_out( context* ctx, int arg )
{
    ctx->output[ ctx->nOut++ ] = COMBO(ctx, arg) & 0b111;

    assert( ctx->nOut < OUTMAX );
}

void handle_bdv( context* ctx, int arg )
{
    assert( ctx );
    DEBUG( "divB, arg: %d\n", arg );
    handle_DIV( ctx, &ctx->B, arg );
}

void handle_cdv( context* ctx, int arg )
{
    assert( ctx );
    DEBUG( "divC, arg: %d\n", arg );
    handle_DIV( ctx, &ctx->C, arg );
}

void runProgram( context* cpu )
{
    long loopCount = 1000000;

    while( cpu->PC < cpu->nProgram )
    {
        //printContext( cpu );

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

        cpu->PC += 2;

        // Infinite loop check
        assert( --loopCount );
    }
}

