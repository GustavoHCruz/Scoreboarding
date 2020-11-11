#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
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

    scoreboardingFunction(instructionsConfig,instructionsMemory,memoryLength, outputFileName);

    return 0;
}
