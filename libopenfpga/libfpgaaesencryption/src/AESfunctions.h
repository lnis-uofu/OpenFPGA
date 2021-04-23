#include <iostream>
using namespace std;

// These are AES constants for AES 128, 192, 256
const unsigned short Nb = 4; // columns (could be changed to a larger number in the future)
const unsigned short rows = 4; // rows
const unsigned short stt_lng = Nb * rows; // state length

// This will allocate (memory) resources to accommodate all AES
const unsigned short Nk_max = 8; // =(4, 6 or 8), max [32-bit words] columns in cipher key
const unsigned short CipherKeyLenghth_max = Nk_max * rows; // max bytes in key length
const unsigned short Nr_max = (Nk_max > Nb) ? Nk_max + 6 : Nb + 6; // max number of rounds
const unsigned short ExtdCipherKeyLenghth_max = (Nr_max + 1) * stt_lng; // max bytes in extended key length

void KeyExpansion(unsigned char*, unsigned short, unsigned char*);
void AES_Encrypt(unsigned char*, unsigned char*, unsigned short, unsigned char*);
void AES_Decrypt(unsigned char*, unsigned char*, unsigned short, unsigned char*);