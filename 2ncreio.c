#include <pthread.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "structs.h"
#include "converter.h"
#include "configFile.h"
#include "processor.h"

typedef struct thread_info
{
    pthread_t thread_id;
    int thread_num;
    char *configFileName;
    char *outputFileName;
    char *programName;
    int memoryLength;
} thread_info;

/* function to be executed */
void *scoreboarding(void *arg)
{
    thread_info *infos = arg;
    unsigned int instructionsMemory[infos->memoryLength];
    converter(infos->programName, instructionsMemory);
    InstConfig instructionsConfig = readInstructionsConfig(infos->configFileName);

    scoreboardingFunction(instructionsConfig,instructionsMemory,infos->memoryLength, infos->outputFileName);

    return NULL;
}

int main(int argc, char *argv[])
{
    thread_info *tinfo;
    int memoryLength1, memoryLength2, option;
    char *configFileName, *outputFileName1, *outputFileName2, *programName1, *programName2;

    while ((option = getopt(argc, argv, ":c:x:y:z:r:s:t:")) != -1)
    {
        switch (option)
        {
        case 'c':
            configFileName = optarg;
            break;
        case 'x':
            outputFileName1 = optarg;
            break;
        case 'y':
            programName1 = optarg;
            break;
        case 'z':
            memoryLength1 = atoi(optarg);
            break;
        case 'r':
            outputFileName2 = optarg;
            break;
        case 's':
            programName2 = optarg;
            break;
        case 't':
            memoryLength2 = atoi(optarg);
            break;
        default: // Invalid params
            printf("Usage Mode: -c <ConfigFileName> -x <OutputFileName1> -y <ProgramName1> -z <MemoryLength1> -r <OutputFileName2> -s <ProgramName2> -t <MemoryLength2>\n");
        }
    }
    int num_thread = 2;
    tinfo = calloc(num_thread, sizeof(struct thread_info));
    tinfo[0].thread_num = 0;
    tinfo[0].configFileName = configFileName;
    tinfo[0].programName = programName1;
    tinfo[0].memoryLength = memoryLength1;
    tinfo[1].thread_num = 1;
    tinfo[1].configFileName = configFileName;
    tinfo[1].programName = programName2;
    tinfo[1].memoryLength = memoryLength2;

    /* create */
    pthread_create(&tinfo[0].thread_id, NULL, scoreboarding, &tinfo[0]);
    pthread_create(&tinfo[1].thread_id, NULL, scoreboarding, &tinfo[1]);

    /* wait to finish */
    pthread_join(tinfo[0].thread_id, NULL);
    pthread_join(tinfo[1].thread_id, NULL);

    printf("Acabou!");
}
