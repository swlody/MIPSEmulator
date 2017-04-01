/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#define getOffset(x) getImmediate(x)

extern int getFunct(uint32_t instr);
extern int getOpcode(uint32_t instr);
extern int getRS(uint32_t instr);
extern int getRT(uint32_t instr);
extern int getRD(uint32_t instr);
extern int getShamt(uint32_t instr);
extern int getImmediate(uint32_t instr);
extern int getInstr_index(uint32_t instr);

#endif
