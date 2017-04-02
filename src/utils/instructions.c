/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

#include <inttypes.h>
#include "instructions.h"

uint8_t getFunc(uint32_t instr) {
    return (uint8_t) instr & 0x3F;
}

uint8_t getOpcode(uint32_t instr) {
    return ((uint8_t) (instr >> 26)) & 0x3F;
}

uint8_t getRS(uint32_t instr) {
    return ((uint8_t) (instr >> 21)) & 0x1F;
}

uint8_t getRT(uint32_t instr) {
    return ((uint8_t) (instr >> 16)) & 0x1F;
}

uint8_t getRD(uint32_t instr) {
    return ((uint8_t) (instr >> 11)) & 0x1F; 
}

uint8_t getShamt(uint32_t instr) {
    return ((uint8_t) (instr >> 6)) & 0x1F; 
}

int16_t getImmediate(uint32_t instr) {
    return (int16_t) instr;
}

uint32_t zeroExtend(int16_t i) {
	return ((uint32_t) i) & 0xFFFF;
}

uint32_t getInstr_index(uint32_t instr) {
    return ((instr << 6) >> 6) & 0x3FFFFFF;
}
