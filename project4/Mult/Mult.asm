// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

// Insert value into R0 and R1
// If either R0 is 0 or R1 is 0 then we won't do anything and produce the value of R2 as 0 (by default the value is 0)
// Replicate R1 -> R2, R0 will become the counter
// Loop, until R0 become 0
// R1 will keep adding the value of itself from R3
  
  // Initilise R2 0
  @R2
  M=0
  
  // Check if R0 == 0
  @R0
  D=M // D = RAM[0]
  @END
  D;JEQ
  // Check if R1 == 0
  @R1
  D=M // D = RAM[1]
  @END
  D;JEQ

(LOOP)
  @R1
  D=M // D = RAM[1]
  @R2 
  M=D+M // RAM[2] = RAM[2] + RAM[1]
  // Decrease R0 because it is the counter
  @R0
  MD=M-1 // RAM[0] = RAM[0] - 1 and D = RAM[0] - 1 

  @LOOP
  D;JGT

(END)
  @END
  0;JMP
