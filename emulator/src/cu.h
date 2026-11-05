/* Control Unit */
#ifndef CU_H
#define CU_H
#include "cpustate.h"
#include "eunits.h"
#include <stdint.h>


enum opcode_e {
  OPC_NOP = 0x00,

  OPC_MOV = 0x01,
  OPC_MOVL = 0x02,
  OPC_MOVU = 0x03,

  OPC_ADD = 0x04,
  OPC_SUB = 0x05,

  OPC_NOT = 0x08,
  OPC_AND = 0x09,
  OPC_OR = 0x0a,

  OPC_LB = 0x0c,
  OPC_SB = 0x0d,
  OPC_LW = 0x0e,
  OPC_SW = 0x0f,

  OPC_HALT = 0x1f,
};

enum operand_type_e {
  OP_T_NONE = 0x0,
  OP_T_IMM = 0x1,
  OP_T_REG = 0x2,
};

/* All ALU operations writeback to register on x-line */
struct DecodedInstruction {
  const enum eunit_e selected;
  const uint8_t eunit_opcode;
  const enum operand_type_e operand0_type;
  const enum operand_type_e operand1_type;
  const uint16_t operand0;
  const uint16_t operand1;
  const bool set_sf;
};

void cu_fetch_instruction(struct CPUState* const cpustate);

struct DecodedInstruction cu_decode_instruction(struct CPUState* const cpustate);


#endif
