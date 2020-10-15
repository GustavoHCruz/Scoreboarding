#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    unsigned int opcode : 6;
    unsigned int rs : 5;
    unsigned int rt : 5;
    unsigned int immediate : 16;
} Instruction_I;

int register_map(char reg[2])
{
    if (strcmp(reg, "F0") == 0)
        return 0;
    if (strcmp(reg, "F1") == 0)
        return 1;
    if (strcmp(reg, "F2") == 0)
        return 2;
    if (strcmp(reg, "F3") == 0)
        return 3;
    if (strcmp(reg, "F4") == 0)
        return 4;
    if (strcmp(reg, "F5") == 0)
        return 5;
    if (strcmp(reg, "F6") == 0)
        return 6;
    if (strcmp(reg, "F7") == 0)
        return 7;
    if (strcmp(reg, "F8") == 0)
        return 8;
    if (strcmp(reg, "F9") == 0)
        return 9;
    if (strcmp(reg, "F10") == 0)
        return 10;
    if (strcmp(reg, "F11") == 0)
        return 11;
    if (strcmp(reg, "F12") == 0)
        return 12;
    if (strcmp(reg, "F13") == 0)
        return 13;
    if (strcmp(reg, "F14") == 0)
        return 14;
    if (strcmp(reg, "F15") == 0)
        return 15;
}

int function_map(char *opcode)
{
    if (strcmp(opcode, "add") == 0)
        return 32;
    if (strcmp(opcode, "and") == 0)
        return 36;
    if (strcmp(opcode, "or") == 0)
        return 37;
    if (strcmp(opcode, "slt") == 0)
        return 42;
    if (strcmp(opcode, "sub") == 0)
        return 34;
    if (strcmp(opcode, "mult") == 0)
        return 26;
    if (strcmp(opcode, "div") == 0)
        return 24;
}

int opcode_map(char *opcode)
{
    if (strcmp(opcode, "addi") == 0)
        return 8;
    if (strcmp(opcode, "andi") == 0)
        return 12;
    if (strcmp(opcode, "ori") == 0)
        return 13;
    if (strcmp(opcode, "lw") == 0)
        return 35;
}

int inst_return(Instruction_R *R, Instruction_I *I, char *str)
{
    int i = 0;
    char *token, *data[4];

    token = strtok(str, " ");
    while (token != NULL)
    {
        data[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    if (strcmp(data[0], "add") == 0 || strcmp(data[0], "and") == 0 || strcmp(data[0], "or") == 0 || strcmp(data[0], "slt") == 0 || strcmp(data[0], "sub") == 0 || strcmp(data[0], "mult") == 0 || strcmp(data[0], "div") == 0)
    {
        R->opcode = 0;
        R->rs = register_map(data[1]);
        R->rt = register_map(data[2]);
        R->rd = register_map(data[3]);
        R->shamt = 0;
        R->funct = function_map(data[0]);
        return 1;
    }
    else if (strcmp(data[0], "addi") == 0 || strcmp(data[0], "andi") == 0 || strcmp(data[0], "ori") == 0 || strcmp(data[0], "lw") == 0)
    {
        I->opcode = opcode_map(data[0]);
        I->rs = register_map(data[1]);
        I->immediate = atoi(data[2]);
        I->rt = register_map(data[3]);
        return 2;
    }
    else
        return 0;
}

int main()
{
    //char line[] = "add F3 F1 F2";
    char line[] = "lw F1 30 F2";
    Instruction_R A;
    Instruction_I B;
    int ret = inst_return(&A, &B, line);
    printf("Retornou: %i \n", ret);
    if(ret == 1)
        printf("%i,%i,%i,%i,%i,%i", A.opcode, A.rd, A.rs, A.rt, A.shamt, A.funct);
    else if(ret == 2)
        printf("%i,%i,%i,%i",B.opcode,B.rs,B.immediate,B.rt);
}