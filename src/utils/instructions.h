/*
 * Ari Geller & Sam Wlody
 * CSC 252 - Project 3
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#define getOffset(x) immediate(x)
#define hint(x) SA(x)

extern uint8_t funcCode(uint32_t instr);
extern uint8_t opCode(uint32_t instr);
extern uint8_t RS(uint32_t instr);
extern uint8_t RT(uint32_t instr);
extern uint8_t RD(uint32_t instr);
extern uint8_t SA(uint32_t instr);
extern int16_t immediate(uint32_t instr);
extern uint32_t zeroExtend(int16_t i);
extern uint32_t instr_index(uint32_t instr);

#endif
