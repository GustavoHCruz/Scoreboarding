#include<stdio.h>

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

struct inst_r{ //três formatos diferentes
    int opcode;
    int rs;
    int rt;
    int rd;
    int shamt;
    int funct;
}inst_r;

struct memoria_registradores{
    char nome;
    int valor;
}memoria_registradores;

typedef struct unidade_1{

}unidade_1;

//uma struct para cada unidade?

struct scoreboarding{
    unidade_1 add1;
    unidade_1 add2;
};

struct pipeline{

}pipeline;

void ler_arquivo();

void converter_arquivo();

//busca

//decode

//executa

//escreve

void executar();

int main(int argc, char const *argv[])
{
    ler_arquivo();
    converter_arquivo();
    executar();

    return 0;
}