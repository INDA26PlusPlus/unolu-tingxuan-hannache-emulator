#include "cu.h"

void cu_fetch_instruction(struct CPUState* const cpustate) {
  cpustate->inst_reg = cpustate->memory[cpustate->gp_reg[IP_IDX]];
  cpustate->gp_reg[IP_IDX] += 2;
}



void halt(void) {
  exit(EXIT_SUCCESS);
}

struct DecodedInstruction cu_decode_instruction(struct CPUState* const cpustate) {
  assert(cpustate != NULL);

  enum eunit_e selected = EUNIT_NONE;
  uint8_t eunit_opcode = 0;
  enum operand_type_e operand0_type = OP_T_NONE;
  enum operand_type_e operand1_type = OP_T_NONE;
  uint16_t operand0 = 0;
  uint16_t operand1 = 0;
  bool set_sf = false;

  const uint16_t instruction = cpustate->inst_reg;
  switch ((enum opcode_e)(instruction>>11)) {
    case OPC_NOP:
      break;
    case OPC_MOV:
      selected = EUNIT_ALU;
      eunit_opcode = ALUOPC_PASSTHROUGH;
      operand0 = instruction&0x7;
      operand0_type = OP_T_REG;
      operand1 = (instruction>>3)&0x7;
      operand1_type = OP_T_REG;
      break;
    case OPC_MOVL:
      selected = EUNIT_ALU;
      eunit_opcode = ALUOPC_PASSTHROUGH;
      operand0 = instruction&0x7;
      operand0_type = OP_T_REG;
      operand1 = (instruction>>3)&0xff;
      operand1_type = OP_T_IMM;
      break;
    case OPC_ADD:
      selected = EUNIT_ALU;
      eunit_opcode = ALUOPC_PASSTHROUGH;
      operand0 = (instruction>>3)&0x7;
      operand0_type = OP_T_REG;
      set_sf = instruction&(1<<10);
      break;
    case OPC_HALT:
      halt();
      break;
    default:
      assert(false);
      break;
  }

  return (struct DecodedInstruction){
    .selected = selected,
    .eunit_opcode = eunit_opcode,
    .operand0_type = operand0_type,
    .operand1_type = operand1_type,
    .operand0 = operand0,
    .operand1 = operand1,
    .set_sf = set_sf,
  };
}
