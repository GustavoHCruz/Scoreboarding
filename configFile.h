#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initializeStruct(InstConfig *instructionConfig)
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

void inst_cicles(char *cicle, char *str, InstConfig *instructionsConfig)
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

InstConfig readInstructionsConfig(char *configFileName)
{
    FILE *config;
    config = fopen(configFileName, "r");

    InstConfig instructionsConfig;

    char *cicleChar, *line = NULL;

    size_t size;
    size_t len = 100;

    int cicles;

    initializeStruct(&instructionsConfig);

    while ((size = getline(&line, &len, config)) != -1)
        inst_cicles(cicleChar, line, &instructionsConfig);

    return instructionsConfig;
}