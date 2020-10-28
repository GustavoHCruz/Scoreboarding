enum registers
{
    zero = 0b00000,
    at = 0b00001,
    v0 = 0b00010,
    v1 = 0b00011,
    a0 = 0b00100,
    a1 = 0b00101,
    a2 = 0b00110,
    a3 = 0b00111,
    t0 = 0b01000,
    t1 = 0b01001,
    t2 = 0b01010,
    t3 = 0b01011,
    t4 = 0b01100,
    t5 = 0b01101,
    t6 = 0b01110,
    t7 = 0b01111,
    s0 = 0b10000,
    s1 = 0b10001,
    s2 = 0b10010,
    s3 = 0b10011,
    s4 = 0b10100,
    s5 = 0b10101,
    s6 = 0b10110,
    s7 = 0b10111,
    t8 = 0b11000,
    t9 = 0b11001,
    k0 = 0b11010,
    k1 = 0b11011,
    gp = 0b11100,
    sp = 0b11101,
    fp = 0b11110,
    ra = 0b11111
};

enum units
{
    INT, MULT1, MULT2, DIV, ADD, SUB
};

typedef struct Instruction_R // R instruction format
{
    unsigned int opcode : 6; // Always zero
    unsigned int rs : 5;     // Origin register 1
    unsigned int rt : 5;     // Origin register 2
    unsigned int rd : 5;     // Destination register
    unsigned int shamt : 5;  // Shift operator
    unsigned int funct : 6;  // Specifies the operation
} Instruction_R;

typedef struct Instruction_I // I instruction format
{
    unsigned int opcode : 6;     // Specifies the operation
    unsigned int rs : 5;         // Origin register
    unsigned int rt : 5;         // Destination register
    unsigned int immediate : 16; // Immediate value
} Instruction_I;

typedef struct InstConfig
{
    int li, move, add, addi, sub, or, ori, and, andi, slt, mult, div;
} InstConfig;