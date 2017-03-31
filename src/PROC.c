/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"

uint32_t DynInstCount = 0;

void write_initialization_vector(uint32_t sp, uint32_t gp, uint32_t start) {
    printf("\n ----- BOOT Sequence ----- \n");
    printf("Initializing sp=0x%08x; gp=0x%08x; start=0x%08x\n", sp, gp, start);
    RegFile[28] = gp;
    RegFile[29] = sp;
    RegFile[31] = start;
    printRegFile();
}

int getFunct(uint32_t instr) {
    return instr & 0x3f;
}

int getOpcode(uint32_t instr) {
    return (instr >> 26) & 0x3f;
}

int main(int argc, char * argv[]) {
    int MaxInst = 0;
    int status = 0;
    uint32_t i; 
    uint32_t PC,newPC;
    uint32_t CurrentInstruction;

    if (argc < 2) {
        printf("Input argument missing \n");
        return -1;
    }
    sscanf(argv[2], "%d", &MaxInst);

    //Open file pointers & initialize Heap & Regsiters
    initHeap();
    initFDT();
    initRegFile(0);
    
    //Load required code portions into Emulator Memory
    status = LoadOSMemory(argv[1]);
    if(status < 0) { 
        return status; 
    }
    
    //set Global & Stack Pointers for the Emulator
    // & provide startAddress of Program in Memory to Processor
    write_initialization_vector(exec.GSP, exec.GP, exec.GPC_START);

    printf("\n ----- Execute Program ----- \n");
    printf("Max Instruction to run = %d \n", MaxInst);
    PC = exec.GPC_START;
    for(i=0; i<MaxInst; i++) {
        DynInstCount++;
        CurrentInstruction = readWord(PC, false);  
        printRegFile();
        switch(getFunct(CurrentInstruction)) {
            case 0x20:
                // add
                int opCode = getOpcode(CurrentInstruction);
                if(opCode == 0x8) {
                    // addi
                } else if(opCode == 0x9) {
                    // addiu
                } else {
                    // Default add
                }
                break;
            case 0x21:
                // addu

                break;
            case 0x22:
                // sub

                break;
            case 0x23:
                // subu

                break;
            case 0x1A:
                // div

                break;
            case 0x1B:
                // divu

                break;
            case 0x18:
                // mult

                break;
            case 0x19:
                // multu

                break;
            case 0x10:
                // mfhi

                break;
            case 0x12:
                // mflo

                break;
            case 0x11:
                // mthi

                break;
            case 0x13:
                // mtlo

                break;
            case 0x24:
                // and
                int opCode = getOpcode(CurrentInstruction);
                if(opCode == 0xC) {
                    // andi
                } else {
                    // Default and
                }
                break;
            case 0x26:
                // xor
                int opCode = getOpcode(CurrentInstruction);
                if(opCode == 0xE) {
                    // xori
                } else {
                    // Default xor
                }
                break;
            case 0x27:
                // nor

                break;
            case 0x25:
                // or
                int opCode = getOpcode(CurrentInstruction);
                if(opCode == 0xD) {
                    // ori
                } else {
                    // Default or
                }
                break;
            case 0x00:
                // sll

                break;
            case 0x4:
                // sllv

                break;
            case 0x2A:
                // slt
                int opCode = getOpcode(CurrentInstruction);
                if(opCode == 0xA) {
                    // slti
                } else if(opCode == 0xB) {
                    // sltiu
                } else {
                    // Default slt
                }
                break;
            case 0x2B:
                // sltu

                break;
            case 0x3:
                // sra

                break;
            case 0x7:
                // srav

                break;
            case 0x2:
                // srl

                break;
            case 0x4:
                // srlv

                break;
            default:
                int opCode = getOpcode(CurrentInstruction);
                switch(opCode) {
                    // LB, LBU, SB, SH, etc.
                    default:
                        // NOP

                        break;
                }

                break;
        } // End switch


    } //end fori
    
    //Close file pointers & free allocated Memory
    closeFDT();
    CleanUp();
    return 1;
}
