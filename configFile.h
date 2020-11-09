#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initializeStruct(InstConfig *instructionConfig)
{
    for (int i=0;i<inst_n;i++){
        instructionConfig->configs[i] = 0;
    }
}

void inst_clock(char *clock, char *str, InstConfig *instructionsConfig)
{
    char *instruction;
    instruction = strtok(str, " ");
    strtok(NULL, " ");
    clock = strtok(NULL, "\n");

    int clockValue = atoi(clock);

    if (strcmp(instruction, "li") == 0)
        instructionsConfig->configs[Li] = clockValue;
    else if (strcmp(instruction, "move") == 0)
        instructionsConfig->configs[Move] = clockValue;
    else if (strcmp(instruction, "add") == 0)
        instructionsConfig->configs[Add] = clockValue;
    else if (strcmp(instruction, "addi") == 0)
        instructionsConfig->configs[Addi] = clockValue;
    else if (strcmp(instruction, "sub") == 0)
        instructionsConfig->configs[Sub] = clockValue;
    else if (strcmp(instruction, "or") == 0)
        instructionsConfig->configs[Or] = clockValue;
    else if (strcmp(instruction, "ori") == 0)
        instructionsConfig->configs[Ori] = clockValue;
    else if (strcmp(instruction, "and") == 0)
        instructionsConfig->configs[And] = clockValue;
    else if (strcmp(instruction, "andi") == 0)
        instructionsConfig->configs[Andi] = clockValue;
    else if (strcmp(instruction, "slt") == 0)
        instructionsConfig->configs[Slt] = clockValue;
    else if (strcmp(instruction, "mult") == 0)
        instructionsConfig->configs[Mult] = clockValue;
    else if (strcmp(instruction, "div") == 0)
        instructionsConfig->configs[Div] = clockValue;
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