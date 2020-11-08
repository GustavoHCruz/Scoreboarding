#include <stdio.h>
#include <math.h>

int clock = 0;
int pc = 0;
RegisterMemory registerMemory[32];
Scoreboarding scoreboarding;
Instruction *instructions = NULL;
int waitingRead[5];

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
    temp.FU_name = NILL;
    temp.pipeline.issue = 0;
    temp.pipeline.issueCheck = 0;
    temp.pipeline.read = 0;
    temp.pipeline.readCheck = -1;
    temp.pipeline.execute = 0;
    temp.pipeline.executeCheck = -1;
    temp.pipeline.write = 0;
    temp.pipeline.writeCheck = -1;
    if (inst > 67108863)
    {
        temp.type = I;
        temp.operand3 = convertTo(&inst, 16);
        temp.operand2 = convertTo(&inst, 5);
        temp.operand1 = convertTo(&inst, 5);
        temp.operation = convertTo(&inst, 6);
    }
    else
    {
        temp.type = R;
        temp.operation = convertTo(&inst, 6);
        convertTo(&inst, 5);
        temp.operand1 = convertTo(&inst, 5);
        temp.operand3 = convertTo(&inst, 5);
        temp.operand2 = convertTo(&inst, 5);
    }
    return temp;
}

void initialize()
{
    scoreboarding.FP_Mult1.name = MULT1;
    scoreboarding.FP_Mult1.busy = false;
    scoreboarding.FP_Mult1.operation = '\0';
    scoreboarding.FP_Mult1.fi = 0;
    scoreboarding.FP_Mult1.fj = 0;
    scoreboarding.FP_Mult1.fk = 0;
    scoreboarding.FP_Mult1.qj = 0;
    scoreboarding.FP_Mult1.qk = 0;
    scoreboarding.FP_Mult1.rj = 0;
    scoreboarding.FP_Mult1.rk = 0;
    scoreboarding.FP_Mult2.name = MULT2;
    scoreboarding.FP_Mult2.busy = false;
    scoreboarding.FP_Mult2.operation = '\0';
    scoreboarding.FP_Mult2.fi = 0;
    scoreboarding.FP_Mult2.fj = 0;
    scoreboarding.FP_Mult2.fk = 0;
    scoreboarding.FP_Mult2.qj = 0;
    scoreboarding.FP_Mult2.qk = 0;
    scoreboarding.FP_Mult2.rj = 0;
    scoreboarding.FP_Mult2.rk = 0;
    scoreboarding.FP_Div.name = DIV;
    scoreboarding.FP_Div.busy = false;
    scoreboarding.FP_Div.operation = '\0';
    scoreboarding.FP_Div.fi = 0;
    scoreboarding.FP_Div.fj = 0;
    scoreboarding.FP_Div.fk = 0;
    scoreboarding.FP_Div.qj = 0;
    scoreboarding.FP_Div.qk = 0;
    scoreboarding.FP_Div.rj = 0;
    scoreboarding.FP_Div.rk = 0;
    scoreboarding.FP_Add.name = ADD;
    scoreboarding.FP_Add.busy = false;
    scoreboarding.FP_Add.operation = '\0';
    scoreboarding.FP_Add.fi = 0;
    scoreboarding.FP_Add.fj = 0;
    scoreboarding.FP_Add.fk = 0;
    scoreboarding.FP_Add.qj = 0;
    scoreboarding.FP_Add.qk = 0;
    scoreboarding.FP_Add.rj = 0;
    scoreboarding.FP_Add.rk = 0;
    scoreboarding.Int_unit.name = INT;
    scoreboarding.Int_unit.busy = false;
    scoreboarding.Int_unit.operation = '\0';
    scoreboarding.Int_unit.fi = 0;
    scoreboarding.Int_unit.fj = 0;
    scoreboarding.Int_unit.fk = 0;
    scoreboarding.Int_unit.qj = 0;
    scoreboarding.Int_unit.qk = 0;
    scoreboarding.Int_unit.rj = 0;
    scoreboarding.Int_unit.rk = 0;
    for (size_t i = 0; i < 32; i++)
    {
        registerMemory->FU = NILL;
        registerMemory->value = NILL;
    }
}

bool rawCheck(FunctionUnity functionUnity, unsigned int operand)
{
    if (functionUnity.fi == operand)
        return true;
    return false;
}

