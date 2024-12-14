#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

typedef struct {
    uint64_t x;
    uint64_t y;
} xy;

typedef struct {
    xy btnA;
    xy btnB;
    xy prize;
} state;

#define MAX 200
int N = 0;
state theInputs[ MAX ] = {0};


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init( void* arg )
{
}
void printXY( xy *ptr )
{
    assert( ptr );
    printf( "%llu,%llu", ptr->x, ptr->y );
}

void printInput( state *input )
{
    assert( input );
    printf( "BtnA: " );
    printXY( &input->btnA );
    printf( "\n");
    printf( "BtnB: " );
    printXY( &input->btnB );
    printf( "\n");
    printf( "Prize: " );
    printXY( &input->prize );
    printf( "\n");
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    state* input = &theInputs[ N ];

    uint64_t *x, *y;
    char* ptr;
    char* endptr;
    if( strchr( line, 'A' ) ){
        x = &(input->btnA.x);
        y = &(input->btnA.y);
    }
    else if( strchr( line, 'B' ) ){
        x = &(input->btnB.x);
        y = &(input->btnB.y);
    }
    else if( strchr( line, 'P' ) ){
        x = &(input->prize.x);
        y = &(input->prize.y);
    }
    else {
        N++;
        printf( "parsed: \n");
        printInput( input );
        printf( "\n");

        return;
    }

    ptr = strchr( line, 'X' );
    assert( ptr );
    ptr += 2;
    *x = strtoull( ptr, &endptr, 10 );
    assert( ptr != endptr );

    line = endptr;
    ptr = strchr( line, 'Y' );
    assert( ptr );
    ptr += 2;
    *y = strtoull( ptr, &endptr, 10 );
    assert( ptr != endptr );

    //printf("Parsing Button A: %llu,%llu\n", x, y );
}

int linearSolve( state *ptr, uint64_t *a,uint64_t *b )
{
    // TODO
    return 0;
}

int numSols = 0;

void trySolution( state* ptr )
{
    int hasSol;
    uint64_t a, b;
    
    hasSol = linearSolve( ptr, &a, &b );
    if( hasSol ){
        printf( "solution found: a=%llu, b=%llu\n", a, b);
        numSols ++;
    }

}

void SOLVER_PrintSolution( void )
{
    for( int i = 0; i < N; i++ ){
        trySolution( &theInputs[i] );
    }

    printf("numPrizes: %d\n", numSols );
}
