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

#define MAX 2000

int nlists = 0;         // How many total lists?
int nlistentries = 0;   // How many numbers in each list?
ull list[ MAX ][ MAX ];
char operator[ MAX ];   // Operator for each list

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    char* tok;

    nlists = 0;         // Count this each time, it's easier...

    tok = strtok( line, " " );
    while( tok != NULL ){

        assert( nlists < MAX );
        assert( nlistentries < MAX );

        if( NULL == strpbrk( tok, "0123456789" )){
            operator[ nlists ] = tok[0];
        } else {
            ull num = atoll( tok );

            list[nlists][nlistentries] = num;
        }

        nlists ++;
        tok = strtok( NULL, " " );
    }

    nlistentries ++;
}

void printSoFar()
{
    for( int l = 0; l < nlists; l++ ){
        INFO( "List %d (%c): ", l, operator[l] );

        for( int i = 0; i < nlistentries; i++ ){
            ull num = list[l][i];

            INFO( "%7lld", num);
            if( i != nlistentries-1 ){
                INFO( ", " );
            }
        }
        INFO( "\n" );
    }
}

void SOLVER_PrintSolution( void )
{
    // We counted the operator as a list entry during the parsing
    nlistentries --;

    //printSoFar();

    ull total = 0;
    for( int l = 0; l < nlists; l++ ){
        char op = operator[l];
        ull localtot = list[l][0];

        for( int i = 1; i < nlistentries; i++ ){
            ull num = list[l][i];
            if( op == '*' ){
                localtot *= num;
            }
            else if (op == '+') {
                localtot += num;
            } else {
                abort();
            }

            DEBUG( "Total for List %d: %lld\n", l, localtot );
        }

        total += localtot;
    }
    INFO( "Total %lld\n", total );
}
