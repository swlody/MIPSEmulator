/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

// TODO What to do in case of exception?
// TODO What to do if MaxInstr > #instructions in proram? (and how to detect)

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"
#include "utils/instructions.h"

uint32_t DynInstCount = 0;

void write_initialization_vector(uint32_t sp, uint32_t gp, uint32_t start) {
    printf("\n ----- BOOT Sequence ----- \n");
    printf("Initializing sp=0x%08x; gp=0x%08x; start=0x%08x\n", sp, gp, start);
    RegFile[28] = gp;
    RegFile[29] = sp;
    RegFile[31] = start;
    printRegFile();
}

int main(int argc, char * argv[]) {
    int MaxInst = 0;
    int status = 0;
    uint32_t i; 
    uint32_t PC, newPC;
    uint32_t CurrentInstruction;

    if(argc < 2) {
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

    // char* RegNames[NUMBER_OF_REGS] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    //     "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    //     "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    //     "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

    printf("\n ----- Execute Program ----- \n");
    printf("Max Instruction to run = %d \n", MaxInst);
    PC = exec.GPC_START;
    newPC = PC;
    for(i=0; i<MaxInst; i++) {
        DynInstCount++;
        CurrentInstruction = readWord(PC, false);  
        printRegFile();
        uint8_t opCode = getOpcode(CurrentInstruction);
        switch(opCode) {
            /*********** IMMEDIATE INSTRUCTIONS ***********/
            case 0x08:{
                // addi
                uint8_t RS = getRS(CurrentInstruction);
                uint8_t RT = getRT(CurrentInstruction);
                int16_t i = getImmediate(CurrentInstruction);
                if((RegFile[RS] > 0 && i > INT_MAX - RegFile[RS])
                    || (RegFile[RS] < 0 && i < INT_MIN - RegFile[RS])) {
                    // TODO Overflow occurs
                    break;
                }
                int32_t sum = RegFile[RS] + i;
                RegFile[RT] = sum;
                break;
            }
            case 0x09:{
                // addiu
                uint8_t RS = getRS(CurrentInstruction);
                int16_t i = getImmediate(CurrentInstruction);
                uint8_t RD = getRD(CurrentInstruction);
                RegFile[RD] = RegFile[RS] + i;
                break;
            }
            case 0x0C:{
                // andi
                uint8_t RS = getRS(CurrentInstruction);
                int16_t i = getImmediate(CurrentInstruction);
                uint8_t RD = getRD(CurrentInstruction);
                RegFile[RD] = RegFile[RS] & zeroExtend(i);
                break;
            }
            case 0x0E:{
                // xori
                uint8_t RS = getRS(CurrentInstruction);
                int16_t i = getImmediate(CurrentInstruction);
                uint8_t RD = getRD(CurrentInstruction);
                RegFile[RD] = RegFile[RS] ^ zeroExtend(i);
                break;
            }
            case 0x0D:{
                // ori
                uint8_t RS = getRS(CurrentInstruction);
                int16_t i = getImmediate(CurrentInstruction);
                uint8_t RD = getRD(CurrentInstruction);
                RegFile[RD] = RegFile[RS] | zeroExtend(i);
                break;
            }
            case 0x0A:{
                // slti

                break;
            }
            case 0x0B:{
                // sltiu

                break;
            }

            /*********** BRANCHES AND JUMPS ***********/
            case 0x04:{
                // beq

                break;
            }
            case 0x14:{
                // beql

                break;
            }
            case 0x07:{
                // bgtz

                break;
            }
            case 0x06:{
                // blez

                break;
            }
            case 0x16:{
                // blezl

                break;
            }
            case 0x05:{
                // bne

                break;
            }
            case 0x15:{
                // bnel

                break;
            }
            case 0x02:{
                // j

                break;
            }
            case 0x03:{
                // jal

                break;
            }

            /*********** LOADS ***********/
            case 0x20:{
                // LB

                break;
            }
            case 0x24:{
                // LBU

                break;
            }
            case 0x21:{
                // LH

                break;
            }
            case 0x25:{
                // LHU

                break;
            }
            case 0x0F:{
                // LUI

                break;
            }
            case 0x23:{
                // LW

                break;
            }
            case 0x22:{
                // LWL

                break;
            }
            case 0x26:{
                // LWR

                break;
            }

            /*********** STORES ***********/
            case 0x28:{
                // SB

                break;
            }
            case 0x29:{
                // SH

                break;
            }
            case 0x2B:{
                // SW

                break;
            }
            case 0x2A:{
                // SWL

                break;
            }
            case 0x2E:{
                // SWR

                break;
            }
            case 0x01:{
                /*********** MORE BRANCHES ***********/
                uint32_t RS = getRS(CurrentInstruction);
                if(RS == 0x01) {
                    // bgez
                } else if(RS == 0x11) {
                    // bgezal
                } else if(RS == 0x10) {
                    // bltzal
                } else {
                    // bltz
                }
            }
            case 0x00:{
                uint8_t func = getFunc(CurrentInstruction);
                switch(func) {
                    /*********** ARITHMETIC INSTRUCTIONS ***********/
                    case 0x20:{
                        // add
                        uint8_t RS = getRS(CurrentInstruction);
                        uint8_t RT = getRT(CurrentInstruction); 
                        uint8_t RD = getRD(CurrentInstruction);
                        if((RegFile[RT] > 0 && RegFile[RS] > INT_MAX - RegFile[RT])
                            || (RegFile[RT] < 0 && RegFile[RS] < INT_MIN - RegFile[RT])) {
                            // TODO Overflow occurs
                            break;
                        }
                        int32_t sum = RegFile[RS] + RegFile[RT];
                        RegFile[RD] = sum;
                        break;
                    }
                    case 0x21:{
                        // addu
                        uint8_t RS = getRS(CurrentInstruction);
                        uint8_t RT = getRT(CurrentInstruction);
                        uint8_t RD = getRD(CurrentInstruction);
                        RegFile[RD] = RegFile[RS] + RegFile[RT];
                        break;
                    }
                    case 0x22:{
                        // sub

                        break;
                    }
                    case 0x23:{
                        // subu

                        break;
                    }
                    case 0x1A:{
                        // div

                        break;
                    }
                    case 0x1B:{
                        // divu

                        break;
                    }
                    case 0x18:{
                        // mult

                        break;
                    }
                    case 0x19:{
                        // multu

                        break;
                    }
                    case 0x10:{
                        // mfhi

                        break;
                    }
                    case 0x12:{
                        // mflo

                        break;
                    }
                    case 0x11:{
                        // mthi

                        break;
                    }
                    case 0x13:{
                        // mtlo

                        break;
                    }

                    /*********** LOGIC INSTRUCTIONS ***********/
                    case 0x24:{
                        // and
                        uint8_t RS = getRS(CurrentInstruction);
                        uint8_t RT = getRT(CurrentInstruction);
                        uint8_t RD = getRD(CurrentInstruction);
                        RegFile[RD] = RegFile[RS] & RegFile[RT];
                        break;
                    }
                    case 0x26:{
                        // xor
                        uint8_t RS = getRS(CurrentInstruction);
                        uint8_t RT = getRT(CurrentInstruction);
                        uint8_t RD = getRD(CurrentInstruction);
                        RegFile[RD] = RegFile[RS] ^ RegFile[RT];
                        break;
                    }
                    case 0x27:{
                        // nor
                        uint8_t RS = getRS(CurrentInstruction);
                        uint8_t RT = getRT(CurrentInstruction);
                        uint8_t RD = getRD(CurrentInstruction);
                        RegFile[RD] = ~(RegFile[RS] | RegFile[RT]);
                        break;
                    }
                    case 0x25:{
                        // or
                        uint8_t RS = getRS(CurrentInstruction);
                        uint8_t RT = getRT(CurrentInstruction);
                        uint8_t RD = getRD(CurrentInstruction);
                        RegFile[RD] = RegFile[RS] | RegFile[RT];
                        break;
                    }
                    
                    /*********** SHIFTS ***********/
                    case 0x00:{
                        // sll
                        if(CurrentInstruction != 0) {

                        } // else NOP
                        break;
                    }
                    case 0x04:{
                        // sllv

                        break;
                    }
                    case 0x2A:{
                        // slt

                        break;
                    }
                    case 0x2B:{
                        // sltu

                        break;
                    }
                    case 0x03:{
                        // sra

                        break;
                    }
                    case 0x07:{
                        // srav

                        break;
                    }
                    case 0x02:{
                        // srl

                        break;
                    }
                    case 0x06:{
                        // srlv

                        break;
                    }
                    case 0x09:{
                        // jalr

                        break;
                    }
                    case 0x08:{
                        // jr

                        break;
                    }
                    case 0x0C:{
                        // syscall

                        break;
                    }
                    default:{
                        // NOP (funcCode not supported)

                        break;
                    }
                } // End func switch
                break;
            } // End opCode == 0
            default:{
                // NOP (opCode not supported)

                break;
            }
        } // End opCode switch

        if(newPC == PC) {
            // No jump/branch instruction has occurred to update PC
            // Default to increment by size of word (4 bytes = 32 bits)
            newPC += 4;
        }
        PC = newPC;
    } //end fori
    
    //Close file pointers & free allocated Memory
    closeFDT();
    CleanUp();
    return 0;
}
