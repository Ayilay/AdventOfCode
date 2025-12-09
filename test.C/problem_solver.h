#ifndef ADVENT_SOLVER_H
#define ADVENT_SOLVER_H

#include <stddef.h>

// To disable these, define NVERBOSE before including this header file
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

typedef unsigned long long ull;

// The init is optional; most problems won't need it
void SOLVER_Init( void* );

void SOLVER_ProcessLine( char* );

void SOLVER_PrintSolution( void );

// Other utilities
void rmnewl( char* s );
int  ndigs( ull n );

#endif
