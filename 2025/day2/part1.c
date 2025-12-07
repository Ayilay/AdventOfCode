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

// How many numbers on the dial
// 100 means numbers 0-99
#define CIRCUMFERENCE   100

int dialRotation = 50;
int numZeros = 0;

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void rotateDial( int dir, int amount )
{
    // GLOBAL vars:
    //  dialRotation

    char* header = "";

    // Do NOT modulo-wrap yet; We must count each zero-wraparound
    if( dir == 'L' ) {
        // Edge case: If orig dial position is zero, the under-rotate
        // logic will detect a false-positive zero cross. So we compensate
        if( dialRotation == 0 ){
            printf( "Compensate for FALSE-underflow\n" );
            numZeros --;
        }

        dialRotation -= amount;

    } else if( dir == 'R' ) {
        dialRotation += amount;
    } else {
        printf( "Unexpected direction %c, aborting", dir );
        abort();
    }

    // Part2: Detect zero-rollover and count it, then do the rollover
    while( dialRotation < 0 ){
        //header= "UNDER click";
        numZeros ++;
        dialRotation += CIRCUMFERENCE;
        printf( "Under-click, dialRot is %3d\n", dialRotation );
    }

    while( dialRotation > CIRCUMFERENCE ){
        //header = "OVER click";
        numZeros++  ;
        dialRotation %= CIRCUMFERENCE;
        printf( "Over-click, dialRot is %3d\n", dialRotation );
    }

    // Edge case where dialRotation == CIRCUMFERENCE:
    // We do one last modulo-wrap and check if exactly zero
    dialRotation %= CIRCUMFERENCE;
    if( dialRotation == 0 ) {
        header = "EXACT click";
        numZeros ++;
    }

    printf( "%20s Rotate dial %c by %3d. New position is %4d\n",
            header,
            dir,
            amount,
            dialRotation
          );
}

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    int dir;
    int amount;

    dir = line[0];
    amount = atoi( &line[1] );

    rotateDial( dir, amount );
}

void SOLVER_PrintSolution( void )
{
    printf( "Number of zeros:\n%d\n", numZeros );
}
