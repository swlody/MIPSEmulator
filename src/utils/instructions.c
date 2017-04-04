/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

#include <inttypes.h>
#include "instructions.h"

uint8_t funcCode(uint32_t instr) {
    return (uint8_t) instr & 0x3F;
}

uint8_t opCode(uint32_t instr) {
    return ((uint8_t) (instr >> 26)) & 0x3F;
}

uint8_t RS(uint32_t instr) {
    return ((uint8_t) (instr >> 21)) & 0x1F;
}

uint8_t RT(uint32_t instr) {
    return ((uint8_t) (instr >> 16)) & 0x1F;
}

uint8_t RD(uint32_t instr) {
    return ((uint8_t) (instr >> 11)) & 0x1F; 
}

uint8_t SA(uint32_t instr) {
    return ((uint8_t) (instr >> 6)) & 0x1F; 
}

int16_t immediate(uint32_t instr) {
    return (int16_t) instr;
}

int32_t signExtend(int16_t value) {
    return (int32_t) value;
}

uint32_t zeroExtend(int16_t value) {
	return ((uint32_t) value) & 0xFFFF;
}

uint32_t instr_index(uint32_t instr) {
    return instr & 0x3FFFFFF;
}
