#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAX 2000

int nlists = 0;         // How many total lists?
int nlistentries = 0;   // How many numbers in each list?
char* list[ MAX ][ MAX ];
char operator[ MAX ];   // Operator for each list

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    //rmnewl( line );

    char* tok;

    nlists = 0;         // Count this each time, it's easier...

    tok = strtok( line, ",\n" );
    while( tok != NULL ){

        assert( nlists < MAX );
        assert( nlistentries < MAX );

        if( NULL != strpbrk( tok, "*+" )){
            operator[ nlists ] = tok[0];
        } else {
            char* num = malloc( strlen(tok)+1 );
            strcpy( num, tok );

            list[nlists][nlistentries] = num;
        }

        nlists ++;
        tok = strtok( NULL, ",\n" );
    }

    nlistentries ++;
}

void printSoFar()
{
    for( int l = 0; l < nlists; l++ ){
        INFO( "List %d (%c): ", l, operator[l] );

        for( int i = 0; i < nlistentries; i++ ){
            char* num = list[l][i];

            INFO( "--%3s--", num);
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

    printSoFar();

    ull total = 0;
    for( int l = 0; l < nlists; l++ ){
        char op = operator[l];
        ull localtot = 0;

        char transpose_buf[10];
        memset( transpose_buf, 0, sizeof(transpose_buf) );

        for( int col = 0; col < strlen(list[l][0]); col++ ){
            for( int digit = 0; digit < nlistentries; digit++ ){
                transpose_buf[digit] = list[l][digit][col];
            }

            // This will be null terminated per the memset
            ull num = atoll( transpose_buf );
            VERBOSE( "List %3d col %d: num is %lld\n", l, col, num );

            if( col == 0 ) {
                localtot = num;
            } else {
                if( op == '*' ){
                    localtot *= num;
                } else {
                    localtot += num;
                }
            }
        }
        VERBOSE( "  localtot is %lld\n", localtot );
        VERBOSE( "\n" );
        total += localtot;

    }
    INFO( "Total %lld\n", total );
}
