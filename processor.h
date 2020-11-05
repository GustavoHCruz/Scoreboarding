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
        temp.operand3 = convertTo(&inst, 16);   //IMMEDIATE c
        temp.operand2 = convertTo(&inst, 5);    //RT -> DESTINO
        temp.operand1 = convertTo(&inst, 5);    //RS -> ORIGIN REGISTER SE FOR DE 3 INSTRUCAO
        temp.operation = convertTo(&inst, 6);   //OPCODE
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

void initializeScoreboarding(Scoreboarding *scorebording)
{
    FunctionUnity aux;
    aux.busy = 0;
    aux.fi = 0;
    aux.fj = 0;
    aux.fk = 0;
    aux.name = "";
    aux.operation = "";
    aux.qj = 0;
    aux.qk = 0;
    aux.rj = 0;
    aux.rk = 0;
    scorebording->FP_Add = aux;
    scorebording->FP_Add.name = "Add";
    scorebording->FP_Div = aux;
    scorebording->FP_Div.name = "Div";
    scorebording->FP_Mult1 = aux;
    scorebording->FP_Mult1.name = "Mult1";
    scorebording->FP_Mult2 = aux;
    scorebording->FP_Mult2.name = "Mult2";
    scorebording->Int_unit = aux;
    scorebording->Int_unit.name = "Int";
}

int rawCheck(FunctionUnity functionUnity, unsigned int operand)
{
    if (functionUnity.fi == operand)
        return 1;
    return 0;
}

int warCheck()
{
}

int wawCheck()
{
}

int rawDependency(Scoreboarding scoreboarding, Instruction instruction)
{
    if(instruction.operation != Li){    
        if (rawCheck(scoreboarding.FP_Add, instruction.operand2))
            return 1;
        if (rawCheck(scoreboarding.FP_Div, instruction.operand2))
            return 1;
        if (rawCheck(scoreboarding.FP_Mult1, instruction.operand2))
            return 1;
        if (rawCheck(scoreboarding.FP_Mult2, instruction.operand2))
            return 1;
        if (rawCheck(scoreboarding.Int_unit, instruction.operand2))
            return 1;
        if(instruction.type == R) {
            if (rawCheck(scoreboarding.FP_Add, instruction.operand3))
                return 1;
            if (rawCheck(scoreboarding.FP_Div, instruction.operand3))
                return 1;
            if (rawCheck(scoreboarding.FP_Mult1, instruction.operand3))
                return 1;
            if (rawCheck(scoreboarding.FP_Mult2, instruction.operand3))
                return 1;
            if (rawCheck(scoreboarding.Int_unit, instruction.operand3))
            return 1;
        }
    }
    return 0;
}

// int warDependecy(Scoreboarding scoreboarding, Instruction instruction)
// {

// }

// int wawDependency(Scoreboarding scoreboarding, Instruction instruction)
// {

// }

void initRegisterTable(Scoreboarding *scoreboarding) {
    for (int i = 0; i < 32; i ++) {
        scoreboarding->registerTable[i] = " ";
    }
}

int update_data_in_functional_units(Scoreboarding *scoreboarding, FunctionUnity *functionUnity, char *nameOperation, Instruction instruction)
{
    if (!rawDependency(*scoreboarding, instruction))
    {
        scoreboarding->registerTable[instruction.operand1] = functionUnity->name;
        functionUnity->operation = nameOperation;

        printf("%s", functionUnity->operation);
        functionUnity->busy = 1;
        if (instruction.type == R)
        {
            if(instruction.operation == Move){
                functionUnity->fi = instruction.operand1;
                functionUnity->fj = instruction.operand2;
                functionUnity->fk = 0;
            } else {
                functionUnity->fi = instruction.operand1;
                functionUnity->fj = instruction.operand2;
                functionUnity->fk = instruction.operand3;
            }
        }
        else
        {
            if(instruction.operation == Li)  {
                functionUnity->fi = instruction.operand2;
                functionUnity->fj = 0;
                functionUnity->fk = 0;
            } else {
                functionUnity->fi = instruction.operand1;
                functionUnity->fj = instruction.operand2;
                functionUnity->fk = 0;
            }
        }
        return 1;
    }
    return 0;
}

int issue(Instruction instructionsMemory, Scoreboarding *scoreboarding, char *registerMemory[])
{

    if (instructionsMemory.operation == Move)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->Int_unit, "move", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Add)
    {
        if (scoreboarding->FP_Add.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->FP_Add, "add", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Sub)
    {
        if (scoreboarding->FP_Add.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->FP_Add, "sub", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == And)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->Int_unit, "and", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Or)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->Int_unit, "or", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Slt)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->Int_unit, "slt", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Mult)
    {

        if (scoreboarding->FP_Mult1.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->FP_Mult1, "mult", instructionsMemory)){

                return 1;
            }
        }
        else if (scoreboarding->FP_Mult2.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->FP_Mult2, "mult", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Div)
    {
        if (scoreboarding->FP_Div.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->FP_Div, "div", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Li)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->Int_unit, "li", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Addi)
    {
        if (scoreboarding->FP_Add.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->FP_Add, "addi", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Andi)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->Int_unit, "andi", instructionsMemory))
                return 1;
        }
    }
    else if (instructionsMemory.operation == Ori)
    {
        if (scoreboarding->Int_unit.busy == 0)
        {
            if (update_data_in_functional_units(scoreboarding, &scoreboarding->Int_unit, "ori", instructionsMemory))
                return 1;
        }
    }
    return 0;
}

