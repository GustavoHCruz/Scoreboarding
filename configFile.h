#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initializeStruct(InstConfig *instructionConfig)
{
    instructionConfig->li = 0;
    instructionConfig->move = 0;
    instructionConfig->add = 0;
    instructionConfig->addi = 0;
    instructionConfig->sub = 0;
    instructionConfig->or = 0;
    instructionConfig->ori = 0;
    instructionConfig->and = 0;
    instructionConfig->andi = 0;
    instructionConfig->slt = 0;
    instructionConfig->mult = 0;
    instructionConfig->div = 0;
}

void inst_clock(char *clock, char *str, InstConfig *instructionsConfig)
{
    char *instruction;
    instruction = strtok(str, " ");
    strtok(NULL, " ");
    clock = strtok(NULL, "\n");

    int clockValue = atoi(clock);

    if (strcmp(instruction, "li") == 0)
        instructionsConfig->li = clockValue;
    else if (strcmp(instruction, "move") == 0)
        instructionsConfig->move = clockValue;
    else if (strcmp(instruction, "add") == 0)
        instructionsConfig->add = clockValue;
    else if (strcmp(instruction, "addi") == 0)
        instructionsConfig->addi = clockValue;
    else if (strcmp(instruction, "sub") == 0)
        instructionsConfig->sub = clockValue;
    else if (strcmp(instruction, "or") == 0)
        instructionsConfig->or = clockValue;
    else if (strcmp(instruction, "ori") == 0)
        instructionsConfig->ori = clockValue;
    else if (strcmp(instruction, "and") == 0)
        instructionsConfig->and = clockValue;
    else if (strcmp(instruction, "andi") == 0)
        instructionsConfig->andi = clockValue;
    else if (strcmp(instruction, "slt") == 0)
        instructionsConfig->slt = clockValue;
    else if (strcmp(instruction, "mult") == 0)
        instructionsConfig->mult = clockValue;
    else if (strcmp(instruction, "div") == 0)
        instructionsConfig->div = clockValue;
}

InstConfig readInstructionsConfig(char *configFileName)
{
    FILE *config;
    config = fopen(configFileName, "r");

    InstConfig instructionsConfig;

    char *clock, *line = NULL;

    size_t size;
    size_t len = 100;

    initializeStruct(&instructionsConfig);

    while ((size = getline(&line, &len, config)) != -1)
        inst_clock(clock, line, &instructionsConfig);

    return instructionsConfig;
}