#include "cu.h"
#include "cpustate.h"
#include "eunits.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

void cu_fetch_instruction(struct CPUState* const cpustate) {
  assert(cpustate->gp_reg[IP_IDX] % 2 == 0); // ensure pointer is word-aligned
  cpustate->inst_reg = ((uint16_t*)cpustate->memory)[cpustate->gp_reg[IP_IDX]/2];
  cpustate->gp_reg[IP_IDX] += 2;
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
      printf("NOP\n");
      break;
    case OPC_MOV:
      printf("MOV\n");
      selected = EUNIT_ALU;
      eunit_opcode = ALUOPC_PASSTHROUGH;
      operand0 = instruction&0x7;
      operand0_type = OP_T_REG;
      operand1 = (instruction>>3)&0x7;
      operand1_type = OP_T_REG;
      break;
    case OPC_MOVL:
      printf("MOVL\n");
      selected = EUNIT_ALU;
      eunit_opcode = ALUOPC_PASSTHROUGH;
      operand0 = instruction&0x7;
      operand0_type = OP_T_REG;
      operand1 = (instruction>>3)&0xff;
      operand1_type = OP_T_IMM;
      break;
    // case OPC_MOVU:
    //   printf("MOVU\n");
    //   selected = EUNIT_ALU;
    //   eunit_opcode = ALUOPC_PASSTHROUGH;
    //   operand0 = instruction&0x7;
    //   operand0_type = OP_T_REG;
    //   operand1 = (instruction>>3)&0xff;
    //   operand1_type = OP_T_IMM;
    //   break;
    case OPC_ADD:
      printf("ADD\n");
      selected = EUNIT_ALU;
      eunit_opcode = ALUOPC_ADD;
      operand0 = instruction&0x7;
      operand0_type = OP_T_REG;
      operand1 = (instruction>>3)&0x7;
      operand1_type = OP_T_REG;
      set_sf = instruction&(1<<10);
      break;
    // sub\[s\] rX rX
    // 0x05 | 1 if s else 0 | 9:6 reserved | src 5:3 | dest 2:0
    case OPC_SUB:
      printf("SUB\n");
      selected = EUNIT_ALU;
      eunit_opcode = ALUOPC_SUB;
      operand0 = instruction&0x7; // ..00111 last three
      operand0_type = OP_T_REG;
      operand1 = (instruction>>3)&0x7;
      operand1_type = OP_T_REG;
      set_sf = instruction&(1<<10);
      break;
    // or rX rX
    // 0x0a | 9:6 reserved | src 5:3 | dest 2:0
    case OPC_OR:
      printf("OR\n");
      selected = EUNIT_ALU;
      eunit_opcode = ALUOPC_OR;
      operand0 = instruction&0x7; // ..00111 last three
      operand0_type = OP_T_REG;
      operand1 = (instruction>>3)&0x7;
      operand1_type = OP_T_REG;
      break;

    case OPC_HALT:
      printf("HALT\n");
      eunit_opcode = 0xff;
      break;
    case OPC_LW:
      printf("LW\n");
      selected = EUNIT_LSU;
      eunit_opcode = LSU_OPSZ_WORD | LSU_MODE_LOAD;
      operand0 = instruction&0x7;
      operand0_type = OP_T_REG;
      operand1 = (instruction>>3)&0x7;
      operand1_type = OP_T_REG;
      break;
    case OPC_SW:
      printf("SW\n");
      selected = EUNIT_LSU;
      eunit_opcode = LSU_OPSZ_WORD | LSU_MODE_STORE;
      operand0 = instruction&0x7;
      operand0_type = OP_T_REG;
      operand1 = (instruction>>3)&0x7;
      operand1_type = OP_T_REG;
      break;
    default:
      fprintf(stderr, "Invalid opcode %02hhx (inst=%04hx)\n", 5, instruction);
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
