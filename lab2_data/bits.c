/* 
 * INF559 Data Lab 
 * 

Name:   Vrushank Agrawal
UserID: vrushank.agrawal

 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif

/* Copyright (C) 1991-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* bit manipulation */
/*
 * func1 - returns 1 if x == 0, and 0 otherwise 
 *   Examples: func1(5) = 0, func1(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int func1(int x) {
  /* Using trivial */ 
  return !x;
}
/* 
 * func2 - ~(x|y) using only ~ and & 
 *   Example: func2(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int func2(int x, int y) {
  /* Consequence of truth table */ 
  return (~x & ~y);
}
/* 
 * func3 - x&~y using only ^ and & 
 *   Example: func3(1,2) = 1, func3(0,1) = 0, 0 
 *   Legal ops: & | ^
 *   Max ops: 8
 *   Rating: 1
 */
int func3(int x, int y) {
  /* Consequence of truth table */ 
  return (x & (x ^ y));
}
/* 
 * func4 - swap the first 16 bits of x with the last 16 bits of x
 *   Examples: func4(0x87654321) = 0x08765432, func4(0x12345678) = 0x56781234
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 2
 */
int func4(int x) {
  /* We logical shift the last 16 bits to left 
  *  then arithmetic shift first 16 bits to right
  *  and use `&` to pad the first 16 bits with 0 
  *  Then using | we can return the swapped word
  */ 
  int first = x<<16;
  int last = x>>16 & 0x0000FFFF;
  return first | last;
}
/* 
 * func5 - set all bits of result to least significant bit of x
 *   Example: func5(5) = 0xFFFFFFFF, func5(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int func5(int x) {
  /* We do a logical shift of 31 to get the LSB to the MSB then
  *  we do an arithmetic shift of 31 to get the desired result */ 
  return (x << 31) >> 31;
}
/* 
 * func6 - return 1 if all even-numbered bits in word set to 1
 *   Examples func6(0xFFFFFFFE) = 0, func6(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int func6(int x) {
  /* We can store the values of even digits in `all_even` using 
  *  `x & even` (even is number with only even digits set to 1)  
  *  then we use `neg_even` and add it to `all_even` which will be
  *  zero iff all even digits in x are set to 1 
  */ 
  int even = 0x55555555;
  int all_even = x & even;
  int neg_even = (~even) + 1;
  return !(neg_even + all_even);
}
/* 
 * func7 - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: func7(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int func7(int x, int n) {
  /* We first check using !!n if the input is zero, if so
  *  then LSB is zero and we do not need to shift any bits.
  *  In `n_zeros`, we pad the first n bits with 0 to convert
  *  arithmetic shift into logical shift.
  */ 
  int check_zero = !!n << 31;
  int n_zeros = (~(check_zero>>n)<<1) + 1;
  return x>>n & n_zeros;
}

/* 2's complement */
/* 
 * func8 - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int func8(void) {
  /* Using trivial */ 
  return 1<<31;
}
/*
 * func9 - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int func9(int x) {
  /* If x is maximum two's complement number then
  *  there will be an overflow for (x+x+2). Another
  *  time when all bits will be zero is if x = -1
  */
  return !(x+x+2) & !!(~x);
}
/* 
 * func10 - return -x 
 *   Example: func10(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int func10(int x) {
  /* Using trivial */ 
  return ~x + 1;
}
/* 
 * func11 - Determine if can compute x+y without overflow
 *   Example: func11(0x80000000,0x80000000) = 0,
 *            func11(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int func11(int x, int y) {
  /* We need to ensure two conditions for no overflow:
  *  if MSB of ints are of different signs or if after
  *  addition MSB of the sum of the ints is same.
  */
  int sum = (x+y)>>31;
  x>>=31; y>>=31;
  int not_same_sign = !!(x^y);
  int change_sign = (x^sum) & (y^sum);
  return not_same_sign | !change_sign;
}
/* 
 * func12 - if x > y  then return 1, else return 0 
 *   Example: func12(4,5) = 0, func12(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int func12(int x, int y) {
  /* By simpl */ 
  // return x>y;
  int minus_y = func10(y);
  int x_sign = x>>31; 
  int y_sign = y>>31;
  int same_sign_xy = !(x_sign^y_sign);                          // true if same sign
  int take_diff = !!(x^y) & same_sign_xy;                      // take diff if not same int
  int pos_difference = take_diff & !((x+minus_y)>>31);        // 1 if positive difference
  int x_greater = !same_sign_xy & !x_sign;                   // true if x is greater and x, y have different signs
  return pos_difference + x_greater;
}
/*
 * func13 - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum positive value.
 *   Examples: func13(0x40000000,0x40000000) = 0x7fffffff
 *             func13(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int func13(int x, int y) {
  /* Essentially, we need to do three things here. 
  *  First we check for overflow. Second, if there
  *  is no overflow and sign of ints is different,
  *  then we do sum. Third, if there is overflow and
  *  it is positive, we add max otherwise min. 
  */
  int xplusy = x+y;
  int x_sign = x>>31; 
  int y_sign = y>>31;
  int min_int = 1<<31;
  int sign_sum = xplusy>>31;
  int overflow = (x_sign^sign_sum) & (y_sign^sign_sum);         // checks for overflow
  int same_sign_xy = !(x_sign^y_sign);                          // true if same sign
  int sum = ((!same_sign_xy | !overflow)<<31)>>31 & xplusy;     // if sign is same then sum=0
  int pos_overflow = same_sign_xy & (!x_sign);                  // checks for positive overflow
  int max = ((overflow & pos_overflow)<<31)>>31 & (~min_int);   // sets max
  int min = ((overflow & !pos_overflow)<<31)>>31 & min_int;     // sets min
  return sum + max + min;
}