bool warCheck(FunctionUnity functinoUnity, unsigned int operand)
{
    if ((functinoUnity.fj == operand && functinoUnity.fj != 0) || (functinoUnity.fk == operand && functinoUnity.fk != 0))
        return true;
    return false;
}

bool wawCheck(FunctionUnity functionUnity, unsigned int operand)
{
    if (functionUnity.fi == operand)
        return true;
    return false;
}

bool rawDependency()
{
    if (instructions[pc].operation != Li)
    {
        return (rawCheck(scoreboarding.FP_Add, instructions[pc].operand2));
        return (rawCheck(scoreboarding.FP_Div, instructions[pc].operand2));
        return (rawCheck(scoreboarding.FP_Mult1, instructions[pc].operand2));
        return (rawCheck(scoreboarding.FP_Mult2, instructions[pc].operand2));
        return (rawCheck(scoreboarding.Int_unit, instructions[pc].operand2));

        if (instructions[pc].type == R)
        {
            return (rawCheck(scoreboarding.FP_Add, instructions[pc].operand3));
            return (rawCheck(scoreboarding.FP_Div, instructions[pc].operand3));
            return (rawCheck(scoreboarding.FP_Mult1, instructions[pc].operand3));
            return (rawCheck(scoreboarding.FP_Mult2, instructions[pc].operand3));
            return (rawCheck(scoreboarding.Int_unit, instructions[pc].operand3));
        }
    }
    return false;
}

bool warDependency()
{
    if (instructions[pc].operation != Li)
    {
        return (warCheck(scoreboarding.FP_Add, instructions[pc].operand1));
        return (warCheck(scoreboarding.FP_Div, instructions[pc].operand1));
        return (warCheck(scoreboarding.FP_Mult1, instructions[pc].operand1));
        return (warCheck(scoreboarding.FP_Mult2, instructions[pc].operand1));
        return (warCheck(scoreboarding.Int_unit, instructions[pc].operand1));
    }
    else
    {
        return (warCheck(scoreboarding.Int_unit, instructions[pc].operand2));
        return (warCheck(scoreboarding.FP_Add, instructions[pc].operand2));
        return (warCheck(scoreboarding.FP_Div, instructions[pc].operand2));
        return (warCheck(scoreboarding.FP_Mult1, instructions[pc].operand2));
        return (warCheck(scoreboarding.FP_Mult2, instructions[pc].operand2));
        return (warCheck(scoreboarding.Int_unit, instructions[pc].operand2));
    }
    return false;
}

bool wawDependency()
{
    if (instructions[pc].operation != Li)
    {
        return (wawCheck(scoreboarding.FP_Add, instructions[pc].operand1));
        return (wawCheck(scoreboarding.FP_Div, instructions[pc].operand1));
        return (wawCheck(scoreboarding.FP_Mult1, instructions[pc].operand1));
        return (wawCheck(scoreboarding.FP_Mult2, instructions[pc].operand1));
        return (wawCheck(scoreboarding.Int_unit, instructions[pc].operand1));
    }
    else
    {
        return (wawCheck(scoreboarding.Int_unit, instructions[pc].operand2));
        return (wawCheck(scoreboarding.FP_Add, instructions[pc].operand2));
        return (wawCheck(scoreboarding.FP_Div, instructions[pc].operand2));
        return (wawCheck(scoreboarding.FP_Mult1, instructions[pc].operand2));
        return (wawCheck(scoreboarding.FP_Mult2, instructions[pc].operand2));
        return (wawCheck(scoreboarding.Int_unit, instructions[pc].operand2));
    }
    return false;
}

