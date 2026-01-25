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

ull totalcost = 0;

// Part2 adds this to prize x+y coords
sll penalty = 10000000000000;

/*
 * Crude matrix representaiton of system of equations
 *
 * | Xa  Xb | |A|    | Px |
 * | Ya  Yb | |B|  = | Py |
 *
 * Where Xa,Ya are +x,+y coords from pushing button A
 * Where Xb,Yb are +x,+y coords from pushing button B
 * Where A and B are num times we push buttons A and B, respectively
 * Where Px,Py is the x,y coord of the prize
 *
 * Solve for unknowns A and B
 *
 */
typedef struct {
    sll r1[3];  // Row 1: [Xa Xb Px]
    sll r2[3];  // Row 2: [Ya Yb Py]
} System;

void gcdReduce( sll row[3] )
{
    sll common = gcd( row[0], gcd(row[1], row[2]) );

    // Reduction not possible
    if( common == 1 ) return;

    for( int i = 0; i < 3; i++ )
        row[i] /= common;
}

void printSys( System* sys )
{
    sll* row = sys->r1;
    for( int r = 0; r < 2; r++ ){
        VERBOSE( "[ %5lld %5lld | %8lld]\n",
                row[0], row[1], row[2] );

        // Hack: We exit after this iter so this is last assignment
        row = sys->r2;
    }
    VERBOSE("\n");
}

int anyNeg( System* sys )
{
    sll* row = sys->r1;
    for( int r = 0; r < 2; r++ ){
        for( int c = 0; c < 3; c++ ){
            if (row[c] < 0)
                return 1;
        }

        // Hack: We exit after this iter so this is last assignment
        row = sys->r2;
    }

    return 0;
}


void scaleRow( sll row[3], int a )
{
    for( int i = 0; i < 3; i++ )
        row[i] *= a;
}

void addRow( sll dst[3], const sll src[3] )
{
    for( int i = 0; i < 3; i++ )
        dst[i] += src[i];
}

/* Crude brute-force gaussian elimination
 */
int solve( System* sys )
{
    VERBOSE( "orig:\n" );
    printSys( sys );

    // Frequently we reduce rows to common multiple if possible,
    // to help numbers stay small and keep integer math 'stable'
    // In the end, we normalize the row-echelon form
    gcdReduce( sys->r1 );
    gcdReduce( sys->r2 );

    VERBOSE( "reduced:\n" );
    printSys( sys );

    // Pivot on row1
    sll common = lcm( sys->r1[0], sys->r2[0] );

    // Scale both rows until their pivot elements are equal
    sll div1 = common / sys->r1[0];
    sll div2 = common / sys->r2[0];
    VERBOSE( "lcm: %lld    div1: %lld, div2: %lld\n\n", common, div1, div2 );

    scaleRow( sys->r1, -div1 );
    scaleRow( sys->r2, div2 );
    VERBOSE( "Scaled to lcm:\n" );
    printSys( sys );

    // Eliminate with first pivot
    addRow( sys->r2, sys->r1 );
    // Fatal arithmetic failure if this is not true
    assert( sys->r2[0] == 0 );

    // Row coeffs MUST be positive for GCD
    if( sys->r2[1] < 0 ){
        scaleRow( sys->r2, -1 );
        if( sys->r2[2] < 0 ){
            VERBOSE( "No solution: Row 2 negative sol:\n" );
            printSys( sys );
            return 0;
        }
    }

    // Undo negate of first row, and reduce rows for math stability
    scaleRow( sys->r1, -1 );
    gcdReduce( sys->r1 );
    gcdReduce( sys->r2 );

    // Prize coord is not integer multiple of button push. No solution
    if( sys->r2[1] != 1 ){
        VERBOSE( "No solution: Row 2 cannot normalize:\n" );
        printSys( sys );
        return 0;
    }

    // Progress so far
    VERBOSE( "After row 1 pivot reduce:\n" );
    printSys( sys );

    ////////////////////////////////////////

    // Pivot on row2
    common = lcm( sys->r1[1], sys->r2[1] );

    // Scale both rows until their pivot elements are equal
    div1 = common / sys->r1[1];
    div2 = common / sys->r2[1];
    VERBOSE( "lcm: %lld    div1: %lld, div2: %lld\n\n", common, div1, div2 );

    scaleRow( sys->r1, div1 );
    scaleRow( sys->r2, -div2 );
    VERBOSE( "Scaled to lcm:\n" );
    printSys( sys );

    // Eliminate with second pivot
    addRow( sys->r1, sys->r2 );
    // Fatal arithmetic failure if this is not true
    assert( sys->r1[1] == 0 );

    // Progress so far
    VERBOSE( "After both row pivot reduce:\n" );
    printSys( sys );

    // Row coeffs MUST be positive for GCD
    if( sys->r1[0] < 0 ){
        scaleRow( sys->r1, -1 );
        if( sys->r2[2] < 0 ){
            VERBOSE( "No solution: Row 1 negative sol:\n" );
            printSys( sys );
            return 0;
        }
    }


    // Undo negate of second row, and reduce rows for math stability
    scaleRow( sys->r2, -1 );
    gcdReduce( sys->r1 );
    gcdReduce( sys->r2 );

    // Prize coord is not integer multiple of button push. No solution
    if( sys->r1[0] != 1 ){
        VERBOSE( "No solution: Row 1 cannot normalize:\n" );
        printSys( sys );
        return 0;
    }

    // RECHECK this
    if( anyNeg(sys) ) {
        VERBOSE( "No solution: some are negative\n" );
        printSys( sys );
        return 0;
    }

    VERBOSE( "Final solution:\n" );
    printSys( sys );
    return 1;
}

ull getCost( sll A, sll B )
{
    assert( A >= 0 );
    assert( B >= 0 );

    return (3*A) + B;
}


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    static System sys = {0};

    int fullyParsed = 0;
    sll *x, *y;
    char* ptr;
    char* endptr;
    if( strchr( line, 'A' ) ){
        x = &(sys.r1[0]);
        y = &(sys.r2[0]);
    }
    else if( strchr( line, 'B' ) ){
        x = &(sys.r1[1]);
        y = &(sys.r2[1]);
    }
    else if( strchr( line, 'P' ) ){
        x = &(sys.r1[2]);
        y = &(sys.r2[2]);

        fullyParsed = 1;
    }
    else {
        return;
    }

    ptr = strchr( line, 'X' );
    assert( ptr );
    ptr += 2;
    *x = strtoull( ptr, &endptr, 10 );
    assert( ptr != endptr );

    line = endptr;
    ptr = strchr( line, 'Y' );
    assert( ptr );
    ptr += 2;
    *y = strtoull( ptr, &endptr, 10 );
    assert( ptr != endptr );

    if( fullyParsed ){
        // Part2: We add penalty to prize coords
        *x += penalty;
        *y += penalty;

        int hasSol = solve( &sys );
        if( hasSol ){
            static int whichProblem = 0;

            ull cost = getCost( sys.r1[2], sys.r2[2] );
            totalcost += cost;


            INFO( "Sol for game %d: A=%lld, B=%lld. Cost: %lld\n",
                    whichProblem,
                    sys.r1[2],
                    sys.r2[2],
                    cost);

            whichProblem ++;
        }
    }
}

void SOLVER_PrintSolution( void )
{
    printf( "Total cost: %lld\n", totalcost );
}
