/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

#include <inttypes.h>
#include "instructions.h"

uint32_t getFunct(uint32_t instr) {
    return instr & 0x3f;
}

uint32_t getOpcode(uint32_t instr) {
    return (instr >> 26) & 0x3f;
}

uint32_t getRS(uint32_t instr) {
    return ((instr << 6) >> 26) & 0x1f;
}

uint32_t getRT(uint32_t instr) {
    return ((instr << 11) >> 26) & 0x1f;
}

uint32_t getRD(uint32_t instr) {
    return ((instr << 16) >> 26) & 0x1f; 
}

uint32_t getShamt(uint32_t instr) {
    return ((instr << 21) >> 26) & 0x1f; 
}

uint32_t getImmediate(uint32_t instr) {
    return ((instr << 16) >> 16) & 0xFFFF;
}

uint32_t getInstr_index(uint32_t instr) {
    return ((instr << 6) >> 6) & 0X3FFFFFF;
}
