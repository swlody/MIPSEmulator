/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#define getOffset(x) getImmediate(x)

extern uint32_t getFunct(uint32_t instr);
extern uint32_t getOpcode(uint32_t instr);
extern uint32_t getRS(uint32_t instr);
extern uint32_t getRT(uint32_t instr);
extern uint32_t getRD(uint32_t instr);
extern uint32_t getShamt(uint32_t instr);
extern uint32_t getImmediate(uint32_t instr);
extern uint32_t getInstr_index(uint32_t instr);

#endif
