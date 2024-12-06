#ifndef ADVENT_SOLVER_H
#define ADVENT_SOLVER_H

#include <stddef.h>

void SOLVER_Init( void* );

void SOLVER_ProcessLine( char* );

void SOLVER_PrintSolution( void );

// Other utilities
void rmnewl( char* s );
int  ndigs( unsigned int n );

#endif
