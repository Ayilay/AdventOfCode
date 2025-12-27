#ifndef COMMON_PROBLEM_H
#define COMMON_PROBLEM_H

#include "problem_solver.h"

// Thank you Eric for making every recipe consistent...
struct blueprint {
    ui index;   // Blueprint number

    ui ore_cost;
    ui clay_cost;

    ui obs_cost_ore;
    ui obs_cost_clay;

    ui geo_cost_ore;
    ui geo_cost_obs;
};

// How many of each robot do we have; max 16 of each robot
// (16-bits)
struct robots {
    u8 nOre  : 4;
    u8 nClay : 4;
    u8 nObs  : 4;
    u8 nGeo  : 4;
};

// Bag holds all of our resources
// (32-bits)
struct bag {
    u8 nOre;
    u8 nClay;
    u8 nObs;
    u8 nGeo;
};

struct factoryState {
    struct bag    bag;
    struct robots robots;
    int8_t        minsRemain;
};

// This encodes both a raw resource AND a robot type, depending on context
enum resource {
    ORE             = (u8) 1 << 0,
    CLAY            = (u8) 1 << 1,
    OBSIDIAN        = (u8) 1 << 2,
    GEODE           = (u8) 1 << 3,
};

#endif