bool update_issue(FunctionUnity *functionUnity, char *nameOperation)
{
    instructions[pc].pipeline.issue = clock;

    functionUnity->operation = nameOperation;

    functionUnity->busy = true;

    if (registerMemory[instructions[pc].operand2].FU == NILL)
    {
        functionUnity->rj = true;
        functionUnity->qj = functionUnity->name;
    }
    else
    {
        functionUnity->rj = false;
    }

    if (instructions[pc].type == R)
    {
        registerMemory[instructions[pc].operand1].FU = functionUnity->name;
        if (registerMemory[instructions[pc].operand3].FU == NILL)
        {
            functionUnity->rk = true;
            functionUnity->qk = functionUnity->name;
        }
        else
        {
            functionUnity->rk = false;
        }

        if (instructions[pc].operation == Move)
        {
            functionUnity->fi = instructions[pc].operand1;
            functionUnity->fj = instructions[pc].operand2;
            functionUnity->fk = 0;
        }
        else
        {
            functionUnity->fi = instructions[pc].operand1;
            functionUnity->fj = instructions[pc].operand2;
            functionUnity->fk = instructions[pc].operand3;
        }
    }
    else
    {
        if (instructions[pc].operation == Li)
        {
            registerMemory[instructions[pc].operand2].FU = functionUnity->name;
            functionUnity->fi = instructions[pc].operand2;
            functionUnity->fj = 0;
            functionUnity->fk = 0;
        }
        else
        {
            registerMemory[instructions[pc].operand1].FU = functionUnity->name;
            functionUnity->fi = instructions[pc].operand1;
            functionUnity->fj = instructions[pc].operand2;
            functionUnity->fk = 0;
        }
    }
    instructions[pc].pipeline.issueCheck = 1;
    instructions[pc].pipeline.readCheck = 0;
    instructions[pc].FU_name = functionUnity->name;
    return true;
}

bool issue()
{
    if (!wawDependency() && instructions[pc].pipeline.issueCheck == 0)
    {
        if (instructions[pc].operation == Move)
        {
            if (!scoreboarding.Int_unit.busy)
                return (update_issue(&scoreboarding.Int_unit, "move"));
        }
        else if (instructions[pc].operation == Add)
        {
            if (!scoreboarding.FP_Add.busy)
                return (update_issue(&scoreboarding.FP_Add, "add"));
        }
        else if (instructions[pc].operation == Sub)
        {
            if (!scoreboarding.FP_Add.busy)
                return (update_issue(&scoreboarding.FP_Add, "sub"));
        }
        else if (instructions[pc].operation == And)
        {
            if (!scoreboarding.Int_unit.busy)
                return (update_issue(&scoreboarding.Int_unit, "and"));
        }
        else if (instructions[pc].operation == Or)
        {
            if (!scoreboarding.Int_unit.busy)
                return (update_issue(&scoreboarding.Int_unit, "or"));
        }
        else if (instructions[pc].operation == Slt)
        {
            if (!scoreboarding.Int_unit.busy)
                return (update_issue(&scoreboarding.Int_unit, "slt"));
        }
        else if (instructions[pc].operation == Mult)
        {
            if (!scoreboarding.FP_Mult1.busy)
                return (update_issue(&scoreboarding.FP_Mult1, "mult"));
            else if (!scoreboarding.FP_Mult2.busy)
                return (update_issue(&scoreboarding.FP_Mult2, "mult"));
        }
        else if (instructions[pc].operation == Div)
        {
            if (!scoreboarding.FP_Div.busy)
                return (update_issue(&scoreboarding.FP_Div, "div"));
        }
        else if (instructions[pc].operation == Li)
        {
            if (!scoreboarding.Int_unit.busy)
                return (update_issue(&scoreboarding.Int_unit, "li"));
        }
        else if (instructions[pc].operation == Addi)
        {
            if (!scoreboarding.FP_Add.busy)
                return (update_issue(&scoreboarding.FP_Add, "addi"));
        }
        else if (instructions[pc].operation == Andi)
        {
            if (!scoreboarding.Int_unit.busy)
                return (update_issue(&scoreboarding.Int_unit, "andi"));
        }
        else if (instructions[pc].operation == Ori)
        {
            if (!scoreboarding.Int_unit.busy)
                return (update_issue(&scoreboarding.Int_unit, "ori"));
        }
    }
    return false;
}

bool read()
{
    for (int i = 0; i < pc; i++)
    {
        if (!rawDependency() && instructions[i].pipeline.readCheck == 0)
        {
            instructions[i].pipeline.read = clock;
            instructions[i].pipeline.readCheck = 1;
            instructions[i].pipeline.executeCheck = 0;
        }
    }
}

