#ifndef CPU_H
#define CPU_H

#define adv     0
#define bxl     1
#define bst     2
#define jnz     3
#define bxc     4
#define out     5
#define bdv     6
#define cdv     7

// Programs are very small
#define MAX     20

#define OUTMAX  200


typedef struct {
    int A;
    int B;
    int C;

    int PC;

    int nProgram;
    int program[ MAX ];

    int nOut;
    int output[ OUTMAX ];
} context;

void runProgram( context* cpu );
void printContext( context* ctx );

#endif
