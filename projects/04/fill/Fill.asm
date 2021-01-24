// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

// pseudo code
// LOOP:
//   fill = 0
//   if KBD == 0:
//       goto PAINT
//   fill = -1
// PAINT:
//   i = 0
//   while i < 8192:
//       SCREEN[i] = fill
//       i++
//   goto LOOP

(LOOP)
  @fill
  M=0
  @KBD
  D=M
  @PAINT
  D;JEQ

  @fill
  M=-1

(PAINT)
  @i
  M=0

  @SCREEN
  D=A
  @pointer
  M=D
(WHILE)
  @8192
  D=A
  @i
  D=D-M
  @LOOP
  D;JEQ

  @fill
  D=M
  @pointer
  A=M
  M=D
  @pointer
  M=M+1
  
  @i
  M=M+1
  @WHILE
  0;JMP
  
  
