#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void print32(int num) {
    string s;
    for (int i = 0; i < 32; ++i) {
        s += (num & 1) ? '1' : '0';
        num >>= 1;
    }
    cout << '\n';
    cout << string(s.rbegin(), s.rend()) << '\n';
}
void print8(unsigned char num) {
    string s;
    for (int i = 0; i < 8; ++i) {
        s += (num & 1) ? '1' : '0';
        num >>= 1;
    }
    cout << '\n';
    cout << string(s.rbegin(), s.rend()) << '\n';
}
const vector<unsigned char> mask = {
    0x01, // 000000001
    0x03, //  00000011
    0x07,
    0x0F,
    0x1F,
    0x3F,
    0x7F,
    0xFF //  11111111
};

int main() {
    int numbits; // numbits to move
    // 1111 1010
    unsigned src = 0xFA; // packed will be start moving numbits bits from the left
    int srcBitsMovedL;
    int dstBitsMovedR;
    int dst = 0x0F0F; // unpacked will be 
    print32(dst);

    dst = dst | (int)((src & maskR[srcBitsMovedR]) >> (8 - (srcBitsMovedR + curToMove))) << dstBitsMovedR;

    dst = (dst | (src & 0x0F) << 4);

    print8(src);
    print32(dst);
}
