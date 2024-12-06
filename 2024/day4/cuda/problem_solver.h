#ifndef ADVENT_SOLVER_H
#define ADVENT_SOLVER_H

#include <stddef.h>

#define VERBOSE( ... )  printf( __VA_ARGS__ )
//#define DEBUG( ... )  printf( __VA_ARGS__ )
#define INFO( ... )  printf( __VA_ARGS__ )

//#define VERBOSE( ... ) // NOTHING
#define DEBUG( ... ) // NOTHING
//#define INFO( ... ) // NOTHING

void SOLVER_Init( void* );

void SOLVER_ProcessLine( char* );

void SOLVER_PrintSolution( void );

// Other utilities
void rmnewl( char* s );
int  ndigs( unsigned int n );

#endif
