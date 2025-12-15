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

int totalMin = 0;

typedef struct {
    ui nbuttons;
    ui buttons[ 20 ]; // Each 'button' is bitmask of which switches are toggled

    ui nlights;
    ui joltTarg[ 20 ];
} machine;

// We only have ONE machine at a time
machine currentMachine;

typedef struct {
    // Most of the "state" data is static, stored in the machine struct

    // Number of lights == number of jolts
    ui jolts[20];
    ui sol;
} state;

// For memoization. We can't really do a map here, so we do array...
#define MAXSTATE 400000

int nstates = 0;
state states[ MAXSTATE ];

int StateComp( const state* a, const state* b )
{
    assert( a );
    assert( b );

    for( int i = 0; i < currentMachine.nlights; i++ ){
        if( a->jolts[i] != b->jolts[i] )
            return 0;
    }

    return 1;
}

const state* GetState( state* s )
{
    // Optimization: We are more likely to be looking for a state
    // added "more recently", i.e. closer to the end of the array
    // So we search backwards
    for( int i = nstates-1; i >= 0; i-- ){
        if( StateComp( s, &states[i] ))
            return &states[i];
    }

    return NULL;
}

void PutState( const state* s )
{
    memcpy( &states[ nstates ], s, sizeof(*s) );
    nstates ++;

    assert( nstates < MAXSTATE );
}

void ResetStates()
{
    memset( states, 0, sizeof(states) );

    // Hack: First state is all zero joltages
    nstates = 1;
}

int ButtonModifiesJoltage( int buttonIndex, int joltIndex )
{
    int button = currentMachine.buttons[ buttonIndex ];
    return !!( button & (1<<joltIndex) );
}

/* Recursively return the minimum number of button pushes
 * to reach the target joltage, starting from joltage state 'this'
 * and pushing button 'button'.
 *
 * Base cases:
 *  - Pushing this button is invalid (exceeds joltage target)
 *  - Pushing this button reaches joltage target (one button push)
 *
 * If pushing this button does not exceed joltage target, then
 * "push it", record new joltage state, and recursively push each
 * button until joltage target is reached.
 *
 * If each following button pushed is invalid following this button, then
 * it is impossible to reach the joltage target by pressing this button
 * from this state.
 */
int TryButton( const state* this, int button )
{
    state temp;
    memcpy( &temp, this, sizeof(temp) );

    // Will pushing this button exceed any joltage limits?
    // If so, it is invalid
    for( int i = 0; i < currentMachine.nlights; i++)
    {
        // Compare only joltages that this button touches
        if( ! ButtonModifiesJoltage(button, i) )
            continue;

        // Target alread reached, so pushing this button will exceed it
        if( this->jolts[i] == currentMachine.joltTarg[i] ){
            return -1;
        } else {
            temp.jolts[i]++;
        }
    }

    // Does a cached solution exist? If so, retrieve it
    const state* statePtr = GetState( &temp );
    if( statePtr != NULL ){
        return statePtr->sol;
    }

    // Did we reach the joltage target? If so, return "one button push"
    int reached = 1;
    for( int i = 0; i < currentMachine.nlights; i++) {
        if( temp.jolts[i] != currentMachine.joltTarg[i] ){
            reached = 0;
            break;
        }
    }
    if( reached ){
        // Save the solution; it is not cached, else we would have found it
        temp.sol = 1;
        PutState( &temp );
        return 1;
    }

    // To begin, assume each button pushed is invalid
    int pushes = -1;

    // Now, 'try' (recursively) every button, and find which one
    // gives the minimum button pushes
    for( int i = 0; i < currentMachine.nbuttons; i++) {
        int bmin = TryButton( &temp, i );
        if( bmin != -1 && (pushes == -1 || bmin < pushes) ){
            pushes = bmin;
        }
    }

    // Add one for pushing the current button
    if( pushes != -1 )
        pushes ++;

    // Save the solution (even if it is no-solution)
    temp.sol = pushes;
    PutState( &temp );

    return pushes;
}

// 
int SolveState( const state* this )
{
    int min = -1;
    for( int b = 0; b < currentMachine.nbuttons; b++ ){
        int bmin = TryButton( this, b );
        if( bmin != -1 && (min == -1 || bmin > min) ){
            min = bmin;
        }
    }

    return min;
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

int SolveMachine()
{
    int min = -1;

    ResetStates();

    min = SolveState( &states[0] );

    return min;
}

void PrintMachine()
{
    machine* m = &currentMachine;

    printf( "Machine: {" );
    for( int i = 0; i < m->nlights; i++ ){
        printf( "%d", m->joltTarg[i] );
        if( i < m->nlights-1 )
            printf( ", " );
    }
    printf( "}\tButtons: " );

    for( int i = 0; i < m->nbuttons; i++ ){
        printf( "%02x", m->buttons[i] );
        if( i < m->nbuttons-1 )
            printf( ", " );
    }

    printf( "\n");
}


void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    memset( &currentMachine, 0, sizeof(currentMachine) );

    // Hack: Replace the last } to parse more easily
    *(strchr( line, '}' )) = '\0';
    // Save this because strtok will destroy it
    char* joltagePtr = strchr( line, '{' );
    assert( joltagePtr );
    joltagePtr++;
    assert( *joltagePtr );

    char* saveptr_button;
    char* saveptr_bit;

    line++;
    char* tok = strtok_r( line, "]", &saveptr_button );
    int len = strlen(tok);
    currentMachine.nlights = len;
    
    // Parse each button
    char* bit;
    tok = strtok_r( NULL, "(", &saveptr_button);
    tok = strtok_r( NULL, ")", &saveptr_button);
    while( tok != NULL ){
        int nbut = currentMachine.nbuttons;

        // Parse each bit of the button
        bit = strtok_r( tok, ",", &saveptr_bit );
        while( bit != NULL ){

            currentMachine.buttons[ nbut ] |= 1 << atoi( bit );

            bit = strtok_r( NULL, ",", &saveptr_bit );
        }

        tok = strtok_r( NULL, "(", &saveptr_button);
        tok = strtok_r( NULL, ")", &saveptr_button);

        currentMachine.nbuttons ++;
    }

    int i = 0;
    tok = strtok( joltagePtr, "," );
    while( tok != NULL ){

        currentMachine.joltTarg[ i ] = atoi( tok );
        i++;
        tok = strtok( NULL, "," );
    }

    // Solve each machine after it is fully parsed
    static int machineIndex = 0;
    int min = SolveMachine( &currentMachine );
    INFO( "Machine %3d: %d button min\n", machineIndex, min );
    machineIndex++;

    totalMin += min;
}

void SOLVER_PrintSolution( void )
{
    printf( "totalMin: %d\n", totalMin);
}
