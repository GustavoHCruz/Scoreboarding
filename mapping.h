#include <string.h>

int register_map(char *reg) // Function to map the registers
{
    if (strcmp(reg, "$zero") == 0)
        return 0;
    else if (strcmp(reg, "$at") == 0)
        return 1;
    else if (strcmp(reg, "$v0") == 0)
        return 2;
    else if (strcmp(reg, "$v1") == 0)
        return 3;
    else if (strcmp(reg, "$a0") == 0)
        return 4;
    else if (strcmp(reg, "$a1") == 0)
        return 5;
    else if (strcmp(reg, "$a2") == 0)
        return 6;
    else if (strcmp(reg, "$a3") == 0)
        return 7;
    else if (strcmp(reg, "$t0") == 0)
        return 8;
    else if (strcmp(reg, "$t1") == 0)
        return 9;
    else if (strcmp(reg, "$t2") == 0)
        return 10;
    else if (strcmp(reg, "$t3") == 0)
        return 11;
    else if (strcmp(reg, "$t4") == 0)
        return 12;
    else if (strcmp(reg, "$t5") == 0)
        return 13;
    else if (strcmp(reg, "$t6") == 0)
        return 14;
    else if (strcmp(reg, "$t7") == 0)
        return 15;
    else if (strcmp(reg, "$s0") == 0)
        return 16;
    else if (strcmp(reg, "$s1") == 0)
        return 17;
    else if (strcmp(reg, "$s2") == 0)
        return 18;
    else if (strcmp(reg, "$s3") == 0)
        return 19;
    else if (strcmp(reg, "$s4") == 0)
        return 20;
    else if (strcmp(reg, "$s5") == 0)
        return 21;
    else if (strcmp(reg, "$s6") == 0)
        return 22;
    else if (strcmp(reg, "$s7") == 0)
        return 23;
    else if (strcmp(reg, "$t8") == 0)
        return 24;
    else if (strcmp(reg, "$t9") == 0)
        return 25;
    else if (strcmp(reg, "$k0") == 0)
        return 26;
    else if (strcmp(reg, "$k1") == 0)
        return 27;
    else if (strcmp(reg, "$gp") == 0)
        return 28;
    else if (strcmp(reg, "$sp") == 0)
        return 29;
    else if (strcmp(reg, "$fp") == 0)
        return 30;
    else if (strcmp(reg, "$ra") == 0)
        return 31;
}

int function_map(char *opcode) // Function to map funct field (for R format)
{
    if (strcmp(opcode, "move") == 0)  // We get the opcode from nor to represent move's opcode
        return 39;
    else if (strcmp(opcode, "add") == 0)
        return 32;
    else if (strcmp(opcode, "sub") == 0)
        return 34;
    else if (strcmp(opcode, "and") == 0)
        return 36;
    else if (strcmp(opcode, "or") == 0)
        return 37;
    else if (strcmp(opcode, "slt") == 0)
        return 42;
    else if (strcmp(opcode, "mult") == 0)
        return 24;
    else if (strcmp(opcode, "div") == 0)
        return 26;
}

// void int_to_function_map(unsigned int operationInt, char *operationChar) // Function to map funct field (for R format)
// {
//     if (operationInt == 39)  // We get the opcode from nor to represent move's opcode
//         strcpy(*operationChar, "move");
//     else if (operationInt == 32)
//         strcpy(*operationChar, "add");
//     else if (operationInt == 34)  
//         strcpy(*operationChar, "sub");
//     else if (operationInt == 36)
//         strcpy(*operationChar, "and");
//     else if (operationInt == 37)  
//         strcpy(*operationChar, "or");
//     else if (operationInt == 42)
//         strcpy(*operationChar, "slt");
//     else if (operationInt == 24)  
//         strcpy(*operationChar, "mult");
//     else if (operationInt == 26)
//         strcpy(*operationChar, "div");
// }

int opcode_map(char *opcode) // Function to map opcode field (for I format)
{
    if (strcmp(opcode, "li") == 0) // We get the opcode from addiu to represent li's opcode
        return 9;
    else if (strcmp(opcode, "addi") == 0)
        return 8;
    else if (strcmp(opcode, "andi") == 0)
        return 12;
    else if (strcmp(opcode, "ori") == 0)
        return 13;
}