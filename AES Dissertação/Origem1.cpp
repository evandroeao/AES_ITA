#include "keyExpansion.h"
using namespace AES;

void keyExpansion::initTable() {
    ifstream subs_file("subbyte table.txt");
    if (!subs_file.good()) {
        cout << "Please create \"subbyte table.txt\" file with 256 bytes (in hex) "
            << "as a substitution box\n";
        exit(-1);
    }
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++) {
            word input;
            subs_file >> hex >> input;
            // If file is at end, do not throw error
            if ((i != 15 || j != 15) && !subs_file.good()) {
                cout << "Please provide exactly 256 bytes\n";
                exit(-1);
            }
            substitutionTable[i][j] = input;
        }
}

word keyExpansion::subWord(const word& w) const {
    word resword = 0;
    // To retrieve the transform word, substitute four bytes.
    for (int i = 0; i < 4; i++) {
        resword <<= 8;
        byte cur = w >> ((3 - i) * 8);
        resword |= substitutionTable[cur >> 4][cur & 0xF];
    }
    return resword;
}

void keyExpansion::expand(byte key[16], word w[44]) const {
    initExpand(key, w);
    word tmp;
    for (int i = 4; i < 44; i++) {
        tmp = w[i - 1];
        if (i % 4 == 0)
            tmp = subWord(rotWord(tmp)) ^ (word(RC[i / 4 - 1]) << 24);
        w[i] = w[i - 4] ^ tmp;
    }
}