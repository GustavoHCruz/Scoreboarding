int clock = 0;
int pc = 0;
RegisterMemory registerMemory[register_n];
Scoreboarding scoreboarding;
Instruction *instructions = NULL;
int executionBuffer[register_n];
InstConfig latencies;

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
    temp.pipeline.issue = NILL;
    temp.pipeline.issueCheck = 0;
    temp.pipeline.read = NILL;
    temp.pipeline.readCheck = -1;
    temp.pipeline.execute = NILL;
    temp.pipeline.executeCheck = -1;
    temp.pipeline.write = NILL;
    temp.pipeline.writeCheck = -1;
    if (inst > 67108863)
    {
        temp.type = I;
        temp.operand3 = convertTo(&inst, 16); // Immediate
        temp.operand1 = convertTo(&inst, 5);  // Destination or RT
        temp.operand2 = convertTo(&inst, 5);  // Origin 1 or RS
        temp.operation = convertTo(&inst, 6);
    }
    else
    {
        temp.type = R;
        temp.operation = convertTo(&inst, 6);
        convertTo(&inst, 5);
        temp.operand1 = convertTo(&inst, 5); // Destination or RD
        temp.operand3 = convertTo(&inst, 5); // Origin 2 or RT
        temp.operand2 = convertTo(&inst, 5); // Origin 1 or RS
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
        scoreboarding.FUs[i].clear = false;
        scoreboarding.FUs[i].instruction_n = NILL;
    }
    for (size_t i = 0; i < register_n; i++)
    {
        registerMemory[i].FU = NILL;
        registerMemory[i].value = NILL;
    }
    for (size_t i = 0; i < register_n; i++)
        executionBuffer[i] = NILL;
}

bool rawCheck(FunctionUnity functionUnity, unsigned int operand)
{
    return (functionUnity.fi == operand);
}

bool rawDependency(int i)
{
    if (instructions[i].operation != Li)
    {
        for (size_t j = 0; j < units_n; j++)
        {
            if (instructions[i].FU_name != scoreboarding.FUs[j].name)
            {
                if (rawCheck(scoreboarding.FUs[j], instructions[i].operand2))
                    if (scoreboarding.FUs[j].instruction_n != -1 && (i > scoreboarding.FUs[j].instruction_n))
                        return true;
            }
        }
        if (instructions[i].type == R)
        {
            for (size_t j = 0; j < units_n; j++)
            {
                if (instructions[i].FU_name != scoreboarding.FUs[j].name)
                {
                    if (rawCheck(scoreboarding.FUs[j], instructions[i].operand3))
                        if (scoreboarding.FUs[j].instruction_n != -1 && (i > scoreboarding.FUs[j].instruction_n))
                            return true;
                }
            }
        }
    }
    return false;
}

bool warCheck(FunctionUnity functinoUnity, unsigned int operand)
{
    if ((functinoUnity.fj == operand && functinoUnity.fj != 0 && functinoUnity.rj != 0) || (functinoUnity.fk == operand && functinoUnity.fk != 0 && functinoUnity.rk != 0))
        return true;
    return false;
}

bool wawCheck(FunctionUnity functionUnity, unsigned int operand)
{
    if (functionUnity.fi == operand)
        return true;
    return false;
}

bool warDependency(int i)
{
    if ((warCheck(scoreboarding.FUs[MULT1], instructions[i].operand1)) || (warCheck(scoreboarding.FUs[MULT2], instructions[i].operand1)) || (warCheck(scoreboarding.FUs[DIV], instructions[i].operand1)) || (warCheck(scoreboarding.FUs[ADD], instructions[i].operand1)) || (warCheck(scoreboarding.FUs[LOG], instructions[i].operand1)))
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
    functionUnity->fi = instructions[pc].operand1;
    registerMemory[instructions[pc].operand1].FU = functionUnity->name;

    instructions[pc].pipeline.issueCheck = 1;
    instructions[pc].pipeline.readCheck = 0;
    instructions[pc].FU_name = functionUnity->name;
    functionUnity->instruction_n = pc;
    return true;
}

