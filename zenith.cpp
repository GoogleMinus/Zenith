#include <stdio.h>
#include <assert.h>

// Type definition for bitboards.
typedef unsigned long long U64;

// Rank constants (1..8).
const U64 Ranks[8] = {
    0xff,
    0xff00,
    0xff0000,
    0xff000000,
    0xff00000000,
    0xff0000000000,
    0xff000000000000,
    0xff00000000000000
};

// File constants (A..H).
const U64 Files[8] = {
    0x8080808080808080,
    0x4040404040404040,
    0x2020202020202020,
    0x1010101010101010,
    0x0808080808080808,
    0x0404040404040404,
    0x0202020202020202,
    0x0101010101010101
};

// White Pieces
U64 whitePawns = 0xff00;
U64 whiteKnights = 0x42;
U64 whiteBishops = 0x24;
U64 whiteRooks = 0x81;
U64 whiteQueens = 0x10;
U64 whiteKing = 0x8;

// Black Pieces
U64 blackPawns = 0xff000000000000;
U64 blackKnights = 0x4200000000000000;
U64 blackBishops = 0x2400000000000000;
U64 blackRooks = 0x8100000000000000;
U64 blackQueens = 0x1000000000000000;
U64 blackKing = 0x800000000000000;

int main() {
    initPopCountByte();
}

/* POPULATION COUNT: The number of one bits in a given bitboard.
 * There is a recurrence relation of population counts (f) given by
 * f(n) = 0                             (if n = 0)
 * f(n) = f(floor(n / 2)) + (n mod 2)   (if n > 0).
 */

/* Lookup table for population count of any byte value. popCountByte[i] = population count of i. */
unsigned char popCountByte[256];

/**
 * Initializes the population count of all possible byte values.
 */
void initPopCountByte() {
    popCountByte[0] = 0;
    for (int i = 1; i < 256; i++) {
        popCountByte[i] = popCountByte[i / 2] + (i & 1);
    }
}

/**
 * Calculates the population count of a given bitboard.
 * @param bb The bitboard to be used for population count calculation.
 * @return The population count of the given bitboard.
 */
int popCount(U64 bb) {
    int s;  // Cumulative population count
    for (int i = 0; i < 8; i++) {
        s += popCountByte[bb & 0xff];
        bb = bb >> 8;
    }
    return s;
}

/* BIT SCANNING (FORWARD): Detecting the index of the least significant one bit. */

/* Lookup table for bit scanning. */
const int bsfIndex64[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

/**
 * Does a forward bitscan of a bitboard. Assumes the bitboard is not empty.
 * @author Kim Walisch (2012)
 * @param bb The bitboard to scan.
 * @return The index (0..63) of the least significant one bit.
 */
int bitScanForward(U64 bb) {
   const U64 debruijn64 = 0x03f79d71b4cb0a89;
   assert (bb != 0);
   return bsfIndex64[((bb ^ (bb-1)) * debruijn64) >> 58];   // It's best not to ask why.
}