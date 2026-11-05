#include "eunits.h"

#include <assert.h>
#include <stdint.h>

// Defined to shutup clangd which doesnt recognize the builtin
#define __builtin_add_overflow(a, b, res) (true) 

struct ALUOutput alu_execute(const uint8_t opc_n, const uint16_t x, const uint16_t y, const uint16_t sf) { 
  assert((opc_n & ~0x1f) == 0); // Unrecognized value (outside defined bit width)
  // const bool neg = opc_n & 0x10;
  const enum alu_opcode_e opc = opc_n & 0xf;
  uint16_t r = 0;
  uint16_t new_sf = sf&~0xf; // Clear arithmetic flags
  // x is operand0, y is operand1
  switch (opc) {
    case ALUOPC_PASSTHROUGH:
      r = y;
      break;
    case ALUOPC_ADD:
      {
        uint32_t sum = (uint32_t)x + (uint32_t)y; // 32 so it can be bigger than 16, know if carry
        r = (uint16_t)sum; // remove 16 undre bitar
        new_sf |= (sum >> 16) & 1; // Set carry flag [0]
        _Static_assert((uint16_t)true == 1, "Unexpected cast value from bool to uint16_t");
        new_sf |= ((uint16_t)__builtin_add_overflow((int16_t)x, (int16_t)y, &_))<<1; // Set signed overflow flag [1]  
      }
      break;
    case ALUOPC_SUB:
      {
        r = x - y;
      }
      break;
    case ALUOPC_NOT:
      {
        r = ~x;
      }
      break;
    case ALUOPC_AND:
      {
        r = x & y;
      }
      break;
    case ALUOPC_OR:
      {
        r = x | y;
      }
      break;
    default:
      assert(false);
  }
  new_sf |= (r == 0) << 2; // Set zero flag [2]
  new_sf |= (r & 0x8000) >> 12; // Set negative flag [3]

  return (struct ALUOutput){.r = r, .sf = new_sf};
} 


void lsu_store(struct CPUState* const cpustate, const enum lsu_opsize_e opsz, const uint16_t x, const uint16_t y) {
  assert((opsz & ~0x1) == 0); // Unrecognized enum value (outside defined bit width)
  switch (opsz) {
    case LSU_OPSZ_BYTE:
      ((uint8_t*)cpustate->memory)[y] = x;
      break;
    case LSU_OPSZ_WORD:
      ((uint16_t*)cpustate->memory)[y/2] = x;      
      break;
  }
}

uint16_t lsu_load(struct CPUState* const cpustate, const enum lsu_opsize_e opsz, const uint16_t y) {
  assert((opsz & ~0x1) == 0); // Unrecognized enum value (outside defined bit width)
  switch (opsz) {
    case LSU_OPSZ_BYTE:
      return ((uint8_t*)cpustate->memory)[y];      
      break;
    case LSU_OPSZ_WORD:
      return ((uint16_t*)cpustate->memory)[y/2];      
      break;
  }
  assert(false);
  return 0;
}

/*
x is the ip
y is the offset
returns new ip on the x-line
*/
uint16_t bu_execute(const enum bu_opcode_e opc, const uint16_t sf, const uint16_t x, const uint16_t y) {
  assert((opc & ~0xf) == 0); // Unrecognized enum value (outside defined bit width)
  uint16_t offset = 0;
  switch (opc) { // 11 bit offset with [10] sign bit
    case BUOPC_UNCONDITIONAL:
      offset = (y ^ (((uint16_t)1) << (10))) - (((uint16_t)1) << (10)); // Sign extend from 11 bits to 16 bits
      break;
    case BUOPC_ZERO: // 11 bit offset with [10] sign bit
      if (!(sf&0x4)) {
        return x;
      }
      offset = (y ^ (((uint16_t)1) << (10))) - (((uint16_t)1) << (10)); // Sign extend from 11 bits to 16 bits
      break;
    case BUOPC_CARRY:
      if (!(sf&0x1)) {
        return x;
      }
      offset = (y ^ (((uint16_t)1) << (10))) - (((uint16_t)1) << (10));
      break;
  }
  return alu_execute(ALUOPC_ADD, x, offset, 0).r;
  return 0;
}