bool issue()
{
    if (instructions[pc].pipeline.issueCheck == 0 && !wawDependency())
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

void read()
{
    for (int i = 0; i < pc; i++)
    {
        if (instructions[i].pipeline.readCheck == 0 && !rawDependency(i))
        {
            instructions[i].pipeline.read = clock;

            scoreboarding.FUs[instructions[i].FU_name].qj = NILL;
            scoreboarding.FUs[instructions[i].FU_name].qk = NILL;

            if (instructions[i].operation == Li)
            {
                scoreboarding.FUs[instructions[i].FU_name].rj = NILL;
                scoreboarding.FUs[instructions[i].FU_name].rk = NILL;
                instructions[i].value3 = instructions[i].operand3;
            }
            else
            {
                scoreboarding.FUs[instructions[i].FU_name].rj = 0;
                if (instructions[i].type == I)
                {
                    scoreboarding.FUs[instructions[i].FU_name].rk = NILL;
                    instructions[i].value2 = registerMemory[instructions[i].operand2].value;
                    instructions[i].value3 = instructions[i].operand3;
                }
                else
                {
                    scoreboarding.FUs[instructions[i].FU_name].rk = 0;
                    instructions[i].value2 = registerMemory[instructions[i].operand2].value;
                    instructions[i].value3 = registerMemory[instructions[i].operand3].value;
                }
            }
            instructions[i].pipeline.readCheck = 1;
            instructions[i].pipeline.executeCheck = 0;
        }
    }
}

int ula(int operand2, int operand3, unsigned int operation)
{
    if (operation == Move)
        return operand2;
    else if (operation == Add)
    {

        return (operand2 + operand3);
    }
    else if (operation == Sub)
        return operand2 - operand3;
    else if (operation == And)
        return (operand2 & operand3);
    else if (operation == Or)
        return (operand2 | operand3);
    else if (operation == Slt)
        return (operand2 < operand3);
    else if (operation == Mult)
        return (operand2 * operand3);
    else if (operation == Div)
        return (operand2 / operand3);
    else if (operation == Li)
        return operand3;
    else if (operation == Addi)
        return (operand2 + operand3);
    else if (operation == Andi)
        return (operand2 & operand3);
    else if (operation == Ori)
        return (operand2 | operand3);
}

void execute()
{
    for (int i = 0; i < pc; i++)
    {
        if (instructions[i].pipeline.executeCheck == 0 && clock == instructions[i].pipeline.read + latencies.configs[instructions[i].operation])
        {
            instructions[i].pipeline.execute = clock;

            executionBuffer[instructions[i].operand1] = ula(instructions[i].value2, instructions[i].value3, instructions[i].operation);

            instructions[i].pipeline.executeCheck = 1;
            instructions[i].pipeline.writeCheck = 0;
        }
    }
}

void write()
{
    for (int i = 0; i < pc; i++)
    {
        if (instructions[i].pipeline.writeCheck == 0 && !(warDependency(i)))
        {
            instructions[i].pipeline.write = clock;
            registerMemory[instructions[i].operand1].value = executionBuffer[instructions[i].operand1];

            for (int j = 0; j < units_n; j++)
            {
                if (scoreboarding.FUs[j].qj == instructions[i].FU_name)
                {
                    scoreboarding.FUs[j].rj = 1;
                    scoreboarding.FUs[j].qj = NILL;
                }
                if (scoreboarding.FUs[j].qk == instructions[i].FU_name)
                {
                    scoreboarding.FUs[j].rk = 1;
                    scoreboarding.FUs[j].qk = NILL;
                }
            }
            scoreboarding.FUs[instructions[i].FU_name].clear = true;

            registerMemory[instructions[i].operand1].FU = NILL;
            instructions[i].pipeline.writeCheck = 1;
        }
    }
}

void clearFU()
{
    for (size_t i = 0; i < units_n; i++)
    {
        if (scoreboarding.FUs[i].clear == true)
        {
            scoreboarding.FUs[i].busy = false;
            scoreboarding.FUs[i].operation = '\0';
            scoreboarding.FUs[i].fi = 0;
            scoreboarding.FUs[i].fj = 0;
            scoreboarding.FUs[i].fk = 0;
            scoreboarding.FUs[i].qj = NILL;
            scoreboarding.FUs[i].qk = NILL;
            scoreboarding.FUs[i].rj = NILL;
            scoreboarding.FUs[i].rk = NILL;
            scoreboarding.FUs[i].clear = false;
            scoreboarding.FUs[i].instruction_n = NILL;
        }
    }
}

void printRegisterValue(FILE *file, int i)
{
    if (i == zero)
        fprintf(file, "\t    \t|");
    else if (i == at)
        fprintf(file, "\t at \t|");
    else if (i == v0)
        fprintf(file, "\t v0 \t|");
    else if (i == v1)
        fprintf(file, "\t v1 \t|");
    else if (i == a0)
        fprintf(file, "\t a0 \t|");
    else if (i == a1)
        fprintf(file, "\t a1 \t|");
    else if (i == a2)
        fprintf(file, "\t a2 \t|");
    else if (i == a3)
        fprintf(file, "\t a3 \t|");
    else if (i == t0)
        fprintf(file, "\t t0 \t|");
    else if (i == t1)
        fprintf(file, "\t t1 \t|");
    else if (i == t2)
        fprintf(file, "\t t2 \t|");
    else if (i == t3)
        fprintf(file, "\t t3 \t|");
    else if (i == t4)
        fprintf(file, "\t t4 \t|");
    else if (i == t5)
        fprintf(file, "\t t5 \t|");
    else if (i == t6)
        fprintf(file, "\t t6 \t|");
    else if (i == s0)
        fprintf(file, "\t s0 \t|");
    else if (i == s1)
        fprintf(file, "\t s1 \t|");
    else if (i == s2)
        fprintf(file, "\t s2 \t|");
    else if (i == s3)
        fprintf(file, "\t s3 \t|");
    else if (i == s4)
        fprintf(file, "\t s4 \t|");
    else if (i == s5)
        fprintf(file, "\t s5 \t|");
    else if (i == s6)
        fprintf(file, "\t s6 \t|");
    else if (i == s7)
        fprintf(file, "\t s7 \t|");
    else if (i == t8)
        fprintf(file, "\t t8 \t|");
    else if (i == t9)
        fprintf(file, "\t t9 \t|");
    else if (i == k0)
        fprintf(file, "\t k0 \t|");
    else if (i == k1)
        fprintf(file, "\t k1 \t|");
    else if (i == gp)
        fprintf(file, "\t gp \t|");
    else if (i == sp)
        fprintf(file, "\t sp \t|");
    else if (i == fp)
        fprintf(file, "\t fp \t|");
    else if (i == ra)
        fprintf(file, "\t ra \t|");
    else if (i == at)
        fprintf(file, "\t at \t|");
}

void printRegisterFU(FILE *file, int i)
{
    if (i == NILL)
        fprintf(file, "\t   \t|");
    else if (i == 0)
        fprintf(file, " mult1 |");
    else if (i == 1)
        fprintf(file, " mult2 |");
    else if (i == 2)
        fprintf(file, "\tdiv\t|");
    else if (i == 3)
        fprintf(file, "\tadd\t|");
    else if (i == 4)
        fprintf(file, "\tlog\t|");
}

void printInstructionFU(FILE *file, int i)
{
    if (i == NILL)
        fprintf(file, "\t\t   \t|");
    else if (i == 0)
        fprintf(file, "\t mult1  |");
    else if (i == 1)
        fprintf(file, "\t mult2  |");
    else if (i == 2)
        fprintf(file, "\tdiv\t\t|");
    else if (i == 3)
        fprintf(file, "\tadd\t\t|");
    else if (i == 4)
        fprintf(file, "\tlog\t\t|");
}

void printFUs(FILE *file, char *FU_name, int FU)
{
    fprintf(file, "%s \t|", FU_name);
    if (scoreboarding.FUs[FU].busy)
        fprintf(file, "\tsim \t|");
    else
        fprintf(file, "\tnao \t|");
    if (scoreboarding.FUs[FU].operation == NULL)
        fprintf(file, "\t    \t|");
    else
        fprintf(file, "\t %s  \t|", scoreboarding.FUs[FU].operation);
    printRegisterValue(file, scoreboarding.FUs[FU].fi);
    printRegisterValue(file, scoreboarding.FUs[FU].fj);
    printRegisterValue(file, scoreboarding.FUs[FU].fk);

    printInstructionFU(file, scoreboarding.FUs[FU].qj);
    printInstructionFU(file, scoreboarding.FUs[FU].qk);

    if (scoreboarding.FUs[FU].rj == NILL)
        fprintf(file, "\t    \t|");
    else if (scoreboarding.FUs[FU].rj == 0)
        fprintf(file, "\tnao \t|");
    else
        fprintf(file, "\tsim \t|");
    if (scoreboarding.FUs[FU].rk == NILL)
        fprintf(file, "\t    \n");
    else if (scoreboarding.FUs[FU].rk == 0)
        fprintf(file, "\tnao\n");
    else
        fprintf(file, "\tsim\n");
}

void print(FILE *file)
{
    fprintf(file, "============================================================ ciclo %i ============================================================\n", clock);
    fprintf(file, "1) status das instrucoes\n");
    fprintf(file, "\temissao\t|\tleitura de operandos\t|\texecucao\t|\tescrita de resultados\n");
    for (int i = 0; i < pc; i++)
    {
        fprintf(file, "i%i\t", i + 1);
        if (instructions[i].pipeline.issue != NILL)
            fprintf(file, "%i \t\t|\t", instructions[i].pipeline.issue);
        else
            fprintf(file, "  \t\t|\t");
        if (instructions[i].pipeline.read != NILL)
            fprintf(file, "%i               \t\t|\t", instructions[i].pipeline.read);
        else
            fprintf(file, "                \t\t|\t");
        if (instructions[i].pipeline.execute != NILL)
            fprintf(file, "%i   \t\t|\t", instructions[i].pipeline.execute);
        else
            fprintf(file, "    \t\t|\t");
        if (instructions[i].pipeline.write != NILL)
            fprintf(file, "%i\n", instructions[i].pipeline.write);
        else
            fprintf(file, " \n");
    }
    fprintf(file, "\n2) status das unidades funcionais\n");
    fprintf(file, "uf   \t|\tbusy\t|\t op \t|\t fi \t|\t fj \t|\t fk \t|\t qj \t|\t qk \t|\t rj \t|\t rk \n");
    printFUs(file, "mult1", MULT1);
    printFUs(file, "mult2", MULT2);
    printFUs(file, "add", ADD);
    printFUs(file, "div", DIV);
    printFUs(file, "log", LOG);

    fprintf(file, "\n3) status dos registradores\n");
    fprintf(file, "   t0\t|\tt1\t|\tt2\t|\tt3\t|\tt4\t|\tt5\t|\tt6\t|\tt7\t|\ts0\t|\ts1\t|\ts2\t|\ts3\t|\ts4\t|\ts5\t|\ts6\t|\ts7\t|\tt8\t|\tt9\t|\n");
    fprintf(file, "uf ");
    for (int i = 8; i < 25; i++)
    {
        printRegisterFU(file, registerMemory[i].FU);
    }
    printRegisterFU(file, registerMemory[25].FU);
    fprintf(file, "\n");

    fprintf(file, "\n4) banco de registradores\n");
    fprintf(file, "t0\t|\tt1\t|\tt2\t|\tt3\t|\tt4\t|\tt5\t|\tt6\t|\tt7\t|\ts0\t|\ts1\t|\ts2\t|\ts3\t|\ts4\t|\ts5\t|\ts6\t|\ts7\t|\tt8\t|\tt9\t|\n");
    for (int i = 8; i < 25; i++)
    {
        if (registerMemory[i].value == NILL)
            fprintf(file, "  \t|\t");
        else
            fprintf(file, "%i\t|\t", registerMemory[i].value);
    }
    if (registerMemory[25].value == NILL)
        fprintf(file, "  \t|\n");
    else
        fprintf(file, "%i\t|\n", registerMemory[25].value);
}

bool end(int n)
{
    for (int i = 0; i < n; i++)
    {
        if (instructions[i].pipeline.writeCheck != 1)
            return true;
    }
    return false;
}

void scoreboardingFunction(InstConfig instructionConfig, unsigned int instructionsMemory[], int memoryLength, char *outputName)
{
    instructions = malloc(sizeof(Instruction) * memoryLength);
    latencies = instructionConfig;

    initialize();
    FILE *output = fopen(outputName, "w+");
    while (end(memoryLength))
    {
        clock++;
        write();

        execute();

        read();

        if (pc < memoryLength)
        {
            instructions[pc] = readMemory(instructionsMemory[pc]);
            if (issue())
                pc++;
        }
        clearFU();
        print(output);
    }
    free(instructions);
    fclose(output);
}