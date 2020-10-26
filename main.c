#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "structs.h"
#include "converter.h"
#include "configFile.h"

int main(int argc, char *argv[])
{
    int option;
    char *configFileName, *outputFileName, *programName;

    while ((option = getopt(argc, argv, ":c:o:p:")) != -1)
    {
        switch (option)
        {
        case 'c':
            configFileName = optarg;
            break;
        case 'o':
            outputFileName = optarg;
            break;
        case 'p':
            programName = optarg;
            break;
        default: // Invalid params
            printf("Usage: -c <ConfigFileName> -o <OutputFileName> -p <ProgramName>\n");
        }
    }
    int inst_amount = instructions_counter(programName);
    unsigned int instructionsMemory[inst_amount];
    converter(programName,instructionsMemory);
    InstConfig instructionsConfig = readInstructionsConfig(configFileName);

    /* printf("Teste da Memoria de Instrucoes\n");
    for(int i=0;i<inst_amount;i++)
        printf("M[%i]=%u\n",i,instructionsMemory[i]);
    
    printf("\nTeste da Configuracao das Instrucoes\n");
    printf("add=%i\naddi=%i\nand=%i\nandi=%i\nor=%i\nori=%i\nlw=%i\nslt=%i\nsub=%i\ndiv=%i\nmult=%i\n",instructionsConfig.add,instructionsConfig.addi,instructionsConfig.and,instructionsConfig.andi,instructionsConfig.or,instructionsConfig.ori,instructionsConfig.lw,instructionsConfig.slt,instructionsConfig.sub,instructionsConfig.div,instructionsConfig.mult); */

    

    return 0;
}