#include <stdio.h>
#include <math.h>

unsigned int convertTo(unsigned int *number, unsigned int length)
{
    unsigned int ans, aux;
    aux = pow(2,length) - 1;
    ans = *number & aux;
    *number = *number >> length;
    return ans;
}

Instruction readMemory(unsigned int inst)
{
    Instruction temp;
    if (inst > 67108863)
    {
        temp.type = I;
        temp.operand3 = convertTo(&inst, 16);
        temp.operand2 = convertTo(&inst, 5);
        temp.operand1 = convertTo(&inst, 5);
        temp.operation = convertTo(&inst, 6);
        temp.pipeline.issue = 0;
        temp.pipeline.read = 0;
        temp.pipeline.execute = 0;
        temp.pipeline.write = 0;
    }
    else
    {
        temp.type = R;
        temp.operation = convertTo(&inst, 6);
        convertTo(&inst,5);
        temp.operand1 = convertTo(&inst, 5);
        temp.operand3 = convertTo(&inst, 5);
        temp.operand2 = convertTo(&inst, 5);
        temp.pipeline.issue = 0;
        temp.pipeline.read = 0;
        temp.pipeline.execute = 0;
        temp.pipeline.write = 0;
    }
    return temp;
}

int processor(InstConfig instructionConfig, unsigned int instructionsMemory[],int memoryLength)
{
    Instruction instructions[memoryLength];
    for (int i=0;i<memoryLength;i++){
        instructions[i] = readMemory(instructionsMemory[i]);
        printf("\nTipo=%u\nOperacao=%u\nOperando1=%u\nOperando2=%u\nOperando3=%u\nPipeline[0]=%u\nPipeline[1]=%u\nPipelin[2]=%u\nPipeline[3]=%u\n",instructions[i].type,instructions[i].operation,instructions[i].operand1,instructions[i].operand2,instructions[i].operand3,instructions[i].pipeline.issue,instructions[i].pipeline.read,instructions[i].pipeline.execute,instructions[i].pipeline.write);
    }
}