/* Execution units */ 
#ifndef EUNITS_H
#define EUNITS_H
#include <stdint.h>
#include <stdbool.h>

struct ALUOutput {
  const uint16_t r;
  const uint16_t sf; 
};

// [3:0] used
enum alu_opcode_e {
  ALUOPC_PASSTHROUGH = 0x0,
  ALUOPC_ADD = 0x4,
  ALUOPC_SUB = 0x5,
  ALUOPC_NOT = 0x8,
  ALUOPC_AND = 0x9,
  ALUOPC_OR = 0xa,
};

// [1:0] used
enum eunit_e {
  EUNIT_ALU = 0x0,
  EUNIT_LSU = 0x1,
  EUNIT_BU = 0x2,
  EUNIT_NONE = 0x3,
};


/*
opc_n: [4] negative?, [3:0] alu opcode (see alu_opcode_e)
*/
struct ALUOutput alu_execute(const uint8_t opc_n, const uint16_t x, const uint16_t y); 

// [0] used
enum lsu_opsize_e {
  LSU_OPSZ_BYTE = 0x0,
  LSU_OPSZ_WORD = 0x1,
};


/*
x is the address on the x-line
y is the data on the y-line
if opsz is LSU_OPSZ_BYTE then only the least significant byte of y is stored. 
*/
void lsu_store(const enum lsu_opsize_e opsz, const uint16_t x, const uint16_t y);

/*
x is the address on the x-line 
the return is the data on the y-line
if opsz is LSU_OPSZ_BYTE then only the least significant byte of y is stored. 
*/
uint16_t lsu_load(const enum lsu_opsize_e opsz, const uint16_t x);

// [3:0] used
enum bu_opcode_e {
  BUOPC_UNCONDITIONAL = 0x0,
  BUOPC_ZERO = 0x1,
};

/*
The branching unit interfaces with the alu.
It checks the condition, if it is true then it executes an operation on the alu.
x is the offset on the x-line
y is the new ip on the y-line
*/
uint16_t bu_execute(const enum bu_opcode_e opc, const uint16_t sf, const uint16_t x);


#endif

