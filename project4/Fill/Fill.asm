// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// So we have like a main function to decide:
// -> Which colour we are filling depend on the keystroke before we jump to a DRAW function

// Inside DRAW function we define: 
// -> a counter 
// -> where to start @address

// Go to a DRAWLOOP:
// -> so we essentially want to fill up the whole screen with @color before we jump back to MAIN function to decide which new colour we are filling
// -> Start with the @address we have
// -> set its value to -1
// -> Go to the next section of the screen
// -> Decrease the counter

(MAIN)
  @KBD
  D=M
  @SET_BLACK
  D;JNE
  @colour
  M=0
  @DRAW
  0;JMP
(SET_BLACK)
  @colour 
  M=-1

(DRAW)
  // Set counter
  @8192
  D=A
  @COUNTER
  M=D
  // Set address for where to start
  @SCREEN
  D=A
  @ADDRESS
  M=D

(DRAWLOOP)
  @colour
  D=M
  @ADDRESS
  A=M
  M=D // set the colour
  
  // Increase to the next screen section
  @ADDRESS
  M=M+1

  // Decrease pointer
  @COUNTER
  MD=M-1
  @DRAWLOOP
  D;JGT

  @MAIN
  0;JMP

