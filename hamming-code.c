/* 
   Isaac Ganoung
   Computer Organization
   2021-09-16
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h> /* For random number generator seed */
#include <stdbool.h> /* For bool, true, false */
#include <stdint.h> /* 16 bit integer */

uint16_t getInput();
/* Get user input, convert into 16 bit unsigned int. */

void createParities(short bits[], uint16_t x);
/* Create the array, including parity bits. */

void flipRandom(short bits[]);
/* Flip a random bit in the array. */

void checkParities(short bits[]);
/* Check the parity bits, correct up to one flipped bit. */

void printArray(short bits[]);
/* Print the array. */

int main(void) {
  short bits[21]; /* Shorts make it easier to do math. */
  uint16_t number = getInput();
  printf("Received: 0x%x => %d\n",number, number);
  srand((unsigned) time(NULL));
  createParities(bits, number);
  printArray(bits);
  flipRandom(bits);
  printArray(bits);
  checkParities(bits);
  printArray(bits);
  return 0;
}

uint16_t getInput() {
  bool repeat = true;
  unsigned int input; 
  uint16_t input16; /* This program only needs to support 16 bit unsigned numbers. */
  while (repeat) {
    printf("Please enter a hexedecimal number with a max of 0xffff prefaced by 0x: ");
    scanf("0x%x", &input);
    if (input <= 65535) {
      repeat = false;
    }
    while ((getchar()) != '\n'); /* Clear the input buffer. This prevents infinite loops on bad input. */
  }
  input16 = input;
  return input16;
}

void createParities(short bits[], uint16_t x) {
/*
  Check bits will be inserted at positions 1, 2, 4, 8, 16
  1 checks 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21
  2 checks 2, 3, 6, 7, 10, 11, 14, 15, 18, 21
  4 checks 4, 5, 6, 7, 12, 13, 14, 15, 20, 21
  8 checks 8, 9, 10, 11, 12, 13, 14, 15
  16 checks 16, 17, 18, 19, 20, 21
  Bits are stored in reverse order: bits[0] is least significant.
*/
  bits[2] = x % 2;
  x = x >> 1;
  for (int i = 4; i < 7; i++) {
    bits[i] = x % 2;
    x = x >> 1;
  }
  for (int i = 8; i < 15; i++) {
    bits[i] = x % 2;
    x = x >> 1;
  }
  for (int i = 16; i < 21; i++) {
    bits[i] = x % 2;
    x = x >> 1;
  }
  
  short current;
  /* bits 16 */
  current = bits[16] + bits[17] + bits[18] + bits[19] + bits[20];
  bits[15] = current % 2;
  /* bits 8 */
  current = bits[9] + bits[10] + bits[11] + bits[12] + bits[13] + bits[14];
  bits[7] = current % 2;
  /* bits 4 */
  current = bits[4] + bits[5] + bits[6] + bits[11] + bits[12] + bits[13] + bits[14] + bits[19] + bits[20];
  bits[3] = current % 2;
  /* bits 2 */
  current = bits[5] + bits[6] + bits[9] + bits[10] + bits[13] + bits[14] + bits[17] + bits[20];
  bits[1] = current % 2;
  /* bits 1 */
  current = bits[2] + bits[4] + bits[6] + bits[8] + bits[10] + bits[12] + bits[14] + bits[16] + bits[18] + bits[20];
  bits[0] = current % 2;
}

void flipRandom(short bits[]) {
  int toFlip = rand() % 21;
  bits[toFlip] = !bits[toFlip];
}


void checkParities(short bits[]) {
  short current = 0;
  if (((bits[2] + bits[4] + bits[6] + bits[8] + bits[10] + bits[12] + bits[14] + bits[16] + bits[18] + bits[20]) % 2) != bits[0]) {
    current += 1;
  }
  if (((bits[5] + bits[6] + bits[9] + bits[10] + bits[13] + bits[14] + bits[17] + bits[20]) % 2) != bits[1]) {
    current += 2;
  }
  if (((bits[4] + bits[5] + bits[6] + bits[11] + bits[12] + bits[13] + bits[14] + bits[19] + bits[20]) % 2) != bits[3]) {
    current += 4;
  }
  if (((bits[9] + bits[10] + bits[11] + bits[12] + bits[13] + bits[14]) % 2) != bits[7]) {
    current += 8;
  }
  if (((bits[16] + bits[17] + bits[18] + bits[19] + bits[20]) % 2) != bits[15]) {
    current += 16;
  }
  if (current != 0) {
    bits[current - 1] = !bits[current - 1];
  }
}

void printArray(short bits[]) {
  for (short x = 20; x >= 0; x--) {
    printf("%d",bits[x]);
  }
  printf("\n");
}
