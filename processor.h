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
        temp.operand3 = convertTo(&inst, 16); // Immediate
        temp.operand1 = convertTo(&inst, 5);  // Destination
        temp.operand2 = convertTo(&inst, 5);  // Origin 1
        temp.operation = convertTo(&inst, 6);
    }
    else
    {
        temp.type = R;
        temp.operation = convertTo(&inst, 6);
        convertTo(&inst, 5);
        temp.operand1 = convertTo(&inst, 5); // Destination
        temp.operand3 = convertTo(&inst, 5); // Origin 2
        temp.operand2 = convertTo(&inst, 5); // Origin 1
    }
    return temp;
}

void initialize()
{
    for (int i = 0; i < units_n; i++)
    {
        scoreboarding.FUs[i].name = i;
        scoreboarding.FUs[i].busy = false;
        scoreboarding.FUs[i].operation = '\0';
        scoreboarding.FUs[i].fi = 0;
        scoreboarding.FUs[i].fj = 0;
        scoreboarding.FUs[i].fk = 0;
        scoreboarding.FUs[i].qj = NILL;
        scoreboarding.FUs[i].qk = NILL;
        scoreboarding.FUs[i].rj = NILL;
        scoreboarding.FUs[i].rk = NILL;
    }
    for (size_t i = 0; i < 32; i++)
    {
        registerMemory[i].FU = NILL;
        registerMemory[i].value = NILL;
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
        if ((rawCheck(scoreboarding.FUs[MULT1], instructions[pc].operand2)) || (rawCheck(scoreboarding.FUs[MULT2], instructions[pc].operand2)) || (rawCheck(scoreboarding.FUs[DIV], instructions[pc].operand2)) || (rawCheck(scoreboarding.FUs[ADD], instructions[pc].operand2)) || (rawCheck(scoreboarding.FUs[LOG], instructions[pc].operand2)))
            return true;

        if (instructions[pc].type == R)
        {
            if ((rawCheck(scoreboarding.FUs[MULT1], instructions[pc].operand3)) || (rawCheck(scoreboarding.FUs[MULT2], instructions[pc].operand3)) || (rawCheck(scoreboarding.FUs[DIV], instructions[pc].operand3)) || (rawCheck(scoreboarding.FUs[ADD], instructions[pc].operand3)) || (rawCheck(scoreboarding.FUs[LOG], instructions[pc].operand3)))
                return true;
        }
    }
    return false;
}

bool warDependency()
{
    if ((warCheck(scoreboarding.FUs[MULT1], instructions[pc].operand1)) || (warCheck(scoreboarding.FUs[MULT2], instructions[pc].operand1)) || (warCheck(scoreboarding.FUs[DIV], instructions[pc].operand1)) || (warCheck(scoreboarding.FUs[ADD], instructions[pc].operand1)) || (warCheck(scoreboarding.FUs[LOG], instructions[pc].operand1)))
        return true;

    return false;
}

bool wawDependency()
{
    if ((wawCheck(scoreboarding.FUs[MULT1], instructions[pc].operand1)) || (wawCheck(scoreboarding.FUs[MULT2], instructions[pc].operand1)) || (wawCheck(scoreboarding.FUs[DIV], instructions[pc].operand1)) || (wawCheck(scoreboarding.FUs[ADD], instructions[pc].operand1)) || (wawCheck(scoreboarding.FUs[LOG], instructions[pc].operand1)))
        return true;

    return false;
}

