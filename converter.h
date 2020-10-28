#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mapping.h"

int instructions_counter(char *fileName)
{
    size_t len = 100;
    size_t size;
    char *line = NULL;
    FILE *file;
    file = fopen(fileName, "r");

    int instructionsCounter = 0;

    while ((size = getline(&line, &len, file)) != -1)
    {
        instructionsCounter++;
    }

    return instructionsCounter;
}

int inst_return(Instruction_R *R, Instruction_I *I, char *str) // Broke the lines and map the parts of it into his respective format, returning it at end
{
    int i = 1;
    char *token, *data[4];

    token = strtok(str, " ");
    data[0] = token;
    token = strtok(NULL, "");
    token = strtok(token, ", ");
    while (token != NULL)
    {
        data[i] = token;
        token = strtok(NULL, ", ");
        i++;
    }

    int temp;
    if (i > 3){
        temp = strlen(data[3]);
        if (data[3][temp - 1] == '\n')
            data[3][temp-1] = '\0';
    }
    else{
        temp = strlen(data[2]);
        if (data[2][temp -1] == '\n')
            data[2][temp - 1] = '\0';
    }

    if (strcmp(data[0], "add") == 0 || strcmp(data[0], "and") == 0 || strcmp(data[0], "or") == 0 || strcmp(data[0], "slt") == 0 || strcmp(data[0], "sub") == 0 || strcmp(data[0], "mult") == 0 || strcmp(data[0], "div") == 0)
    {
        R->opcode = 0;
        R->rs = register_map(data[2]);
        R->rt = register_map(data[3]);
        R->rd = register_map(data[1]);
        R->shamt = 0;
        R->funct = function_map(data[0]);
        return 1;
    }
    else if (strcmp(data[0], "addi") == 0 || strcmp(data[0], "andi") == 0 || strcmp(data[0], "ori") == 0)
    {
        I->opcode = opcode_map(data[0]);
        I->rs = register_map(data[2]);
        I->rt = register_map(data[1]);
        I->immediate = atoi(data[3]);
        return 2;
    }
    else if (strcmp(data[0], "move") == 0)
    {
        R->opcode = 0;
        R->rd = register_map(data[1]);
        R->rs = register_map(data[2]);
        R->rt = 0;
        R->shamt = 0;
        R->funct = function_map(data[0]);
        return 1;
    }
    else if (strcmp(data[0], "li") == 0)
    {
        I->opcode = opcode_map(data[0]);
        I->rs = 0;
        I->rt = register_map(data[1]);
        I->immediate = atoi(data[2]);
        return 2;
    }
    else
        return 0;
}

unsigned int toConvert(unsigned int result, int amount, unsigned int data) // Result = Number to be returned, Amount = amount of bits to be shifted, Data = Number to do the OR operation
{
    result = result << amount;
    result = result | data;
    return result;
}

int converter(char *fileName, unsigned int instructionsMemory[]) // It opens the files and treat the logic of the converter
{
    size_t len = 100;
    size_t size;
    FILE *file;
    int instructionsCounter = 0;
    file = fopen(fileName, "r");

    if (file == NULL)
        return 0;

    char *line = NULL;

    Instruction_R R;
    Instruction_I I;
    unsigned int res;
    int ret;

    int i = 0;
    while ((size = getline(&line, &len, file)) != -1)
    {
        res = 0;
        ret = inst_return(&R, &I, line);
        if (ret == 1)
        {
            res = toConvert(res, 6, R.opcode);
            res = toConvert(res, 5, R.rs);
            res = toConvert(res, 5, R.rt);
            res = toConvert(res, 5, R.rd);
            res = toConvert(res, 5, R.shamt);
            res = toConvert(res, 6, R.funct);
        }
        else if (ret == 2)
        {
            res = toConvert(res, 6, I.opcode);
            res = toConvert(res, 5, I.rs);
            res = toConvert(res, 5, I.rt);
            res = toConvert(res, 16, I.immediate);
        }
        instructionsMemory[i] = res;
        i++;
    }
    fclose(file);

    return 1;
}