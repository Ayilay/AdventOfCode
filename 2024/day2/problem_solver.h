#ifndef ADVENT_SOLVER_H
#define ADVENT_SOLVER_H

#include <stddef.h>
#include "problem_solver.h"

void SOLVER_Init( void* );

void SOLVER_ProcessLine( char* );

void SOLVER_PrintSolution( void );

// Other utilities
void remove_newline( char* s );

#endif
