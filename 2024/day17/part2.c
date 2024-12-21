#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <pthread.h>
#include <semaphore.h>

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

context orig_cpu;

typedef struct {
    int              threadNum;
    const context*   orig_cpu;
} threadArg;

#define NTHREADS    16
pthread_t threads[ NTHREADS ];

threadArg args[ NTHREADS ];

// All threads can get this number, and it increments each time
// Semaphore guarantees each thread get unique num
sem_t numsem;
int curNum = 0;

int solutionFound = 0;

int getNumber()
{
    int n;

    sem_wait( &numsem );
    n = curNum++;
    sem_post( &numsem );

    return n;
}

int isQuine( context* cpu )
{
    assert( cpu );

    if( cpu->nOut != cpu->nProgram )
        return 0;

    for( int i = 0; i < cpu->nOut; i++ ){
        if( cpu->output[i] != cpu->program[i] )
            return 0;
    }

    return 1;
}

void* thread_solver(void* varg)
{
    threadArg* arg = (threadArg*) varg;
    assert( arg );

    printf( "Hello from Thread %d\n", arg->threadNum );

    context cpu;

    while( ! solutionFound )
    {
        // Reset the CPU, and set RegA to a unique number
        memcpy( &cpu, arg->orig_cpu, sizeof( cpu ) );
        cpu.A = getNumber();

        printf( "trying %d\n", cpu.A );

        runProgram( &cpu );
        if( isQuine( &cpu ) ){
            printf( "Solution found for RegA = %d\n", cpu.A );
            solutionFound = 1;
        }
    }

    return NULL;
}

void SOLVER_Init( void* arg )
{
    memset( &orig_cpu, 0, sizeof( orig_cpu ) );
    sem_init( &numsem, 0, 1 );
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    static char whichParse = 'A';

    if(whichParse == 'A'){
        parseReg( line, &(orig_cpu.A) );
        whichParse = 'B';
    }
    else if(whichParse == 'B'){
        parseReg( line, &(orig_cpu.B) );
        whichParse = 'C';
    }
    else if(whichParse == 'C'){
        parseReg( line, &(orig_cpu.C) );
        whichParse = 'N';
    }
    else if(whichParse == 'N'){
        // nothing
        whichParse = 'P';
    }
    else if(whichParse == 'P'){
        parseProgram( &orig_cpu, line );
        whichParse = 'D';
    }
    else{
        printf( "unexpected state: %c\n", whichParse );
        assert( 0 );
    }
}

void SOLVER_PrintSolution( void )
{
    threadArg* arg;

    for( int i = 0; i < NTHREADS; i++ )
    {
        arg = &args[ i ];
        memset( arg, 0, sizeof(*arg) );
        arg->threadNum = i;
        arg->orig_cpu = &orig_cpu;

        pthread_create(
                &threads[ i ],
                NULL,
                thread_solver,
                (void*) arg
                );
    }

    for( int i = 0; i < NTHREADS; i++ ){
        pthread_join( 
                threads[ i ],
                NULL
                );
    }

}
