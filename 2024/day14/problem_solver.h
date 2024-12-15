#ifndef ADVENT_SOLVER_H
#define ADVENT_SOLVER_H

#include <stddef.h>

#ifndef NVERBOSE
#define VERBOSE( ... )  printf( __VA_ARGS__ )
#else
#define VERBOSE( ... ) // NOTHING
#endif

#ifndef NDEBUG
#define DEBUG( ... )  printf( __VA_ARGS__ )
#else
#define DEBUG( ... ) // NOTHING
#endif

#ifndef NINFO
#define INFO( ... )  printf( __VA_ARGS__ )
#else
#define INFO( ... ) // NOTHING
#endif

void SOLVER_Init( void* );

void SOLVER_ProcessLine( char* );

void SOLVER_PrintSolution( void );

// Other utilities
void rmnewl( char* s );
int  ndigs( unsigned int n );

unsigned long long intpow10( int exp );

#endif
