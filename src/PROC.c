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

    printf("\n ----- Execute Program ----- \n");
    printf("Max Instruction to run = %d \n", MaxInst);
    PC = exec.GPC_START;
    newPC = PC;
    for(i=0; i<MaxInst; i++) {
        DynInstCount++;
        CurrentInstruction = readWord(PC, false);  
        printRegFile();
        switch(getFunct(CurrentInstruction)) {
            /*********** ARITHMETIC INSTRUCTIONS ***********/
            case 0x20:{
                // add
                uint32_t RS = getRS(CurrentInstruction);
                uint32_t RT = getRT(CurrentInstruction); 
                uint32_t RD = getRD(CurrentInstruction);
                uint32_t sum = readWord(RS, false) + readWord(RT, false);
                writeWord(RD, sum, false);
                break;
            }
            case 0x21:{
                // addu

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
                break;
            }
            case 0x26:{
                // xor
                break;
            }
            case 0x27:{
                // nor

                break;
            }
            case 0x25:{
                // or

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
                int opCode = getOpcode(CurrentInstruction);
                switch(opCode) {
                    /*********** IMMEDIATE INSTRUCTIONS ***********/
                    case 0x08:{
                        // addi
                        uint32_t RS = getRS(CurrentInstruction);
                        uint32_t RT = getRT(CurrentInstruction);
                        uint32_t sum = readWord(RS, false) + getImmediate(CurrentInstruction);
                        writeWord(RT, sum, false);
                        break;
                    }
                    case 0x09:{
                        // addiu

                        break;
                    }
                    case 0x0C:{
                        // andi

                        break;
                    }
                    case 0x0E:{
                        // xori

                        break;
                    }
                    case 0x0D:{
                        // ori

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
                    default:{
                        // NOP

                        break;
                    }
                } // End opCode switch
                break;
            } // End func default
        } // End func switch

        if(newPC == PC) {
            // No jump/branch instruction has occurred to update PC
            // Default to increment by size of word (4 bytes = 32 bits)
            PC += 4;
        }
        PC = newPC;
    } //end fori
    
    //Close file pointers & free allocated Memory
    closeFDT();
    CleanUp();
    return 1;
}
