#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "common.h"
#include "problem_solver.h"

//--------------------------------------------------------------------------------
//  Private State
//--------------------------------------------------------------------------------

#define MAXBLUEPRINT    31

int nblueprints = 0;
struct blueprint blueprints[ MAXBLUEPRINT ];

int CanAfford( const struct blueprint* bp, const struct bag *bag, enum resource resource )
{
    switch( resource ){
        case ORE:
            if( bag->nOre < bp->ore_cost ) return 0;
            return 1;
            break;

        case CLAY:
            if( bag->nOre < bp->clay_cost ) return 0;
            return 1;
            break;

        case OBSIDIAN:
            if( bag->nOre < bp->obs_cost_ore ) return 0;
            if( bag->nClay < bp->obs_cost_clay ) return 0;
            return 1;
            break;

        case GEODE:
            if( bag->nOre < bp->geo_cost_ore ) return 0;
            if( bag->nObs < bp->geo_cost_obs ) return 0;
            return 1;
            break;

        default:
            printf( "Invalid case 0x%x\n", resource );
            abort();
            return 0;
    }
}


/* Return bitmask of all options that we can afford
 */
int CanAffordAny( const struct blueprint* bp, const struct bag *bag )
{
    int mask = 0;

    do {
        if( bag->nOre < bp->ore_cost ) break;
        mask |= ORE;
    } while(0);

    do {
        if( bag->nOre < bp->clay_cost ) break;
        mask |= CLAY;
    } while(0);

    do {
        if( bag->nOre < bp->obs_cost_ore ) break;
        if( bag->nClay < bp->obs_cost_clay ) break;
        mask |= OBSIDIAN;
    } while(0);

    do {
        if( bag->nOre < bp->geo_cost_ore ) break;
        if( bag->nObs < bp->geo_cost_obs ) break;
        mask |= GEODE;
    } while(0);

    return mask;
}


//--------------------------------------------------------------------------------
//  API Functions
//--------------------------------------------------------------------------------

void SOLVER_ProcessLine( char* line )
{
    rmnewl( line );

    struct blueprint* bp = &blueprints[ nblueprints ];
    bp->index = nblueprints;

    #define NUMBER "0123456789"

    // Every recipe is identical, only the numbers are different.
    // So we look for the numbers only.

    // First number is index; we don't care so we skip
    line = strpbrk( line, NUMBER );
    line = strchr( line, ' ' );

    // Cost of ore robot
    line = strpbrk( line, NUMBER );
    bp->ore_cost = atoi( line );

    // Cost of clay robot
    line = strchr( line, ' ' );
    line = strpbrk( line, NUMBER );
    bp->clay_cost = atoi( line );

    // Cost of obsidian robot (ore)
    line = strchr( line, ' ' );
    line = strpbrk( line, NUMBER );
    bp->obs_cost_ore = atoi( line );

    // Cost of obsidian robot (clay)
    line = strchr( line, ' ' );
    line = strpbrk( line, NUMBER );
    bp->obs_cost_clay = atoi( line );

    // Cost of geode robot (ore)
    line = strchr( line, ' ' );
    line = strpbrk( line, NUMBER );
    bp->geo_cost_ore = atoi( line );

    // Cost of geode robot (obsidian)
    line = strchr( line, ' ' );
    line = strpbrk( line, NUMBER );
    bp->geo_cost_obs = atoi( line );

    nblueprints ++;
    assert( nblueprints < MAXBLUEPRINT );
}

void PrintBlueprint( struct blueprint *bp )
{
    assert(bp);

    printf( "Blueprint %d\n", bp->index );
    printf( "  Ore  Robot cost: %d ore\n", bp->ore_cost );
    printf( "  Clay Robot cost: %d ore\n", bp->clay_cost );
    printf( "  Obsidian Robot cost: %d ore, %d clay\n", bp->obs_cost_ore, bp->obs_cost_clay );
    printf( "  Geode Robot cost: %d ore, %d obsidian\n", bp->geo_cost_ore, bp->geo_cost_obs );
}

void SOLVER_Init( void* p )
{
    // If not true, then we can't hash it using 64-bit number
    assert( sizeof(struct factoryState) <= 8 );
}

void SOLVER_PrintSolution( void )
{
    struct blueprint *bp = &blueprints[0];
    PrintBlueprint( bp );

    struct bag bag;
    memset( &bag, 0, sizeof(bag));

    bag.nOre = 3;
    bag.nClay = 2;

    printf( "Can afford ore:      %d\n", CanAfford( bp, &bag, ORE ) );
    printf( "Can afford clay:     %d\n", CanAfford( bp, &bag, CLAY ) );
    printf( "Can afford obsidian: %d\n", CanAfford( bp, &bag, OBSIDIAN ) );
    printf( "Can afford geode:    %d\n", CanAfford( bp, &bag, GEODE ) );
}

void InitFactory( struct factoryState *state )
{
    assert( state );
    memset( state, 0, sizeof(*state));

    state->minsRemain = 24;
    state->robots.nOre = 1;
}

/*
 * Recursively find the max number of geode robots we can build, given:
 *  - How many robots we have
 *  - How many resources we have
 *  - How much time is left
 */
int MaxNumGeodeRobots( const struct blueprint* bp, struct factoryState* state )
{
    if( state->minsRemain <= 0 ){
        return 0;
    }
    if( state->minsRemain == 1 ) {
        if( CanAfford(bp, &state->bag, GEODE) )
            return 1;
        else
            return 0;
    }

    /*
     * 0. Calculate resources we will gain this round, but do NOT gain them yet;
     *    we gain them after we build a robot
     *
     * 1. Calculate (recursively) which robot we should build to maximize long-term
     *    geode gains
     *
     * 2. Pay resources to build the optimal robot (if any)
     *
     * 3. Collect the produced resources from the existing robots (not including
     *      the newly-built one)
     */
}
