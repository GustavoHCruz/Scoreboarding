#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

typedef struct instrConfig
{
    int add, addi, and, andi, or, ori, lw, slt, sub, div, mult;
} instrConfig;

void initializeStruct(instrConfig *instructionConfig)
{
    instructionConfig->add = 0;
    instructionConfig->addi = 0;
    instructionConfig->and = 0;
    instructionConfig->andi = 0;
    instructionConfig->or = 0;
    instructionConfig->ori = 0;
    instructionConfig->lw = 0;
    instructionConfig->slt = 0;
    instructionConfig->sub = 0;
    instructionConfig->div = 0;
    instructionConfig->mult = 0;
}

void inst_cicles(char *cicle, char *str, instrConfig *instructionsConfig)
{
    char *instruction;
    instruction = strtok(str, " ");
    strtok(NULL, " ");
    cicle = strtok(NULL, "\n");

    int valueCicle = atoi(cicle);

    if (strcmp(instruction, "add") == 0)
        instructionsConfig->add = valueCicle;
    else if (strcmp(instruction, "addi") == 0)
        instructionsConfig->addi = valueCicle;
    else if (strcmp(instruction, "and") == 0)
        instructionsConfig->and = valueCicle;
    else if (strcmp(instruction, "andi") == 0)
        instructionsConfig->andi = valueCicle;
    else if (strcmp(instruction, "or") == 0)
        instructionsConfig->or = valueCicle;
    else if (strcmp(instruction, "ori") == 0)
        instructionsConfig->ori = valueCicle;
    else if (strcmp(instruction, "lw") == 0)
        instructionsConfig->lw = valueCicle;
    else if (strcmp(instruction, "slt") == 0)
        instructionsConfig->slt = valueCicle;
    else if (strcmp(instruction, "sub") == 0)
        instructionsConfig->sub = valueCicle;
    else if (strcmp(instruction, "div") == 0)
        instructionsConfig->div = valueCicle;
    else if (strcmp(instruction, "mult") == 0)
        instructionsConfig->mult = valueCicle;
}

void ler_arquivo()
{
    FILE *config;
    config = fopen("config.txt", "r");

    instrConfig instructionsConfig;

    char *cicleChar, *line = NULL;

    size_t size;
    size_t len = 100;

    int cicles;

    initializeStruct(&instructionsConfig);

    while ((size = getline(&line, &len, config)) != -1)
        inst_cicles(cicleChar, line, &instructionsConfig);
}

int main()
{
    ler_arquivo();
    return 0;
}