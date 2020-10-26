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

typedef struct InstConfig
{
    int add, addi, and, andi, or, ori, lw, slt, sub, div, mult;
} InstConfig;