/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#define getOffset(x) getImmediate(x)

extern uint8_t getFunc(uint32_t instr);
extern uint8_t getOpcode(uint32_t instr);
extern uint8_t getRS(uint32_t instr);
extern uint8_t getRT(uint32_t instr);
extern uint8_t getRD(uint32_t instr);
extern uint8_t getShamt(uint32_t instr);
extern int16_t getImmediate(uint32_t instr);
extern uint32_t zeroExtend(int16_t i);
extern uint32_t getInstr_index(uint32_t instr);

#endif