// int write(Scoreboarding *scoreboarding)
// {
//     Instruction instructionsMemory;
//     if (instructionsMemory.operation == Move)
//     {

//         if (scoreboarding->Int_unit.busy == 1)
//         {
//             scoreboarding->Int_unit.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Add)
//     {

//         if (scoreboarding->FP_Add.busy == 1)
//         {
//             scoreboarding->FP_Add.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Sub)
//     {

//         if (scoreboarding->FP_Add.busy == 1)
//         {
//             scoreboarding->FP_Add.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == And)
//     {

//         if (scoreboarding->Int_unit.busy == 1)
//         {
//             scoreboarding->Int_unit.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Or)
//     {

//         if (scoreboarding->Int_unit.busy == 1)
//         {
//             scoreboarding->Int_unit.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Slt)
//     {

//         if (scoreboarding->Int_unit.busy == 1)
//         {
//             scoreboarding->Int_unit.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Mult)
//     {

//         if (scoreboarding->FP_Mult1.busy == 1)
//         {
//             scoreboarding->FP_Mult1.busy = 0;
//         }

//         else if (scoreboarding->FP_Mult2.busy == 1)
//         {
//             scoreboarding->FP_Mult2.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Div)
//     {

//         if (scoreboarding->FP_Div.busy == 1)
//         {
//             scoreboarding->FP_Div.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Li)
//     {

//         if (scoreboarding->Int_unit.busy == 1)
//         {
//             scoreboarding->Int_unit.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Addi)
//     {

//         if (scoreboarding->FP_Add.busy == 1)
//         {
//             scoreboarding->FP_Add.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Andi)
//     {

//         if (scoreboarding->Int_unit.busy == 1)
//         {
//             scoreboarding->Int_unit.busy = 0;
//         }
//     }
//     else if (instructionsMemory.operation == Ori)
//     {

//         if (scoreboarding->Int_unit.busy == 1)
//         {
//             scoreboarding->Int_unit.busy = 0;
//         }
//     }
// }

int scoreboardingFunction(InstConfig instructionConfig, unsigned int instructionsMemory[], int memoryLength)
{
    int clock = 0, pc = 0;
    Instruction instructionRead;
    Scoreboarding scoreboarding;
    initializeScoreboarding(&scoreboarding);
    initRegisterTable(&scoreboarding);
    int i = 0;
    while (i < 5)
    {
        // clock++;
        // write();
        instructionRead = readMemory(instructionsMemory[pc]);
        // printf("%i\n", instructionRead.operation);
        // execute();
        // read();
        
        printf("AOPAJKSPOFAPSODFJASPODPAK\n\n\n\n\n\n");
        if (issue(instructionRead, &scoreboarding, scoreboarding.registerTable))
                pc++;

        printf("FP_Mult1\n%i\n%s\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%s\n", scoreboarding.FP_Mult1.busy, scoreboarding.FP_Mult1.operation, scoreboarding.FP_Mult1.fi, scoreboarding.FP_Mult1.fj,
            scoreboarding.FP_Mult1.fk, scoreboarding.FP_Mult1.qj, scoreboarding.FP_Mult1.qk, scoreboarding.FP_Mult1.rj, scoreboarding.FP_Mult1.rk, scoreboarding.FP_Mult1.name);
        printf("FP_Mult2\n%i\n%s\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%s\n", scoreboarding.FP_Mult2.busy,scoreboarding.FP_Mult2.operation, scoreboarding.FP_Mult2.fi, scoreboarding.FP_Mult2.fj,
            scoreboarding.FP_Mult2.fk,scoreboarding.FP_Mult2.qj, scoreboarding.FP_Mult2.qk, scoreboarding.FP_Mult2.rj, scoreboarding.FP_Mult2.rk, scoreboarding.FP_Mult2.name);
        printf("FP_Div%i\n%s\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%s\n", scoreboarding.FP_Div.busy,scoreboarding.FP_Div.operation, scoreboarding.FP_Div.fi, scoreboarding.FP_Div.fj,
            scoreboarding.FP_Div.fk,scoreboarding.FP_Div.qj, scoreboarding.FP_Div.qk, scoreboarding.FP_Div.rj, scoreboarding.FP_Div.rk, scoreboarding.FP_Div.name);
        printf("FP_Add%i\n%s\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%s\n", scoreboarding.FP_Add.busy,scoreboarding.FP_Add.operation, scoreboarding.FP_Add.fi, scoreboarding.FP_Add.fj,
            scoreboarding.FP_Add.fk,scoreboarding.FP_Add.qj, scoreboarding.FP_Add.qk, scoreboarding.FP_Add.rj, scoreboarding.FP_Add.rk, scoreboarding.FP_Add.name);
        printf("Int_unit%i\n%s\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%s\n", scoreboarding.Int_unit.busy,scoreboarding.Int_unit.operation, scoreboarding.Int_unit.fi, scoreboarding.Int_unit.fj,
            scoreboarding.Int_unit.fk ,scoreboarding.Int_unit.qj, scoreboarding.Int_unit.qk, scoreboarding.Int_unit.rj, scoreboarding.Int_unit.rk, scoreboarding.Int_unit.name);
        i++;
    }
}