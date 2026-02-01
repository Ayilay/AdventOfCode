#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NVERBOSE
#include "problem_solver.h"

/*
 *  Num Keypad:
 *  +---+---+---+
 *  | 7 | 8 | 9 |
 *  +---+---+---+
 *  | 4 | 5 | 6 |
 *  +---+---+---+
 *  | 1 | 2 | 3 |
 *  +---+---+---+
 *      | 0 | A |
 *      +---+---+
 *
 * Arrow Keypad:
 *     +---+---+
 *     | ^ | A |
 * +---+---+---+
 * | < | v | > |
 * +---+---+---+
 *
 */

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

typedef struct {
    int r;
    int c;
} Vec;

typedef struct {
    char button;
    Vec  coord;
} Key;

#define KEY_LIST_TERM   (Key) { 0 }

const Key numpadMap[] = {
    { '7', {0,0} },
    { '8', {0,1} },
    { '9', {0,2} },

    { '4', {1,0} },
    { '5', {1,1} },
    { '6', {1,2} },

    { '1', {2,0} },
    { '2', {2,1} },
    { '3', {2,2} },

    { '0', {3,1} },
    { 'A', {3,2} },

    KEY_LIST_TERM
};

const Key arrowpadMap[] = {
    { '^', {0,1} },
    { 'A', {0,2} },

    { '<', {1,0} },
    { 'v', {1,1} },
    { '>', {1,2} },

    KEY_LIST_TERM
};

typedef struct {
    char current;

    const Key* keymap;
} Keypad;

void Vec_Print( Vec* v ){
    printf( "(%2d, %2d)", v->r, v->c );
}

void Keypad_GetButtonCoord( Keypad* kpad, char key, Vec* coord )
{
    Key const* k;
    for( k = &kpad->keymap[0]; k->button != 0; k++ )
    {
        VERBOSE( "Try button %c\n", k->button );
        if( k->button == key ){
            break;
        }
    }

    if( k->button == 0 ){
        printf( "Key %c not found in keymap\n", key );
        abort();
    }

    coord->r = k->coord.r;
    coord->c = k->coord.c;
    kpad->current = key;
    //VERBOSE( "  Found %c at ", key );
    //Vec_Print( coord );
    //VERBOSE( "\n");
}

/* Press a specific key on a keypad by modifying the keypad's last-pressed button.
 * Calculates the distance between the last and current pressed keys, and puts it
 * in 'travel'
 */
void Keypad_ClickButton( Keypad* kpad, char key, Vec* travel )
{
    Vec orig;
    Vec new;

    Keypad_GetButtonCoord( kpad, kpad->current, &orig );
    Keypad_GetButtonCoord( kpad, key, &new );

    travel->r = new.r - orig.r;
    travel->c = new.c - orig.c;
}

/*
 *
 * Robot Arrangement:

/---------\ /---------\ /---------\ /---------\
|         | |         | |         | |         |
| Robot 3 | | Robot 2 | | Robot 1 | | Numpad  |
|         | |         | |         | |         |
\---------/ \---------/ \---------/ \---------/

*/

/*
 * Convert vector into sequence of V^<> moves
 */
int PrintMovesFromVec( Vec* v, char* buf, int withPush )
{
    int ntravel = 0;
    if( v->r > 0 ){
        for( int i = 0; i < v->r; i++ )
            *buf++ = 'v', ntravel++;

    } else if ( v->r < 0 ) {
        for( int i = v->r; i < 0; i++ )
            *buf++ = '^', ntravel++;
    }

    if( v->c > 0 ){
        for( int i = 0; i < v->c; i++ )
            *buf++ = '>', ntravel++;

    } else if ( v->c < 0 ) {
        for( int i = v->c; i < 0; i++ )
            *buf++ = '<', ntravel++;
    }

    // Sometimes we capture the pushing of the button 'A'
    if( withPush ){
        *buf++ = 'A', ntravel++;
    }

    return ntravel;
}

int RenderSequence( Keypad* kpad, const char* seq, char* buf, int doA )
{
    char* ptr = &buf[0];

    int totTravel = 0;

    //INFO( "Sequence %s\n", seq );
    for( char const* p = seq; *p != '\0'; p++ ){
        int ntravel = 0;
        Vec travel;

        VERBOSE( "Doing %c\n", *p );
        Keypad_ClickButton( kpad, *p, &travel );

        VERBOSE( "Travel: %d, %d\n", travel.r, travel.c );

        ntravel = PrintMovesFromVec( &travel, ptr, doA );
        ptr += ntravel;
        totTravel += ntravel;
    }
    DEBUG( "moves so far: %s\n", buf );
    DEBUG( "nmoves: %d\n", totTravel );

    return totTravel;
}

Keypad numpad = {.keymap = numpadMap,   .current = 'A'};
Keypad robot1 = {.keymap = arrowpadMap, .current = 'A'};
Keypad robot2 = {.keymap = arrowpadMap, .current = 'A'};

void ResetKeypad( Keypad* kpad )
{
    kpad->current = 'A';
}

void ResetAllKeypads()
{
    ResetKeypad( &numpad );
    ResetKeypad( &robot1 );
    ResetKeypad( &robot2 );
}

ull CalcSol( const char* seq )
{
    char buf1[512] = {0};
    char buf2[512] = {0};
    char buf3[512] = {0};

    int tot1 = RenderSequence( &numpad, seq, buf1, 1 );
    int tot2 = RenderSequence( &robot1, buf1, buf2, 1 );
    int tot3 = RenderSequence( &robot2, buf2, buf3, 1 );

    INFO( "%s: %d * %d\n", seq, tot3, atoi(seq) );
    return tot3 * atoi( seq );
}

//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

ull FullTotal = 0;

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    char* seq = line;

    ResetAllKeypads();
    ull sol = CalcSol( seq );
    printf( "%s: %lld\n", seq, sol );

    FullTotal += sol;
}

void SOLVER_PrintSolution( void )
{

    printf( " FullTotal: %lld\n", FullTotal );
}
