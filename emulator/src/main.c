#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "cpustate.h"
#include "cu.h"
#include "eunits.h"

int main(int argc, char** argv) {
  bool memdump = false;
  switch (argc) {
    case 3:
      memdump = true;
    case 2:
      break;
    default:
      perror("Expected 1 or 2 args!\n");
      return 1;
  }

  struct CPUState cpustate;  
  memset(&cpustate, 0, sizeof(struct CPUState));

  {
    char* const mem_file_path = argv[1];
    FILE* const mem_file = fopen(mem_file_path, "r");
    assert(mem_file != NULL);
    fread(cpustate.memory, 1, 0x10000, mem_file);
    fclose(mem_file);
  }

  cpustate.gp_reg[7] = 0x1000;
  while (true) {
    cu_fetch_instruction(&cpustate);
    printf("%04hx ", cpustate.gp_reg[7] - 2);
    printf("i=%04hx ", cpustate.inst_reg);
    struct DecodedInstruction dec_inst = cu_decode_instruction(&cpustate);
    uint16_t x = 0;
    uint16_t y = 0;

    switch (dec_inst.operand0_type) {
      case OP_T_IMM:
        x = dec_inst.operand0;
        break;
      case OP_T_REG:
        x = cpustate_get_gp_reg(&cpustate, dec_inst.operand0);
        break;
      case OP_T_NONE:
        break;
      default:
        assert(false);
    }
    switch (dec_inst.operand1_type) {
      case OP_T_IMM:
        y = dec_inst.operand1;
        break;
      case OP_T_REG:
        y = cpustate_get_gp_reg(&cpustate, dec_inst.operand1);
        break;
      case OP_T_NONE:
        break;
      default:
        assert(false);
    }

    printf("r0: %04hx\n", cpustate.gp_reg[0]);
    printf("r1: %04hx\n", cpustate.gp_reg[1]);
    printf("r2: %04hx\n", cpustate.gp_reg[2]);
    printf("r3: %04hx\n", cpustate.gp_reg[3]);
    printf("r4: %04hx\n", cpustate.gp_reg[4]);
    printf("r5: %04hx\n", cpustate.gp_reg[5]);
    printf("r6: %04hx\n", cpustate.gp_reg[6]);
    printf("r7: %04hx\n", cpustate.gp_reg[7]);

    switch (dec_inst.selected) {
      case EUNIT_ALU:
        {
          struct ALUOutput alu_output = alu_execute(dec_inst.eunit_opcode, x, y, cpustate.sf_reg);
          x = alu_output.r;
          if (dec_inst.set_sf) {
            cpustate_set_sf(&cpustate, alu_output.sf);
          }
        }
        break;
      case EUNIT_LSU:
        switch (dec_inst.eunit_opcode & 0x2) {
          case LSU_MODE_LOAD:
            x = lsu_load(&cpustate, dec_inst.eunit_opcode&0x1, y);
            break;
          case LSU_MODE_STORE:
            lsu_store(&cpustate, dec_inst.eunit_opcode&0x1, x, y);
            break;
          default:
            assert(false);
        }
        break;
      case EUNIT_BU:
        x = bu_execute(dec_inst.eunit_opcode, cpustate.sf_reg, x, y);
        break;
      case EUNIT_NONE:
        if (dec_inst.eunit_opcode == 0xff) {
          goto halt;
        }
        break;
      default:
        assert(false);
    }
    if (dec_inst.operand0_type == OP_T_REG && !(dec_inst.selected == EUNIT_LSU && (dec_inst.eunit_opcode & 0x2) == LSU_MODE_STORE)) {
      printf("WRITEBAKC\n");
      cpustate_set_gp_reg(&cpustate, x, dec_inst.operand0);
    }
    
  }
  halt:

  if (memdump) {
    char* const memdump_file_path = argv[2];
    FILE* const memdump_file = fopen(memdump_file_path, "w");
    assert(memdump_file != NULL);
    fwrite(&cpustate.memory, 1, 0x10000, memdump_file);
    fclose(memdump_file);
    printf("r0: %04hx\n", cpustate.gp_reg[0]);
    printf("r1: %04hx\n", cpustate.gp_reg[1]);
    printf("r2: %04hx\n", cpustate.gp_reg[2]);
    printf("r3: %04hx\n", cpustate.gp_reg[3]);
    printf("r4: %04hx\n", cpustate.gp_reg[4]);
    printf("r5: %04hx\n", cpustate.gp_reg[5]);
    printf("r6: %04hx\n", cpustate.gp_reg[6]);
    printf("r7: %04hx\n", cpustate.gp_reg[7]);
    printf("sf: %04hx\n", cpustate.sf_reg);
    printf("in: %04hx\n", cpustate.inst_reg);
  }

  return 0;
}
