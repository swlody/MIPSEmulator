/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

#include <inttypes.h>
#include "instructions.h"

int getFunct(uint32_t instr) {
    return instr & 0x3f;
}

int getOpcode(uint32_t instr) {
    return (instr >> 26) & 0x3f;
}

int getRS(uint32_t instr) {
    return ((instr << 6) >> 26) & 0x1f;
}

int getRT(uint32_t instr) {
    return ((instr << 11) >> 26) & 0x1f;
}

int getRD(uint32_t instr) {
    return ((instr << 16) >> 26) & 0x1f; 
}

int getShamt(uint32_t instr) {
    return ((instr << 21) >> 26) & 0x1f; 
}

int getImmediate(uint32_t instr) {
    return ((instr << 16) >> 16) & 0xFFFF;
}

int getInstr_index(uint32_t instr) {
    return ((instr << 6) >> 6) & 0X3FFFFFF;
}
