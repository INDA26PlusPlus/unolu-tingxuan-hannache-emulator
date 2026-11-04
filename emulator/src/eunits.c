#include "eunits.h"

#include <assert.h>

/*
add:
  sf of the ALUOutput is set by the following conditions:
    z = r == 0
    c = 
    o = 
    n = r[31]


*/
struct ALUOutput alu_execute(const uint8_t opc_n, const uint16_t x, const uint16_t y) { 
  assert((opc_n & ~0x1f) == 0); // Unrecognized value (outside defined bit width)
  const bool neg = opc_n & 0x10;
  const enum alu_opcode_e opc = opc_n & 0xf;

  return (struct ALUOutput){.r = 0, .sf = 0};
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


