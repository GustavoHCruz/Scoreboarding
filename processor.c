#include<stdio.h>

enum registers
{
    F0 = 0b0000,
    F1 = 0b0001,
    F2 = 0b0010,
    F3 = 0b0011,
    F4 = 0b0100,
    F5 = 0b0101,
    F6 = 0b0110,
    F7 = 0b0111,
    F8 = 0b1000,
    F9 = 0b1001,
};

typedef struct Instruction_R
{
    unsigned int opcode : 6; // Always zero
    unsigned int rs : 5;     // Destination register
    unsigned int rt : 5;     // Origin register 1
    unsigned int rd : 5;     // Origin register 2
    unsigned int shamt : 5;  // Shift operator
    unsigned int funct : 6;  // Specifies the operation
} Instruction_R;

typedef struct Instruction_I
{
    unsigned int opcode : 6;     // Specifies the operation
    unsigned int rs : 5;         // Destination register
    unsigned int rt : 5;         // Origin register
    unsigned int immediate : 16; // Immediate value
} Instruction_I;

typedef struct instrConfig{
    char * name;
    int cicles;
}instrConfig;

// typedef struct unidade_1{

// }unidade_1;

//uma struct para cada unidade?

// typedef struct scoreboarding{
//     unidade_1 add1;
//     unidade_1 add2;
// } scoreboarding;

// struct pipeline{

// }pipeline;



void converter_arquivo();

//busca

//decode

//executaCONFIG)
{
    ler_arquivo();
    converter_arquivo();
    executar();

    return 0;
}