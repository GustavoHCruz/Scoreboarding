#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
    F10 = 0b1010,
    F11 = 0b1011,
    F12 = 0b1100,
    F13 = 0b1101,
    F14 = 0b1110,
    F15 = 0b1111
};

typedef struct Instruction_R // R instruction format
{
    unsigned int opcode : 6; // Always zero
    unsigned int rs : 5;     // Destination register
    unsigned int rt : 5;     // Origin register 1
    unsigned int rd : 5;     // Origin register 2
    unsigned int shamt : 5;  // Shift operator
    unsigned int funct : 6;  // Specifies the operation
} Instruction_R;

typedef struct Instruction_I     // I instruction format
{
    unsigned int opcode : 6;     // Specifies the operation
    unsigned int rs : 5;         // Destination register
    unsigned int rt : 5;         // Origin register
    unsigned int immediate : 16; // Immediate value
} Instruction_I;

int register_map(char reg[2])   // Function to map the registers
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

int function_map(char *opcode)          // Function to map funct field (for R format)
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
        return 24;
    if (strcmp(opcode, "div") == 0)
        return 26;
}

int opcode_map(char *opcode)            // Function to map opcode field (for I format)
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

int inst_return(Instruction_R *R, Instruction_I *I, char *str) // Broke the lines and map the parts of it into his respective format, returning it at end
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

    int temp = strlen(data[3]);
    data[3][temp] = '\0';

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

int converter(char *fileName)   // It opens the files and treat the logic of the converter
{
    size_t len = 100;
    size_t size;
    FILE *file, *result;
    file = fopen(fileName, "r");
    result = fopen("binary.txt", "wb");

    if (file == NULL)
        return 0;

    char *line = NULL;

    Instruction_R R;
    Instruction_I I;
    long unsigned int res;
    int ret;

    while ((size = getline(&line, &len, file)) != -1)
    {
        ret = inst_return(&R, &I, line);
        if (ret == 1)
        {
            res = R.funct + R.shamt * pow(2, 6) + R.rd * pow(2, 11) + R.rt * pow(2, 16) + R.rs * pow(2, 21) + R.opcode * pow(2, 26);
            printf("%i,%i,%i,%i,%i,%i - Numero 32 bits:%i\n", R.opcode, R.rs, R.rt, R.rd, R.shamt, R.funct, res);
        }
        else if (ret == 2)
        {
            res = I.immediate + I.rt * pow(2, 16) + I.rs * pow(2, 21) + I.opcode * pow(2, 26);
            printf("%i,%i,%i,%i - Numero 32 bits:%lu\n", I.opcode, I.rs, I.rt, I.immediate, res);
        }

        fwrite(&res, 1, sizeof(long unsigned int), result);
    }

    fclose(file);
    fclose(result);
}

int main()
{
    converter("inputFile.txt");
}