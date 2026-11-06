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

0001 0111 1111 1000 movl 0xff r0
0001 0000 0001 0001 movl 0x2 r1
0010 0000 0000 1000 add r1 r0
1111 1000 0000 0000 halt

0000100000111000 mov r7 r0 (0x1002)
0001000010000001 movl 16 r1
0010010000001000 adds r1 r0
0111000000000010 lw r0 r2
0001000000010001 movl 2 r1
0010010000001000 adds r1 r0
0111100000000010 sw r0 r2
1111100000000000 halt
1111000011110000 data 0xf0f0

nop
0x00 | 10:0 reserved

mov rX rX
0x01 | 9:6 reserved | src 5:3 | dest 2:0

movl imm rX (clears most significant byte)
0x02 | 10:3 imm | 2:0 dest 

movu imm rX 
0x03 | 10:3 imm | 2:0 dest 

add\[s\] rX rX
0x04 | 1 if s else 0 | 9:6 reserved | src 5:3 | dest 2:0

sub\[s\] rX rX
0x05 | 1 if s else 0 | 9:6 reserved | src 5:3 | dest 2:0

not rX
0x08 | 9:3 reserved | dest 2:0
and rX rX
0x09 | 9:6 reserved | src 5:3 | dest 2:0
or rX rX
0x0a | 9:6 reserved | src 5:3 | dest 2:0

lb rX rX (zeroes upper byte)
0x0c | 0 | 9:6 reserved | src 5:3 | dest 2:0
sb rX rX (stores lower byte)
0x0d | 0 | 9:6 reserved | src 5:3 | dest 2:0

lw rX rX
0x0e | 0 | 9:6 reserved | src 5:3 | dest 2:0
sw rX rX
0x0f | 0 | 9:6 reserved | src 5:3 | dest 2:0


b imm
0x10 | 10:0 imm (signed) 

bc imm (branch carry)
0x11 | 10:0 imm (signed)

bz imm
0x14 | 10:0 imm (signed)

halt
0x1f | 10:0 reserved


