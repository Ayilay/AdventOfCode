#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAXCONSTR 40

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

int ContiguousNonBlank( const char* str )
{
    char* end = strchr( str, '.' );
    if( ! end ){
        return strlen(str);
    } else {
        return end - str;
    }
}

int ContiguousHash( const char* str )
{
    char* end = strpbrk( str, ".?" );
    if( ! end ){
        return strlen(str);
    } else {
        return end - str;
    }
}

ull CountArrangements( char* bluep, int* constraints, int nconstr )
{
    // Skip forwards until we find a non-blank character
    if( bluep[0] == '.' )
        bluep = strpbrk( bluep, "#?" );

    // Nothing left to parse
    if( bluep == NULL || bluep[0] == '\0' || nconstr == 0 )
        return 0;

    int constr;
    if( nconstr == 1 ){
        constr = constraints[0];

        // Blueprint is too SHORT, it is impossible to fit a spring
        if( strlen(bluep) < constr )
            return 0;

        int nonblank = ContiguousNonBlank( bluep );
        int nhash    = ContiguousHash( bluep );

        // Contiguous # and ? characters add up EXACTLY to constr
        // So exactly one arrangement
        if( nonblank == constr )
            return 1;

        // Not enough non-blank characters to fit constraint
        else if( nonblank < constr )
            return 0;

        // We might have multiple arrangements. Try them all
        // TODO
        else {
            if( nhash >= constr )
                // Too many fixed springs; can't satisfy constraint
                return 0;
            else if( nhash == constr )
                return 1;
            else {
                // TODO
            }
        }
    }

    return 0;
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    char* blueprint = strtok( line, " " );
    char* str_constr = strtok( NULL, " ");

    printf( "Bluept: %s - Constr: ", blueprint);

    int nconstr = 0;
    int constraints[ MAXCONSTR ];
    char* ptr = strtok( str_constr, "," );
    while( ptr ) {
        constraints[ nconstr ] = atoi( ptr );
        printf( "%d, ", constraints[nconstr] );

        nconstr ++;
        assert( nconstr < MAXCONSTR );
        ptr = strtok( NULL, "," );
    }
    printf( "\n");

    ull numOptions = CountArrangements( blueprint, constraints, nconstr );
}

void SOLVER_PrintSolution( void )
{
}
