#include <stdio.h>
#include <math.h>

unsigned int convertTo(unsigned int *number, unsigned int length)
{
    unsigned int ans, aux;
    aux = pow(2, length) - 1;
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
        convertTo(&inst, 5);
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

int depedencyCheck(FunctionUnity f, unsigned int operand)
{
    if (f.fi == operand || f.fj == operand || f.fk == operand)
        return 1;
}

int rawCheck(Scoreboarding scoreboarding, Instruction instruction)
{
    int result = 0;
    result = depedencyCheck(scoreboarding.FP_Add, instruction.operand2);
    if (instruction.type == R)
        result = depedencyCheck(scoreboarding.FP_Add, instruction.operand3);
    depedencyCheck(scoreboarding.FP_Div, instruction.operand2);
    if (instruction.type == R)
        depedencyCheck(scoreboarding.FP_Div, instruction.operand3);
        depedencyCheck(scoreboarding.FP_Mult1, instruction.operand2);
    if (instruction.type == R)
        depedencyCheck(scoreboarding.FP_Mult1, instruction.operand3);
        depedencyCheck(scoreboarding.FP_Mult2, instruction.operand2);
    if (instruction.type == R)
        depedencyCheck(scoreboarding.FP_Mult2, instruction.operand3);
        depedencyCheck(scoreboarding.Int_unit, instruction.operand2);
    if (instruction.type == R)
        depedencyCheck(scoreboarding.Int_unit, instruction.operand3);
    
    return result;
}

int warCheck(Scoreboarding scoreboarding, Instruction instruction)
{
}

int wawCheck(Scoreboarding scoreboarding, Instruction instruction)
{
}

int update_data_in_functional_units(Scoreboarding *scoreboarding, FunctionUnity *functionUnity, char *nameOperation, Instruction instruction)
{
    if (!(rawCheck(*scoreboarding, instruction) && warCheck(*scoreboarding, instruction) && wawCheck(*scoreboarding, instruction)))
    {
        strcpy(scoreboarding->registerTable[instruction.operand1], functionUnity->name);
        strcpy(functionUnity->operation, *nameOperation);
        functionUnity->busy = 1;
        if (instruction.type == R)
        {
            functionUnity->fi = instruction.operand1;
            functionUnity->fj = instruction.operand2;
            functionUnity->fk = instruction.operand3;
        }
        else
        {
            functionUnity->fi = instruction.operand1;
            functionUnity->fj = instruction.operand2;
            functionUnity->fk = NILL;
        }
    }
}

int issue(unsigned int instructionsMemory, Scoreboarding *scoreboarding, char *registerMemory[])
{
    Instruction instructionRead;
    instructionRead = readMemory(instructionsMemory);

    if (instructionRead.operation == Move)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->Int_unit, "move", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Add)
    {
        if (scoreboarding->FP_Add.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->FP_Add, "add", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Sub)
    {
        if (scoreboarding->FP_Add.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->FP_Add, "sub", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == And)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->Int_unit, "and", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Or)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->Int_unit, "or", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Slt)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->Int_unit, "slt", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Mult)
    {
        if (scoreboarding->FP_Mult1.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->FP_Mult1, "mult", instructionRead);
            return 1;
        }
        else if (scoreboarding->FP_Mult2.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->FP_Mult2, "mult", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Div)
    {
        if (scoreboarding->FP_Div.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->FP_Div, "div", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Li)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->Int_unit, "li", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Addi)
    {
        if (scoreboarding->FP_Add.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->FP_Add, "addi", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Andi)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->Int_unit, "andi", instructionRead);
            return 1;
        }
    }
    else if (instructionRead.operation == Ori)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            update_data_in_functional_units(&scoreboarding, &scoreboarding->Int_unit, "ori", instructionRead);
            return 1;
        }
    }
    return 0;
}

int write(Scoreboarding *scoreboarding)
{
    Instruction instructionRead;
    if (instructionRead.operation == Move)
    {

        if (scoreboarding->Int_unit.busy == 1)
        {
            scoreboarding->Int_unit.busy = 0;
        }
    }
    else if (instructionRead.operation == Add)
    {

        if (scoreboarding->FP_Add.busy == 1)
        {
            scoreboarding->FP_Add.busy = 0;
        }
    }
    else if (instructionRead.operation == Sub)
    {

        if (scoreboarding->FP_Add.busy == 1)
        {
            scoreboarding->FP_Add.busy = 0;
        }
    }
    else if (instructionRead.operation == And)
    {

        if (scoreboarding->Int_unit.busy == 1)
        {
            scoreboarding->Int_unit.busy = 0;
        }
    }
    else if (instructionRead.operation == Or)
    {

        if (scoreboarding->Int_unit.busy == 1)
        {
            scoreboarding->Int_unit.busy = 0;
        }
    }
    else if (instructionRead.operation == Slt)
    {

        if (scoreboarding->Int_unit.busy == 1)
        {
            scoreboarding->Int_unit.busy = 0;
        }
    }
    else if (instructionRead.operation == Mult)
    {

        if (scoreboarding->FP_Mult1.busy == 1)
        {
            scoreboarding->FP_Mult1.busy = 0;
        }

        else if (scoreboarding->FP_Mult2.busy == 1)
        {
            scoreboarding->FP_Mult2.busy = 0
        }
    }
    else if (instructionRead.operation == Div)
    {

        if (scoreboarding->FP_Div.busy == 1)
        {
            scoreboarding->FP_Div.busy = 0;
        }
    }
    else if (instructionRead.operation == Li)
    {

        if (scoreboarding->Int_unit.busy == 1)
        {
            scoreboarding->Int_unit.busy = 0;
        }
    }
    else if (instructionRead.operation == Addi)
    {

        if (scoreboarding->FP_Add.busy == 1)
        {
            scoreboarding->FP_Add.busy = 0;
        }
    }
    else if (instructionRead.operation == Andi)
    {

        if (scoreboarding->Int_unit.busy == 1)
        {
            scoreboarding->Int_unit.busy = 0;
        }
    }
    else if (instructionRead.operation == Ori)
    {

        if (scoreboarding->Int_unit.busy == 1)
        {
            scoreboarding->Int_unit.busy = 0;
        }
    }
}

int scoreboarding(InstConfig instructionConfig, unsigned int instructionsMemory[], int memoryLength)
{
    int clock = 0;
    Instruction instructionRead;
    int i = 0;
    Scoreboarding scoreboarding;

    while ()
    {
        write();
        execute();
        read();
        issue(instructionsMemory[i], &scoreboarding, scoreboarding.registerTable);
    }
}