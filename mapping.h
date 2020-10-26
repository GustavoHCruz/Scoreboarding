#include <string.h>

int register_map(char reg[2]) // Function to map the registers
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

int function_map(char *opcode) // Function to map funct field (for R format)
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

int opcode_map(char *opcode) // Function to map opcode field (for I format)
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