

#include <stdio.h>
#include <inttypes.h>
#include "RegFile.h"

// 32x32 Register File
char* RegName[NUMBER_OF_REGS] = {"$zero", "$at", "$v0", "$v1", "$a0", 
    "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", 
    "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", 
    "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra", "HI", "LO"};
int32_t RegFile[NUMBER_OF_REGS];

void initRegFile(int32_t val) {
    int i;
    for(i=0;i<34;i++) {
    RegFile[i] = val;
    }
}

void printRegFile() {
    printf("\n ----- REG DUMP ----- \n");
    int j;
    for ( j=0; j < NUMBER_OF_REGS; j++) {
        printf("REG[%2d]: 0x%08x (%d)",j,RegFile[j],RegFile[j]);
        if(j%2==0){printf("\t\t");}
        else{printf("\n");}
        }
}




