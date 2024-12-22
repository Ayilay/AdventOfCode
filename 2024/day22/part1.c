#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <pthread.h>
#include <semaphore.h>

#include "problem_solver.h"

#define ull unsigned long long

//--------------------------------------------------------------------------------
//  Problem-specific Stuff
//--------------------------------------------------------------------------------

// forward decls
extern int curNum;
ull mix( ull num, ull arg );
ull prune( ull num );

#define MAX_MONKEY  2100

int nInput = 0;
ull inputs[ MAX_MONKEY ];
ull secrets[ MAX_MONKEY ];

ull getInit( int monkey )
{
    assert( monkey < nInput );

    // monkey is unique for this thread, thus no race condition
    return inputs[ monkey ];
}

ull getSecret( ull monkey, ull input )
{
    assert( monkey < MAX_MONKEY );

    ull secret = input;

    for( int i = 0; i < 2000; i++ ){
        ull a = secret * 64;
        secret = mix( secret, a );
        secret = prune( secret );

        ull b = secret / 32;
        secret = mix( secret, b );
        secret = prune( secret );

        ull c = secret * 2048;
        secret = mix( secret, c );
        secret = prune( secret );
    }

    return secret;
}

__inline__
ull mix( ull num, ull arg )
{
    return num ^ arg;
}

__inline__
ull prune( ull num )
{
    return num % 16777216;
}


//--------------------------------------------------------------------------------
//  Thread Stuff
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
        int whichMonkey = getNumber();
        if( whichMonkey >= nInput )
            break;

        DEBUG( "Thread %d: Trying %d\n", arg->threadNum, whichMonkey );

        ull orig = getInit( whichMonkey );
        ull secret = getSecret( whichMonkey, orig );

        // whichMonkey is unique for this thread, thus no race condition
        secrets[ whichMonkey ] = secret;
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

    inputs[ nInput++ ] = atoll( line );
    assert( nInput < MAX_MONKEY );
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

    printf( "\nSOLUTIONS\n" );

    ull total = 0;
    for( int i = 0; i < nInput; i++ ){
        ull secret = secrets[i];
        total += secret;

        INFO( "Secret for monkey %d (orig %llu): %llu\n",
                i,
                inputs[i],
                secret
             );

    }
    printf("Total: %llu\n", total );
}