bool update_issue(FunctionUnity *functionUnity, char *nameOperation)
{
    instructions[pc].pipeline.issue = clock;
    functionUnity->operation = nameOperation;
    functionUnity->busy = true;

    functionUnity->fi = instructions[pc].operand1;

    registerMemory[instructions[pc].operand1].FU = functionUnity->name;
    if (instructions[pc].type == I)
    {
        if (instructions[pc].operation == Li)
        {
            functionUnity->fj = 0;
            functionUnity->fk = 0;
            functionUnity->qj = NILL;
            functionUnity->qk = NILL;
            functionUnity->rj = NILL;
            functionUnity->rk = NILL;
        }
        else
        {
            functionUnity->fj = instructions[pc].operand2;
            functionUnity->fk = 0;
            functionUnity->qk = NILL;
            functionUnity->rk = NILL;
            if (registerMemory[instructions[pc].operand2].FU == NILL)
            {
                functionUnity->qj = NILL;
                functionUnity->rj = 1;
            }
            else
            {
                functionUnity->qj = registerMemory[instructions[pc].operand2].FU;
                functionUnity->rj = 0;
            }
        }
    }
    else
    {
        if (instructions[pc].operation == Move)
        {
            functionUnity->fj = instructions[pc].operand2;
            functionUnity->fk = 0;
            functionUnity->qk = NILL;
            functionUnity->rk = NILL;
            if (registerMemory[instructions[pc].operand2].FU == NILL)
            {
                functionUnity->qj = NILL;
                functionUnity->rj = 1;
            }
            else
            {
                functionUnity->qj = registerMemory[instructions[pc].operand2].FU;
                functionUnity->rj = 0;
            }
        }
        else
        {
            functionUnity->fj = instructions[pc].operand2;
            functionUnity->fk = instructions[pc].operand3;
            if (registerMemory[instructions[pc].operand2].FU == NILL)
            {
                functionUnity->qj = NILL;
                functionUnity->rj = 1;
            }
            else
            {
                functionUnity->qj = registerMemory[instructions[pc].operand2].FU;
                functionUnity->rj = 0;
            }
            if (registerMemory[instructions[pc].operand3].FU == NILL)
            {
                functionUnity->qk = NILL;
                functionUnity->rk = 1;
            }
            else
            {
                functionUnity->qk = registerMemory[instructions[pc].operand3].FU;
                functionUnity->rk = 0;
            }
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
            if (!scoreboarding.FUs[LOG].busy)
                return (update_issue(&scoreboarding.FUs[LOG], "move"));
        }
        else if (instructions[pc].operation == Add)
        {
            if (!scoreboarding.FUs[ADD].busy)
                return (update_issue(&scoreboarding.FUs[ADD], "add"));
        }
        else if (instructions[pc].operation == Sub)
        {
            if (!scoreboarding.FUs[ADD].busy)
                return (update_issue(&scoreboarding.FUs[ADD], "sub"));
        }
        else if (instructions[pc].operation == And)
        {
            if (!scoreboarding.FUs[LOG].busy)
                return (update_issue(&scoreboarding.FUs[LOG], "and"));
        }
        else if (instructions[pc].operation == Or)
        {
            if (!scoreboarding.FUs[LOG].busy)
                return (update_issue(&scoreboarding.FUs[LOG], "or"));
        }
        else if (instructions[pc].operation == Slt)
        {
            if (!scoreboarding.FUs[LOG].busy)
                return (update_issue(&scoreboarding.FUs[LOG], "slt"));
        }
        else if (instructions[pc].operation == Mult)
        {
            if (!scoreboarding.FUs[MULT1].busy)
                return (update_issue(&scoreboarding.FUs[MULT1], "mult"));
            else if (!scoreboarding.FUs[MULT2].busy)
                return (update_issue(&scoreboarding.FUs[MULT2], "mult"));
        }
        else if (instructions[pc].operation == Div)
        {
            if (!scoreboarding.FUs[DIV].busy)
                return (update_issue(&scoreboarding.FUs[DIV], "div"));
        }
        else if (instructions[pc].operation == Li)
        {
            if (!scoreboarding.FUs[LOG].busy)
                return (update_issue(&scoreboarding.FUs[LOG], "li"));
        }
        else if (instructions[pc].operation == Addi)
        {
            if (!scoreboarding.FUs[ADD].busy)
                return (update_issue(&scoreboarding.FUs[ADD], "addi"));
        }
        else if (instructions[pc].operation == Andi)
        {
            if (!scoreboarding.FUs[LOG].busy)
                return (update_issue(&scoreboarding.FUs[LOG], "andi"));
        }
        else if (instructions[pc].operation == Ori)
        {
            if (!scoreboarding.FUs[LOG].busy)
                return (update_issue(&scoreboarding.FUs[LOG], "ori"));
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
            scoreboarding.FUs[instructions[i].FU_name].qj = 0;
            scoreboarding.FUs[instructions[i].FU_name].qk = 0;
            scoreboarding.FUs[instructions[i].FU_name].rj = false;
            scoreboarding.FUs[instructions[i].FU_name].rk = false;
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
    fprintf(file, "Mult1\t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.FUs[MULT1].busy, scoreboarding.FUs[MULT1].operation, scoreboarding.FUs[MULT1].fi, scoreboarding.FUs[MULT1].fj, scoreboarding.FUs[MULT1].fk, scoreboarding.FUs[MULT1].qj, scoreboarding.FUs[MULT1].qk, scoreboarding.FUs[MULT1].rj, scoreboarding.FUs[MULT1].rk);
    fprintf(file, "Mult2\t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.FUs[MULT2].busy, scoreboarding.FUs[MULT2].operation, scoreboarding.FUs[MULT2].fi, scoreboarding.FUs[MULT2].fj, scoreboarding.FUs[MULT2].fk, scoreboarding.FUs[MULT2].qj, scoreboarding.FUs[MULT2].qk, scoreboarding.FUs[MULT2].rj, scoreboarding.FUs[MULT2].rk);
    fprintf(file, "Add  \t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.FUs[ADD].busy, scoreboarding.FUs[ADD].operation, scoreboarding.FUs[ADD].fi, scoreboarding.FUs[ADD].fj, scoreboarding.FUs[ADD].fk, scoreboarding.FUs[ADD].qj, scoreboarding.FUs[ADD].qk, scoreboarding.FUs[ADD].rj, scoreboarding.FUs[ADD].rk);
    fprintf(file, "Div  \t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.FUs[DIV].busy, scoreboarding.FUs[DIV].operation, scoreboarding.FUs[DIV].fi, scoreboarding.FUs[DIV].fj, scoreboarding.FUs[DIV].fk, scoreboarding.FUs[DIV].qj, scoreboarding.FUs[DIV].qk, scoreboarding.FUs[DIV].rj, scoreboarding.FUs[DIV].rk);
    fprintf(file, "Log  \t|\t%i\t|\t%s\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\t|\t%i\n", scoreboarding.FUs[LOG].busy, scoreboarding.FUs[LOG].operation, scoreboarding.FUs[LOG].fi, scoreboarding.FUs[LOG].fj, scoreboarding.FUs[LOG].fk, scoreboarding.FUs[LOG].qj, scoreboarding.FUs[LOG].qk, scoreboarding.FUs[LOG].rj, scoreboarding.FUs[LOG].rk);

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