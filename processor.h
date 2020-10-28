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


Intruction issue(unsigned int instructionsMemory, Scoreboarding *scoreboarding) {
    Instruction instructionRead;
    instrcutionRead = readMemory(instructionsMemory);
    if(instrcutionRead.operation == Move) {

        if(scoreboarding.Int_unit.busy == 0){
            scoreboarding.Int_unit.busy = 1;
            return 1;
        }
               
    } else if(instructionRead.operation == Add){

        if(scoreboarding.FP_Add.busy == 0){
            scoreboarding.FP_Add.busy = 1;
            return 1;
        }
        
            
    } else if(instructionRead.operation == Sub){

        if(scoreboarding.FP_Add.busy == 0){
            scoreboarding.FP_Add.busy = 1;
            return 1;
        }
        
            
    } else if(instrcutionRead.operation == And) {

        if(scoreboarding.Int_unit.busy == 0) {
            scoreboarding.Int_unit.busy = 1;
            return 1;
        }
        
    } else if(instrcutionRead.operation == Or) {

        if(scoreboarding.Int_unit.busy == 0) {
            scoreboarding.Int_unit.busy = 1;
            return 1;
        }
        
    } else if(instrcutionRead.operation == Slt) {
        
        if(scoreboarding.Int_unit.busy == 0) {
            scoreboarding.Int_unit.busy = 1;
            return 1;
        }

    } else if(instrcutionRead.operation == Mult) {
        
        if(scoreboarding.FP_Mult1.busy == 0){
            scoreboarding.FP_Mult1.busy = 1;
            return 1;
        }

            
        else if(scoreboarding.FP_Mult2.busy == 0){
            scoreboarding.FP_Mult2.busy = 1;
            return 1;
        }
        
            
    } else if(instrcutionRead.operation == Div) {

        if(scoreboarding.FP_Div.busy == 0) {
            scoreboarding.FP_Div.busy = 1;
            return 1;
        }

            
    } else if(instrcutionRead.operation == Li) {

        if(scoreboarding.Int_unit.busy == 0) {
            scoreboarding.Int_unit.busy = 1;
            return 1;
        }
        
    } else if(instrcutionRead.operation == Addi) {

        if(scoreboarding.FP_Add.busy == 0) {
            scoreboarding.FP_Add.busy = 1;
            return 1;
        }

            
    } else if(instrcutionRead.operation == Andi) {

        if(scoreboarding.Int_unit.busy == 0) {
            scoreboarding.Int_unit.busy = 1;
            return 1;
        }
        
    } else if(instrcutionRead.operation == Ori) {
        
        if(scoreboarding.Int_unit.busy == 0) {
            scoreboarding.Int_unit.busy = 1;
            return 1;
        }
        
    } 

    return 0;
}

int write() {
    if(instrcutionRead.operation == Move) {

        if(scoreboarding.Int_unit.busy == 1){
            scoreboarding.Int_unit.busy = 0;

        }
               
    } else if(instructionRead.operation == Add){

        if(scoreboarding.FP_Add.busy == 1){
            scoreboarding.FP_Add.busy = 0;
            
        }
        
            
    } else if(instructionRead.operation == Sub){

        if(scoreboarding.FP_Add.busy == 1){
            scoreboarding.FP_Add.busy = 0;
            
        }
        
            
    } else if(instrcutionRead.operation == And) {

        if(scoreboarding.Int_unit.busy == 1) {
            scoreboarding.Int_unit.busy = 0;
            
        }
        
    } else if(instrcutionRead.operation == Or) {

        if(scoreboarding.Int_unit.busy == 1) {
            scoreboarding.Int_unit.busy = 0;
            
        }
        
    } else if(instrcutionRead.operation == Slt) {
        
        if(scoreboarding.Int_unit.busy == 1) {
            scoreboarding.Int_unit.busy = 0;
            
        }

    } else if(instrcutionRead.operation == Mult) {
        
        if(scoreboarding.FP_Mult1.busy == 1){
            scoreboarding.FP_Mult1.busy = 0;
            
        }

            
        else if(scoreboarding.FP_Mult2.busy == 1){
            scoreboarding.FP_Mult2.busy = 0
            
        }
        
            
    } else if(instrcutionRead.operation == Div) {

        if(scoreboarding.FP_Div.busy == 1) {
            scoreboarding.FP_Div.busy = 0;
            
        }

            
    } else if(instrcutionRead.operation == Li) {

        if(scoreboarding.Int_unit.busy == 1) {
            scoreboarding.Int_unit.busy = 0;
            
        }
        
    } else if(instrcutionRead.operation == Addi) {

        if(scoreboarding.FP_Add.busy == 1) {
            scoreboarding.FP_Add.busy = 0;
            
        }

            
    } else if(instrcutionRead.operation == Andi) {

        if(scoreboarding.Int_unit.busy == 1) {
            scoreboarding.Int_unit.busy = 0;
            
        }
        
    } else if(instrcutionRead.operation == Ori) {
        
        if(scoreboarding.Int_unit.busy == 1) {
            scoreboarding.Int_unit.busy = 0;
            
        }
        
    }   
}

int scoreboarding(InstConfig instructionConfig, unsigned int instructionsMemory[], int memoryLength)
{
    int clock = 0;
    Instruction instructionRead;
    int i = 0;
    Scoreboarding scoreboarding;

    while() {
        write();
        execute();
        read();
        
        issue(instructionsMemory[i], &score);
    }


}