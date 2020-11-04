#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "structs.h"
#include "converter.h"
#include "configFile.h"
#include "processor.h"

int main(int argc, char *argv[])
{
    int memoryLength, option;
    char *configFileName, *outputFileName, *programName;

    while ((option = getopt(argc, argv, ":c:o:p:m:")) != -1)
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
        case 'm':
            memoryLength = atoi(optarg);
            break;
        default: // Invalid params
            printf("Usage Mode: -c <ConfigFileName> -o <OutputFileName> -p <ProgramName> -m <MemoryLength>\n");
        }
    }
    unsigned int instructionsMemory[memoryLength];
    converter(programName, instructionsMemory);
    InstConfig instructionsConfig = readInstructionsConfig(configFileName);

    // printf("Teste da Memoria de Instrucoes\n");
    // for (int i = 0; i < memoryLength; i++)
    //     printf("M[%i]=%u\n", i, instructionsMemory[i]);

    // printf("\nTeste da Configuracao das Instrucoes\n");
    // printf("li=%i\namove=%i\nadd=%i\nandi=%i\nsub=%i\nor=%i\nori=%i\nand=%i\nandi=%i\nslt=%i\nmult=%i\ndiv=%i\n", instructionsConfig.li, instructionsConfig.move, instructionsConfig.add, instructionsConfig.addi, instructionsConfig.sub, instructionsConfig.or, instructionsConfig.ori, instructionsConfig.and, instructionsConfig.andi, instructionsConfig.slt, instructionsConfig.mult, instructionsConfig.div);

    scoreboardingFunction(instructionsConfig,instructionsMemory,memoryLength);

    return 0;
}