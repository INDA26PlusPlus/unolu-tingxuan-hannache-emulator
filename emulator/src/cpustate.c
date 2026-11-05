#include "cpustate.h"

#include <assert.h>

void cpustate_set_sf(struct CPUState* const cpustate, const uint16_t sf) {
  assert((sf & ~0xf) == 0);
  cpustate->sf_reg = sf;
}

void cpustate_set_gp_reg(struct CPUState* const cpustate, const uint16_t value, const uint_fast8_t reg_idx) {
  assert(reg_idx < 8);
  cpustate->gp_reg[reg_idx] = value;
}

uint16_t cpustate_get_gp_reg(struct CPUState* const cpustate, const uint_fast8_t reg_idx) {
  assert(reg_idx < 8);
  return cpustate->gp_reg[reg_idx];
}


