#include <stdio.h>
#include "structs.h"

typedef struct FunctionUnity
{
    unsigned int busy : 1;
    char *operation;
    unsigned int fi : 5;
    unsigned int fj : 5;
    unsigned int fk : 5;
    unsigned int qj : 3;
    unsigned int qk : 3;
    unsigned int rj : 1;
    unsigned int rk : 1;
} FunctionUnity;

typedef struct Scoreboarding
{
    FunctionUnity FP_Mult1;
    FunctionUnity FP_Mult2;
    FunctionUnity FP_Div;
    FunctionUnity FP_Add;
    FunctionUnity Int_unit;
} Scoreboarding;

typedef struct Pipeline
{

} Pipeline;

typedef struct Instruction
{

} Instruction;

void deconvert()
{
}

int processor()
{
}