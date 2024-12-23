#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <pthread.h>
#include <semaphore.h>

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

typedef struct {
    int              threadNum;
} threadArg;

#define NTHREADS    8
pthread_t threads[ NTHREADS ];

threadArg args[ NTHREADS ];

// Thread-safe "grab a ticket" mechanism
// Each thread calls this function and obtains a UNIQUE number
#define MAX_NUM    2000
sem_t numsem;
int curNum = 0;

int getNumber()
{
    int n;

    sem_wait( &numsem );
    n = curNum++;
    sem_post( &numsem );

    return n;
}

void* thread_solver(void* varg)
{
    assert( varg );
    threadArg* arg = (threadArg*) varg;

    printf( "Hello from Thread %d\n", arg->threadNum );

    while( 1 )
    {
        int num = getNumber();
        if( num >= MAX_NUM )
            break;
    }

    printf( "Thread %d done\n", arg->threadNum );
    return NULL;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_Init(void* arg)
{
    sem_init( &numsem, 0, 1 );
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );
}

void SOLVER_PrintSolution( void )
{
    threadArg* arg;

    for( int i = 0; i < NTHREADS; i++ )
    {
        arg = &args[ i ];
        memset( arg, 0, sizeof(*arg) );
        arg->threadNum = i;

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
