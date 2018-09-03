#include <iostream>
#include <cmath>
#include <vector>
#include <stdio.h>

using namespace std;

inline bool isPowOf2(unsigned x) {
    return !(x & (x-1));
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

const vector<unsigned char> maskR = {
    0xFF, // 11111111
    0x7F, // 01111111
    0x3F, // 00111111
    0x1F, // 00011111
    0x0F, // 00001111
    0x07, // 
    0x03,
    0x01,
};

vector<unsigned char> bitpack(const vector<unsigned> & ranges, const vector<vector<unsigned>> & data) {
    // transform ranges into unsigned numbits
    unsigned m = data.size();
    unsigned n = ranges.size();

    vector<unsigned> numbits(n);
    for (unsigned i = 0; i < ranges.size(); ++i) {
        numbits[i] = (unsigned)!isPowOf2(ranges[i]) + (unsigned)log2(ranges[i]);
    }

    vector<unsigned char> packed; // vector of bytes
    unsigned char byte = 0x00; // current byte being packed
    unsigned numEmptyBits = 8; // number of empty bits in byte

    for (unsigned i = 0; i < m; ++i) { // data obj
        for (unsigned j = 0; j < n; ++j) { // data feat
            unsigned datum = data[i][j];
            unsigned bitsToMove = numbits[j]; // move from data to byte
            while(bitsToMove) {
                unsigned curToMove = min(numEmptyBits, bitsToMove);

                byte = byte | ((unsigned char)(datum) & mask[curToMove-1]) << (numEmptyBits - curToMove);
                datum >>= curToMove;

                numEmptyBits -= curToMove;
                if (!numEmptyBits) {
                    numEmptyBits = 8;
                    packed.push_back(byte);
                    byte = 0x00;
                }

                bitsToMove -= curToMove;
            }
        }
    }
    //if (numEmptyBits < 8) {
    if (1) {
        packed.push_back(byte);
    }
    return packed;
}

vector<vector<unsigned>> unpack(const vector<unsigned> & ranges, const vector<unsigned char> & packed, unsigned m) {
    unsigned n = ranges.size();
    vector<unsigned> numbits(n);
    for (unsigned i = 0; i < ranges.size(); ++i) {
        numbits[i] = (unsigned)!isPowOf2(ranges[i]) + (unsigned)log2(ranges[i]);
    }

    vector<vector<unsigned>> unpacked;

    unsigned k = 0; // index unsignedo packed, byte = packed[k]
    unsigned char byte = packed[k++];
    unsigned byteBitsMovedL = 0;

    for (unsigned i = 0; i < m; ++i) {
        vector<unsigned> feats;
        for (unsigned j = 0; j < n; ++j) {
            unsigned feat = 0;
            unsigned featBitsMovedR = 0;

            while (featBitsMovedR < numbits[j]) {
                unsigned curToMove = min((unsigned)(8 - (unsigned)byteBitsMovedL), numbits[j] - featBitsMovedR);

                feat |= ((unsigned)((byte & maskR[byteBitsMovedL])) >> (8 - (byteBitsMovedL + curToMove))) << featBitsMovedR;
                //cout << "from byte: " << (unsigned)byte << ", we moved " << curToMove << " bits just now onto " << feat << " \n";

                byteBitsMovedL += curToMove;
                if (byteBitsMovedL == 8) {
                    byteBitsMovedL = 0;
                    byte = packed[k++];
                    //cout << "\nnext byte\n";
                    if (k > packed.size()) {
                        cout << "\n\n\n" << "k of " << k << " is too big\n";
                        cout << i << ' ' << j << "\n\n";
                        exit(1);
                    }
                }

                featBitsMovedR += curToMove;
            }
            //cout << "\nfeat: " << feat << '\n';
            feats.push_back(feat);
        }
        unpacked.push_back(feats);
        //cout << '\n';
    }

    cout << "finished unpacking\n\n";
    return unpacked;
}

template <class mytype>
void printV(vector<mytype> & v) {
    for (auto x : v) {
        cout << (unsigned)x << ' ';
    }
    cout << '\n';
}

void printVV(vector<vector<unsigned>> & vv) {
    unsigned m = vv.size();
    if (!m) return;
    unsigned n = vv[0].size();
    for (unsigned i = 0; i < m; ++i) {
        for (unsigned j = 0; j < n; ++j) {
            cout << vv[i][j] << ' ';
        }
        cout << '\n';
    }
}

int main() {
    // year, month, day, hour, minute, sec
    unsigned m = 100;
    vector<unsigned> ranges = {3000, 12, 31, 24, 60, 60};
    unsigned n = ranges.size();
    int total = 0;
    for (unsigned i = 0; i < ranges.size(); ++i) {
        int bits = (unsigned)!isPowOf2(ranges[i]) + (unsigned)log2(ranges[i]);
        cout << bits << ' ';
        total += bits;
    }
    cout << "\n" << total << " bits per obj\n";
    cout << "should have " << (m*total)/8 << " bytes in total\n\n";

    srand(13);
    // create data
    vector<vector<unsigned>> data(m, vector<unsigned>(n));
    for (unsigned i = 0; i < m; ++i) {
        for (unsigned j = 0; j < n; ++j) {
            data[i][j] = rand() % ranges[j];
        }
    }
    printVV(data);

    // pack data
    auto packed = bitpack(ranges, data);
    cout << "packed:\n";
    printV(packed);
    cout << "\n";
    cout << "packed has size " << packed.size() << "\n\n";

    // unpack data
    vector<vector<unsigned>> unpacked = unpack(ranges, packed, m);
    cout << "unpacked:\n";
    printVV(unpacked);

    bool passedTestCase = true;
    if (unpacked.size() != m || unpacked[0].size() != n) {
        passedTestCase = false;
        cout << "a size of unpacked is wrong\n";
    }
    else {
        for (unsigned i = 0; i < m; ++i) {
            for (unsigned j = 0; j < n; ++j) {
                if (unpacked[i][j] != data[i][j]) {
                    cout << "i j is " << i << ' ' << j << '\n';
                    cout << unpacked[i][j] << ' ' << data[i][j] << '\n';
                    cout << "the above does not match";
                    passedTestCase = false;
                    break;
                }
            }
        }
    }
    if (passedTestCase) {
        cout << "the test case was passed!\n";
    }

    return 0;
}
