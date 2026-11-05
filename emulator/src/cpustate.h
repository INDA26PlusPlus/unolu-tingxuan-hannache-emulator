#ifndef CPUSTATE_H
#define CPUSTATE_H
#include <stdint.h>

/* Stack pointer register */
#define SP_IDX 6
/* Instruction pointer register */
#define IP_IDX 7

// Should be stack-allocated at the start of the program
struct CPUState {
  uint8_t memory[0x10000]; // Placed first to improve cache spatial locality on stack
  uint16_t inst_reg;
  uint16_t sf_reg;
  uint16_t gp_reg[8];
};

enum sf_reg_e {
  SFR_Z = 0x0001, // Zero
  SFR_C = 0x0002, // Carry
  SFR_O = 0x0004, // Signed Overflow
  SFR_N = 0x0008, // Negative
};

void cpustate_set_sf(struct CPUState* const cpustate, const uint16_t sf);

void cpustate_set_gp_reg(struct CPUState* const cpustate, const uint16_t value, const uint_fast8_t reg_idx);

uint16_t cpustate_get_gp_reg(struct CPUState* const cpustate, const uint_fast8_t reg_idx);


#endif
