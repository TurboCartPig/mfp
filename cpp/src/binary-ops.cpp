/**
 * @author Dennis Kristiansen
 * @file binary-ops.cpp
 */

#include <iostream>
#include <iomanip>
using namespace std;

// g)
float myfabs(float x) {
    uint32_t n = reinterpret_cast<uint32_t&>(x); // Reinterpret not convert
    n &= ~(1<<31);
    return reinterpret_cast<float&>(n);
}

int main() {
    // Print hex
    uint16_t a = 0xFFEF; // You say you use 32 bits, but only show 16.
    cout << hex << a << endl;

    // OR OPS ----------------------------------------------------------------

    // Set bit nr 10 in a
    // 0x0400 = 0000 0100 0000 0000
    a |= 0x0400;
    cout << hex << a << endl;

    // Set bit nr 12 in a
    a |= 0x1000;
    cout << hex << a << endl;

    // Set bit nr 2 and 5 in a
    a |= 0x0024;
    cout << hex << a << endl;

    // Set all bits except bit 3 in a
    a |= 0xFFF8;
    cout << hex << a << endl;

    // Set all bits in a
    a |= 0xFFFF;
    cout << hex << a << endl;

    // AND OPS ---------------------------------------------------------------

    // Clear bit 4 in a
    a &= 0xFFEF;
    cout << hex << a << endl;

    // Clear bit 12 in a
    a &= 0xEFFF;
    cout << hex << a << endl;

    // Clear bit 2 and 5 in a
    a &= 0xFFDB;
    cout << hex << a << endl;

    // Clear all bits except bit 4 in a
    a &= 0x0010;
    cout << hex << a << endl;

    // Clear all bits in a
    a &= 0x0000;
    cout << hex << a << endl;

    // XOR OPS ---------------------------------------------------------------

    // Toggle bits 4 and 5 in a
    a ^= 0x0030;

    // Toggle bit 12 in a
    a ^= 0x1000;

    // Toggle bits 2 and 5 in a
    a ^= 0x0024;

    // Toggle all bits except 4 in a
    a ^= 0xFFEF;

    // Toggle all bits in a
    a ^= 0xFFFF;

    // NOT OPS ---------------------------------------------------------------

    // Compute bitwise not a
    a = ~a;

    // BIT SHIFTING ----------------------------------------------------------

    // Set bits 3 and 13 in a
    a |= (1<<3) | (1<<13);

    // Clear bits 4 and 12 in a
    a &= ~(1<<4) & ~(1<<12);

    // Toggle bits 14 and 11 in a
    a ^= (1<<14) ^ (1<<11);

    // Clear bit 4 and set bit 23 in a
    uint32_t a_2 = 0xABCD; // I known it's cheating, but I'm lazy
    a_2 &= ~(1<<4) | (1<<23);

    // BITSETS ---------------------------------------------------------------

    // if bit 11 is clear
    if (~a & (1<<11)) {}

    // if bit 11 and bit 12 are set
    if (a & 0x0180) {}

    // if bit 2 and bit 4 are different
    if ((a & (1<<2) >> 2) ^ (a & (1<<4) >> 4)) {}

    // if bit 10 is clear and bit 11 is set
    if ((~a & (1<<10)) && (a & (1<<11))) {}

    // ARITHMETIC WITH BITWISE LOGIC -----------------------------------------

    // Multiply a by 128
    a <<= 7;

    // Divide a by 64
    a >>= 6;

    // Calculate a mod 1024
    a &= 0x3FF;

    // FLOATS ----------------------------------------------------------------
    cout << dec << endl;

    // a)
    float fa = 4.5;
    uint32_t n = reinterpret_cast<uint32_t&>(fa);
    n += (1 << 23);
    cout << fa << " * 2 = " << reinterpret_cast<float&>(n) << endl;

    // b)
    n = reinterpret_cast<uint32_t&>(fa);
    n ^= (1<<31);
    cout << fa << ", " << reinterpret_cast<float&>(n) << endl;

    // c)
    n = reinterpret_cast<uint32_t&>(fa);
    n -= (1 << 24);
    cout << fa << " / 4 = " << reinterpret_cast<float&>(n) << endl;

    // d) Something to do with representation and precision and stuff

    // e)


    // g)
    cout << -5.3 << " " << myfabs(-5.3) << endl;

    // WORKING WITH FLOATS ----------------------------------------------------

    // a)
    float x, y, z;

    // Can fail if x or y is NaN or if x + y = NaN
    if (x + y == y + x) {}

    // Can fail if
    if (x + (y + z) == (x + y) + z) {}

    // Can fail if x = NaN, because NaN == NaN is false
    if (x == x) {}

    // b
//    if (abs(x - 0.03) <= epsilon) {}

//    for (float i = 10; fabs(i) <= epsilon; i -= x) {}

    // I guess you could spilt it up into multiple loops instead?
    // so you avoid having high floating numbers and skipping a bunch of numbers?
    // Or start with 10000 * 100000 / -2 and go up to 10000 * 100000 / 2, and get more numbers inbetween?
    for (float i = 0; i < 10000 * 100000; i += x);

    return EXIT_SUCCESS;
}