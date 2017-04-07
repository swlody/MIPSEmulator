/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

// TODO Check values of opcodes and func codes

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
    uint32_t CI; // CI = CurrentInstruction

    if(argc < 2) {
        printf("Input argument missing \n");
        return -1;
    }
    sscanf(argv[2], "%d", &MaxInst);

    // Open file pointers & initialize Heap & Regsiters
    initHeap();
    initFDT();
    initRegFile(0);
    
    // Load required code portions into Emulator Memory
    status = LoadOSMemory(argv[1]);
    if(status < 0) { 
        return status; 
    }
    
    // Set Global & Stack Pointers for the Emulator
    // & provide startAddress of Program in Memory to Processor
    write_initialization_vector(exec.GSP, exec.GP, exec.GPC_START);

    // char* RegNames[NUMBER_OF_REGS] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    //     "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    //     "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    //     "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra", "LO", "HI"};

    printf("\n ----- Execute Program ----- \n");
    printf("Max Instruction to run = %d \n", MaxInst);
    PC = exec.GPC_START;
    int branch = 0;
    for(i=0; i<MaxInst; i++) {
        DynInstCount++;
        CI = readWord(PC, false);  
        printRegFile();
        uint8_t op = opCode(CI);
        switch(op) {
            case 0x00:{
                if(CI == 0) {
                    // NOP (intentional)
                    break;
                }
                uint8_t func = funcCode(CI);
                switch(func) {
                    /*********** ARITHMETIC INSTRUCTIONS ***********/
                    case 0x20:{
                        // add
                        uint8_t source = RS(CI);
                        uint8_t adder = RT(CI); 
                        if((RegFile[adder] > 0 && RegFile[source] > INT_MAX - RegFile[adder])
                            || (RegFile[adder] < 0 && RegFile[source] < INT_MIN - RegFile[adder])) {
                            printf("Unexpected behavior. Value overflow.\n");
                        }
                        RegFile[RD(CI)] = RegFile[source] + RegFile[adder];
                        break;
                    }
                    case 0x21:{
                        // addu
                        RegFile[RD(CI)] = RegFile[RS(CI)] + RegFile[RT(CI)];
                        break;
                    }
                    case 0x22:{
                        // sub
                        uint8_t source = RS(CI);
                        uint8_t subber = RT(CI);
                        // TODO Check this
                        if((RegFile[source] > 0 && RegFile[subber] < INT_MIN + RegFile[source])
                            || (RegFile[source] < 0 && RegFile[subber] > INT_MAX - RegFile[source])) {
                            printf("Unexpected behavior. Value overflow.\n");
                        }
                        RegFile[RD(CI)] = RegFile[source] - RegFile[subber];
                        break;
                    }
                    case 0x23:{
                        // subu
                        RegFile[RD(CI)] = RegFile[RS(CI)] - RegFile[RT(CI)];
                        break;
                    }
                    case 0x1A:{
                        // div
                        int32_t dividend = RegFile[RS(CI)];
                        int32_t divisor = RegFile[RT(CI)];
                        RegFile[32] = (int32_t) (dividend / divisor);
                        RegFile[33] = dividend % divisor;
                        break;
                    }
                    case 0x1B:{
                        // divu
                        uint32_t dividend = (uint32_t) RegFile[RS(CI)];
                        uint32_t divisor = (uint32_t) RegFile[RT(CI)];
                        RegFile[32] = (int32_t) (dividend / divisor);
                        RegFile[33] = (int32_t) (dividend % divisor);
                        break;
                    }
                    case 0x18:{
                        // mult
                        int64_t multiplicand = (int64_t) RegFile[RS(CI)];
                        int64_t multiplier = (int64_t) RegFile[RT(CI)];
                        int64_t result = multiplier * multiplicand;
                        RegFile[32] = (int32_t) (result & 0xFFFFFFFF);
                        RegFile[33] = (int32_t) ((result >> 32) & 0xFFFFFFFF);
                        break;
                    }
                    case 0x19:{
                        // multu
                        uint64_t multiplicand = (uint64_t) RegFile[RS(CI)];
                        uint64_t multiplier = (uint64_t) RegFile[RT(CI)];
                        int64_t result = multiplier * multiplicand;
                        RegFile[32] = (int32_t) (result & 0xFFFFFFFF);
                        RegFile[33] = (int32_t) ((result >> 32) & 0xFFFFFFFF);
                        break;
                    }
                    case 0x10:{
                        // mfhi
                        RegFile[RD(CI)] = RegFile[33];
                        break;
                    }
                    case 0x12:{
                        // mflo
                        RegFile[RD(CI)] = RegFile[32];
                        break;
                    }
                    case 0x11:{
                        // mthi
                        RegFile[33] = RegFile[RS(CI)];
                        break;
                    }
                    case 0x13:{
                        // mtlo
                        RegFile[32] = RegFile[RD(CI)];
                        break;
                    }

                    /*********** LOGIC INSTRUCTIONS ***********/
                    case 0x24:{
                        // and
                        RegFile[RD(CI)] = RegFile[RS(CI)] & RegFile[RT(CI)];
                        break;
                    }
                    case 0x26:{
                        // xor
                        RegFile[RD(CI)] = RegFile[RS(CI)] ^ RegFile[RT(CI)];
                        break;
                    }
                    case 0x27:{
                        // nor
                        RegFile[RD(CI)] = ~(RegFile[RS(CI)] | RegFile[RT(CI)]);
                        break;
                    }
                    case 0x25:{
                        // or
                        RegFile[RD(CI)] = RegFile[RS(CI)] | RegFile[RT(CI)];
                        break;
                    }
                    
                    /*********** SHIFTS ***********/
                    case 0x00:{
                        // sll
                        RegFile[RD(CI)] = RegFile[RT(CI)] << SA(CI);
                        break;
                    }
                    case 0x04:{
                        // sllv
                        uint8_t shamt = RegFile[RS(CI)] & 0x1F;
                        RegFile[RD(CI)] = RegFile[RT(CI)] << shamt;
                        break;
                    }
                    case 0x2A:{
                        // slt
                        RegFile[RD(CI)] = RegFile[RS(CI)] < RegFile[RT(CI)];
                        break;
                    }
                    case 0x2B:{
                        // sltu
                        RegFile[RD(CI)] = (uint32_t) RegFile[RS(CI)] < (uint32_t) RegFile[RT(CI)];
                        break;
                    }
                    case 0x03:{
                        // sra
                        RegFile[RD(CI)] = RegFile[RT(CI)] >> SA(CI);
                        break;
                    }
                    case 0x07:{
                        // srav
                        uint8_t shamt = RegFile[RS(CI)] & 0x1F;
                        RegFile[RD(CI)] = RegFile[RT(CI)] >> shamt;
                        break;
                    }
                    case 0x02:{
                        // srl
                        uint8_t s = SA(CI);
                        if(s == 0) {
                            RegFile[RD(CI)] = RegFile[RT(CI)];
                        } else {
                            RegFile[RD(CI)] = (0x7FFFFFFF >> (s - 1)) & (RegFile[RT(CI)] >> s);
                        }
                        break;
                    }
                    case 0x06:{
                        // srlv
                        uint8_t s = RegFile[RS(CI)];
                        if(s == 0) {
                            RegFile[RD(CI)] = RegFile[RT(CI)];
                        } else {
                            RegFile[RD(CI)] = (0x7FFFFFFF >> (s - 1)) & (RegFile[RT(CI)] >> s);
                        }
                        break;
                    }
                    case 0x09:{
                        // jalr
                        if(branch == 1) {
                            printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                            printf("Exiting");
                            return -1;
                        }
                        uint8_t dest = RD(CI);
                        if(dest == 0) {
                            RegFile[31] = PC + 8;
                        } else {
                            RegFile[dest] = PC + 8;
                        }
                        newPC = RegFile[RS(CI)];
                        branch = 1;
                        break;
                    }
                    case 0x08:{
                        // jr
                        if(branch == 1) {
                            printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                            printf("Exiting");
                            return -1;
                        }
                        newPC = RegFile[RS(CI)];
                        branch = 1;
                        break;
                    }
                    case 0x0C:{
                        // syscall
                        SyscallExe(RegFile[2]);
                        break;
                    }
                    default:{
                        printf("Function code %" PRIu8 " not supported or invalid.\n", func);
                    }
                } // End func switch
            } // End opCode == 0

            /*********** IMMEDIATE INSTRUCTIONS ***********/
            case 0x08:{
                // addi
                uint8_t source = RS(CI);
                int16_t imm = immediate(CI);
                if((RegFile[source] > 0 && imm > INT_MAX - RegFile[source])
                    || (RegFile[source] < 0 && imm < INT_MIN - RegFile[source])) {
                    printf("Unexpected behavior. Value overflow.\n");
                }
                RegFile[RT(CI)] = RegFile[source] + imm;
                break;
            }
            case 0x09:{
                // addiu
                RegFile[RD(CI)] = RegFile[RS(CI)] + immediate(CI);
                break;
            }
            case 0x0C:{
                // andi
                RegFile[RD(CI)] = RegFile[RS(CI)] & zeroExtend(immediate(CI));
                break;
            }
            case 0x0E:{
                // xori
                RegFile[RD(CI)] = RegFile[RS(CI)] ^ zeroExtend(immediate(CI));
                break;
            }
            case 0x0D:{
                // ori
                RegFile[RD(CI)] = RegFile[RS(CI)] | zeroExtend(immediate(CI));
                break;
            }
            case 0x0A:{
                // slti
                RegFile[RT(CI)] = RegFile[RS(CI)] < signExtend(immediate(CI));
                break;
            }
            case 0x0B:{
                // sltiu
                RegFile[RT(CI)] = (uint32_t) RegFile[RS(CI)] < (uint32_t) signExtend(immediate(CI));
                break;
            }

            /*********** BRANCHES AND JUMPS ***********/
            case 0x04:{
                // beq
                if(branch == 1) {
                    printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                    printf("Exiting");
                    return -1;
                }
                if(RegFile[RS(CI)] == RegFile[RT(CI)]) {
                    newPC = PC + 4 + (signExtend(offset(CI)) << 2);
                    branch = 1;
                }
                break;
            }
            case 0x14:{
                // beql
                if(branch == 1) {
                    printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                    printf("Exiting");
                    return -1;
                }
                if(RegFile[RS(CI)] == RegFile[RT(CI)]) {
                    newPC = PC + 4 + (signExtend(offset(CI)) << 2);
                    branch = 1;
                } else {
                    PC += 4;
                }
                break;
            }
            case 0x07:{
                // bgtz
                if(branch == 1) {
                    printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                    printf("Exiting");
                    return -1;
                }
                if(RegFile[RS(CI)] > 0) {
                    newPC = PC + 4 + (signExtend(offset(CI)) << 2);
                    branch = 1;
                }
                break;
            }
            case 0x06:{
                // blez
                if(branch == 1) {
                    printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                    printf("Exiting");
                    return -1;
                }
                if(RegFile[RS(CI)] <= 0) {
                    newPC = PC + 4 + (signExtend(offset(CI)) << 2);
                    branch = 1;
                }
                break;
            }
            case 0x16:{
                // blezl
                if(branch == 1) {
                    printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                    printf("Exiting");
                    return -1;
                }
                if(RegFile[RS(CI)] <= 0) {
                    newPC = PC + 4 + (signExtend(offset(CI)) << 2);
                    branch = 1;
                } else {
                    PC += 4;
                }
                break;
            }
            case 0x05:{
                // bne
                if(branch == 1) {
                    printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                    printf("Exiting");
                    return -1;
                }
                if(RegFile[RS(CI)] != RegFile[RT(CI)]) {
                    newPC = PC + 4 + (signExtend(offset(CI)) << 2);
                    branch = 1;
                }
                break;
            }
            case 0x15:{
                // bnel
                if(branch == 1) {
                    printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                    printf("Exiting");
                    return -1;
                }
                if(RegFile[RS(CI)] != RegFile[RT(CI)]) {
                    newPC = PC + 4 + (signExtend(offset(CI)) << 2);
                    branch = 1;
                } else {
                    PC += 4;
                }
                break;
            }
            case 0x02:{
                // j
                if(branch == 1) {
                    printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                    printf("Exiting");
                    return -1;
                }
                newPC = ((PC + 4) & ~0x3) & (instr_index(CI) << 2);
                branch = 1;
                break;
            }
            case 0x03:{
                // jal
                if(branch == 1) {
                    printf("Undefined behavior. Branch or jump in branch delay slot.\n");
                    printf("Exiting");
                    return -1;
                }
                RegFile[31] = PC + 8;
                newPC = ((PC + 4) & ~0x3) & (instr_index(CI) << 2);
                branch = 1;
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
                uint32_t REGIMM = RT(CI);
                if(REGIMM == 0x01) {
                    // bgez
                } else if(REGIMM == 0x11) {
                    // bgezal
                } else if(REGIMM == 0x10) {
                    // bltzal
                } else if(REGIMM == 0x00) {
                    // bltz
                } else {
                    printf("Invalid REGIMM %u for opCode 0x01\n", REGIMM);
                }
                break;
            }
            default:{
                printf("Op code %" PRIu8 " not supported or invalid.", opCode);
            }
        } // End opCode switch

        if(branch == 2) {
            // Full instruction complete since previous instruction's branch
            PC = newPC;
            branch = 0;
        } else {
            // If branch == 1 set to 2, else keep 0
            branch *= 2;
            PC += 4;
        }
    } // End program loop
    
    // Close file pointers & free allocated Memory
    closeFDT();
    CleanUp();
    return 1;
}