void print(FILE *file)
{
    fprintf(file, "============================================================ CICLO %i ============================================================\n", clock);
    fprintf(file, "1) STATUS DAS INSTRUCOES\n");
    fprintf(file, "\tEmissao | Leitura de Operandos | Execucao | Escrita de Resultados\n");
    for (int i = 0; i < pc; i++)
    {
        fprintf(file, "I%i  %i       | %i                    | %i        | %i\n", i, instructions[i].pipeline.issue, instructions[i].pipeline.read, instructions[i].pipeline.execute, instructions[i].pipeline.write);
    }
    fprintf(file, "\n2) STATUS DAS UNIDADES FUNCIONAIS\n");
    fprintf(file, "UF   \t| Busy  |\tOp\t|\tFi\t|\tFj\t|\tFk\t|\tQj\t|\tQk\t|\tRj\t|\tRk\n");
    fprintf(file, "Mult1\t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.FP_Mult1.busy, scoreboarding.FP_Mult1.operation, scoreboarding.FP_Mult1.fi, scoreboarding.FP_Mult1.fj, scoreboarding.FP_Mult1.fk, scoreboarding.FP_Mult1.qj, scoreboarding.FP_Mult1.qk, scoreboarding.FP_Mult1.rj, scoreboarding.FP_Mult1.rk);
    fprintf(file, "Mult2\t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.FP_Mult2.busy, scoreboarding.FP_Mult2.operation, scoreboarding.FP_Mult2.fi, scoreboarding.FP_Mult2.fj, scoreboarding.FP_Mult2.fk, scoreboarding.FP_Mult2.qj, scoreboarding.FP_Mult2.qk, scoreboarding.FP_Mult2.rj, scoreboarding.FP_Mult2.rk);
    fprintf(file, "Add  \t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.FP_Add.busy, scoreboarding.FP_Add.operation, scoreboarding.FP_Add.fi, scoreboarding.FP_Add.fj, scoreboarding.FP_Add.fk, scoreboarding.FP_Add.qj, scoreboarding.FP_Add.qk, scoreboarding.FP_Add.rj, scoreboarding.FP_Add.rk);
    fprintf(file, "Div  \t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.FP_Div.busy, scoreboarding.FP_Div.operation, scoreboarding.FP_Div.fi, scoreboarding.FP_Div.fj, scoreboarding.FP_Div.fk, scoreboarding.FP_Div.qj, scoreboarding.FP_Div.qk, scoreboarding.FP_Div.rj, scoreboarding.FP_Div.rk);
    fprintf(file, "Log  \t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.Int_unit.busy, scoreboarding.Int_unit.operation, scoreboarding.Int_unit.fi, scoreboarding.Int_unit.fj, scoreboarding.Int_unit.fk, scoreboarding.Int_unit.qj, scoreboarding.Int_unit.qk, scoreboarding.Int_unit.rj, scoreboarding.Int_unit.rk);

    fprintf(file, "\n3) STATUS DOS REGISTRADORES\n");
    fprintf(file, "    t0  |  t1  |  t2  |  t3  |  t4  |  t5  |  t6  |  t7  |  s0  |  s1  |  s2  |  s3  |  s4  |  s5  |  s6  |  s7  |  t8  |  t9\n");
    fprintf(file, "UF ");
    for (int i = 8; i < 25; i++)
    {
        fprintf(file, "%i | ", registerMemory[i].FU);
    }
    fprintf(file, "%i\n", registerMemory[25].FU);

    fprintf(file, "\n4) BANCO DE REGISTRADORES\n");
    fprintf(file, "t0 | t1 | t2 | t3 | t4 | t5 | t6 | t7 | s0 | s1 | s2 | s3 | s4 | s5 | s6 | s7 | t8 | t9\n");
    for (int i = 8; i < 25; i++)
    {
        fprintf(file, "%i | ", registerMemory[i].value);
    }
    fprintf(file, "%i\n", registerMemory[25].value);
}

void scoreboardingFunction(InstConfig instructionConfig, unsigned int instructionsMemory[], int memoryLength, char *outputName)
{
    instructions = malloc(sizeof(Instruction) * memoryLength);

    initialize();
    FILE *output = fopen(outputName, "w+");
    while (pc < memoryLength)
    {
        clock++;
        // write();

        // execute();

        // read();
        instructions[pc] = readMemory(instructionsMemory[pc]);

        if (issue())
            pc++;

        print(output);
    }
    free(instructions);
    fclose(output);
}