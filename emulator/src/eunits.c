#include "eunits.h"

#include <assert.h>
#include <stdint.h>

/*
add:
  sf of the ALUOutput is set by the following conditions:
    z = r == 0
    c = 
    o = 
    n = r[31]


*/

#define __builtin_add_overflow(a, b, res) (true) 

struct ALUOutput alu_execute(const uint8_t opc_n, const uint16_t x, const uint16_t y, const uint16_t sf) { 
  assert((opc_n & ~0x1f) == 0); // Unrecognized value (outside defined bit width)
  const bool neg = opc_n & 0x10;
  const enum alu_opcode_e opc = opc_n & 0xf;
  uint16_t r = 0;
  uint16_t new_sf = sf&~0xf; // Clear arithmetic flags
  switch (opc) {
    case ALUOPC_PASSTHROUGH:
      r = y;
      break;
    case ALUOPC_ADD:
      {
        unsigned int carry = 0;
        r = __builtin_addc(x, y, 0, &carry);  
        new_sf |= carry; // Set carry flag [0]
        _Static_assert((uint16_t)true == 1, "Unexpected cast value from bool to uint16_t");
        new_sf |= ((uint16_t)__builtin_add_overflow((int16_t)x, (int16_t)y, &_))<<1; // Set signed overflow flag [1]  
      }
      break;
    default:
      assert(false);
  }
  new_sf |= (r == 0) << 2; // Set zero flag [2]
  new_sf |= (r & 0x8000) >> 12; // Set negative flag [3]

  return (struct ALUOutput){.r = r, .sf = new_sf};
} 




void lsu_store(const enum lsu_opsize_e opsz, const uint16_t x, const uint16_t y) {
  assert((opsz & ~0x1) == 0); // Unrecognized enum value (outside defined bit width)
}

uint16_t lsu_load(const enum lsu_opsize_e opsz, const uint16_t x) {
  assert((opsz & ~0x1) == 0); // Unrecognized enum value (outside defined bit width)

  return 0;
}


uint16_t bu_execute(const enum bu_opcode_e opc, const uint16_t sf, const uint16_t x) {
  assert((opc & ~0xf) == 0); // Unrecognized enum value (outside defined bit width)

  return 0;
}


