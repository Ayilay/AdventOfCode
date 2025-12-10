#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE
#define NDEBUG
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

int totalMin = 0;

typedef struct {
    ui nbuttons;
    ui solmask;
    ui buttons[ 20 ]; // Each 'button' is bitmask of which switches are toggled
} machine;

int nmachines = 0;
machine machines[ 300 ];

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

int SolveMachine( machine* mach )
{
    // Worst case is each button pushed once
    int min = mach->nbuttons;

    DEBUG( "Solving %#x\n", mach->solmask);

    // for-each combination of button pushes
    for( int i = 0; i < pow(2, mach->nbuttons); i++ ) {
        DEBUG( "  trying button-vector %#04x\n", i );
        // Bitmask of which buttons to try
        int mask = i;
        int out  = 0;

        int npushed = 0;
        int bit = 0;
        // for each button in the current combination
        while( mask != 0 )
        {
            if( mask & 1 ) {
                // Push this button. Increment how many buttons we've pushed
                int but = mach->buttons[ bit ];
                out = out ^ but;

                VERBOSE( "    apply %02x\n", but );

                npushed ++;
            }
            else {
                // Do not push this button
            }

            // Mask and bit change backwards wrt each other but it's fine
            // because we try all combinations so it doesn't matter
            mask >>= 1;
            bit ++;
        } // for each button in the current combination

        if( out == mach->solmask ){
            VERBOSE( "  Sol found!\n" );
            if( npushed < min ){
                min = npushed;
                DEBUG( "      New min for sol %#x: %d\n", mach->solmask, min);
            }
        }

    } // for-each combination of button pushes
    DEBUG( "   Final sol is %d\n", min );
    return min;
}


void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    char* saveptr_button;
    char* saveptr_bit;

    line++;
    char* tok = strtok_r( line, "]", &saveptr_button );
    int len = strlen(tok);

    int solmask = 0;
    for( int i = len-1; i >= 0; i--){
        solmask <<= 1;
        if( tok[i] == '#' )
            solmask |= 1;
    }

    
    // Now parse each button
    char* bit;
    tok = strtok_r( NULL, "(", &saveptr_button);
    tok = strtok_r( NULL, ")", &saveptr_button);
    while( tok != NULL ){

        int nbut = machines[ nmachines ].nbuttons;

        // Parse each bit of the button
        bit = strtok_r( tok, ",", &saveptr_bit );
        while( bit != NULL ){

            machines[ nmachines ].buttons[ nbut ] |= 1 << atoi( bit );

            bit = strtok_r( NULL, ",", &saveptr_bit );
        }

        //// Hack: Flip the bits of the button
        //int orig = machines[ nmachines ].buttons[ nbut ];
        //int flipped = 0;
        //while( orig ){
        //    flipped <<= 1;

        //    flipped |= orig & 1;
        //    orig >>= 1;
        //}
        //machines[ nmachines ].buttons[ nbut ] = flipped;

        tok = strtok_r( NULL, "(", &saveptr_button);
        tok = strtok_r( NULL, ")", &saveptr_button);

        machines[ nmachines ].nbuttons ++;
    }

    machines[ nmachines ].solmask = solmask;

    // Solve each machine after it is fully parsed
    int min = SolveMachine( &machines[nmachines] );
    totalMin += min;

    nmachines ++;
}

void PrintAllMachines()
{
    for( int m = 0; m < nmachines; m++ ){
        machine* mach = &machines[m];

        printf( "Machine %d: Sol %#x, Buttons ", m, mach->solmask );
        for( int b = 0; b < mach->nbuttons; b++ ){
            printf( "%#x ",  mach->buttons[b] );
        }
        printf( "\n" );
    }
}

void SOLVER_PrintSolution( void )
{
    printf( "Total Min:\n%d\n" , totalMin );
}
