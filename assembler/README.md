# Assembler 

Comments are single line and start with ; 

## Directives
@align n
set current_pos to -1*(-1*current_pos & ((1<<current_pos)-1))
@org a
set current_pos to a

## Data directive
%db v
Define byte with value v

%dw v
Define word with value v

%fb v, n
Fill with n bytes with value v

%fw v, n
Fill with n words with value v


## Instructiosn
Instruction format:

```
mnem
mnem rX (src/dest)
mnem imm
mnem rX rX (src dest)
mnem imm rX (imm dest)
```

imm = hexadecimal | bas10

rX = r \[0-7\]

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

not rX rX

0x08 

and rX rX

0x09

or rX rX

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
