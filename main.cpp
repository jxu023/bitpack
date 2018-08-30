#include <iostream>
#include <cmath>
#include <vector>
#include <stdio.h>

using namespace std;

inline bool isPowOf2(int x) {
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

vector<unsigned char> bitpack(const vector<int> & ranges, const vector<vector<int>> & data) {
    // transform ranges into numbits
    int m = data.size();
    int n = ranges.size();

    vector<unsigned> numbits(n);
    for (int i = 0; i < ranges.size(); ++i) {
        numbits[i] = (unsigned)!isPowOf2(ranges[i]) + (unsigned)log2(ranges[i]);
    }

    vector<unsigned char> packed; // vector of bytes
    unsigned char byte = 0x00; // current byte being packed
    int numEmptyBits = 8; // number of empty bits in byte

    for (int i = 0; i < m; ++i) { // data obj
        for (int j = 0; j < n; ++j) { // data feat
            int datum = data[i][j];
            int bitsToMove = numbits[j]; // move from data to byte
            while(bitsToMove) {
                // calc numBits to move now
                int curToMove = min(numEmptyBits, bitsToMove);
                // move bits here
                byte = byte | ((unsigned char)(datum) & mask[curToMove-1]) << (numEmptyBits - curToMove);
                datum >> curToMove;

                // bits were moved
                numEmptyBits -= curToMove;
                bitsToMove -= curToMove;

                // current byte is full
                if (!numEmptyBits) {
                    numEmptyBits = 8;
                    packed.push_back(byte);
                    byte = 0x00;
                }
            }
        }
    }
    return packed;
}

vector<vector<int>> unpack(vector<int> & ranges, vector<unsigned char> & packed, int m) {
    // transform ranges into numbits
    int n = ranges.size();

    vector<unsigned> numbits(n);
    for (int i = 0; i < ranges.size(); ++i) {
        numbits[i] = (unsigned)!isPowOf2(ranges[i]) + (unsigned)log2(ranges[i]);
    }

    vector<vector<int>> unpacked;

    int k = 0; // index into packed, byte = packed[k]
    int bitsRemaining = 8; // how many bits in byte are not copied yet

    // feat was originally: A-B-C where A,B,C are bits with A being some remainder of 8 and B,C being 8 bits
    // the order of copied bits in byte then becomes C-B-A .. need to shift those bits back to A-B-C
    //
    // src to dst
    // section -- A,B,C where B and C are 8 bits, A is numFeatBits % 8
    // sectionBitsRemaining -- num bits within an individual section that still need to be copied
    // byteBitsRemaining -- num bits left within a byte to copy before switching to next byte 
    // chunk -- num bits to move at once = min(sectionBitsRemaining, byteBitsRemaining)
    //
    //
    // s - start, e - end, u - uncopied bit, c - copied bit
    //
    // src --         cccsuuueuu -- state of some packed byte
    //      for src, mask out ccc in the front and any uu after e
    //      shift masked src to left by src start - dst start onto dst
    //
    // dst -- usuuueuuucccccccccc -- state of some unpacked feat
    //      increase src start by chunk
    //          if src start is now out of byte, then switch to next byte
    //      increase dst start by chunk
    //          if dst start is now onto ccc.. then switch to next section
    //              if this was last section of feat, feat switch to next feat
    //
    //  
    //
    for (int i = 0; i < m; ++i) { // for each object
        vector<int> feats;
        for (int j = 0; j < n; ++j) { // obj feat
            int feat = 0; // current feat to be added to unpacked
            int remFeatBits = numbits[j]; // obj feat

            int copiedSectionBits = 0;
            int copySectionStart = min(8, remFeatBits) + copiedSectionBits;

            dst = dst | src >> 

            feats.push_back(feat);
        }
        unpacked.push_back(feats);
    }

    return {};
}

template <class mytype>
void printV(vector<mytype> & v) {
    for (auto x : v) {
        cout << (int)x << ' ';
    }
    cout << '\n';
}

void printVV(vector<vector<int>> & vv) {
    int m = vv.size();
    if (!m) return;
    int n = vv[0].size();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << vv[i][j] << ' ';
        }
        cout << '\n';
    }
}

int main() {
    // year, month, day, hour, minute, sec
    vector<int> ranges = {3000, 12, 31, 24, 60, 60};

    int m = 100;
    int n = ranges.size();

    srand(13);
    vector<vector<int>> data(m, vector<int>(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            data[i][j] = rand() % ranges[j];
        }
    }
    printVV(data);
    auto packed = bitpack(ranges, data);
    cout << "\n\n\n\n";
    printV(packed);
    cout << "\n\n\n\n";
    auto unpacked = unpack(ranges, packed, m);
    printVV(unpacked);

    bool passedTestCase = true;
    if (unpacked.size() != m || unpacked[0].size() != n) {
        passedTestCase = false;
        cout << "a size of unpacked is wrong\n";
    }
    else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
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
