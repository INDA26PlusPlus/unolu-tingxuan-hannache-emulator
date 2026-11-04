# unolu-tingxuan-hannache-emulator

GP Registers r0-r7
r6 is the stack pointer
r7 is the instruction pointer (points to next instruction)

State & Flags (SF) register
0: Carry (C) flag, 0=no carry/borrow, 1=carry/no borrow
1: Signed Overflow (O) flag
2: Zero (Z) flag
3: Negative (N) flag


5 bit opcode

Instruction format:
mnem
mnem rX (src/dest)
mnem imm
mnem rX rX (src dest)
mnem imm rX (imm dest)

dest is always on the x-line


Instructions V1
imm = hexadecimal | bas10

All opcodes not defined here are reserved

nop
0x00 | 10:0 reserved

mov rX rX
0x01 | 9:6 reserved | src 5:3 | dest 2:0

movl imm rX (clears most significant byte)
0x02 | 10:3 imm | 2:0 dest 

add\[s\] rX rX
0x04 | 1 if s else 0 | 9:6 reserved | src 5:3 | dest 2:0

sub\[s\] rX rX
0x05 | 1 if s else 0 | 9:6 reserved | src 5:3 | dest 2:0

not
0x08 
and
0x09
or
0x0a

lw rX rX
0x0e | 0 | 9:6 reserved | src 5:3 | dest 2:0
sw rX rX
0x0f | 0 | 9:6 reserved | src 5:3 | dest 2:0

b imm
0x10 | 10:0 imm 

bz imm
0x14 | 1 | 9:0 imm 

halt
0x1f | 10:0 reserved


