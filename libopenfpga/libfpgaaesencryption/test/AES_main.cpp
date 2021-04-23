#include "AESfunctions.h"

int main()
{
    // These variables allow to change the AES length
	// Nk = 4, 6 or 8 for AES 128, 192 or 256 respectively
	// Only set Nk and the rest will compute according to the AES specification
    unsigned short Nk = 4; // 4 or 6 or 8 [32-bit words] columns in cipher key
    unsigned short CipherKeyLenghth = Nk * rows; // bytes
    unsigned short Nr = (Nk > Nb) ? Nk + 6 : Nb + 6; // = 10, 12 or 14 rounds
    unsigned short ExtdCipherKeyLenghth = (Nr + 1) * stt_lng; // bytes in the expanded cipher key

    cout << "AES with Nb = " << Nb << " columns, Nk = " << Nk << " (32-bit) words i.e. CipherKeyLenghth = "
        << CipherKeyLenghth << " bytes (or " << CipherKeyLenghth * 8 << " bits), Nr = " << Nr << " rounds" << endl << endl;


    // create a dummy test cipher key
    unsigned char key[CipherKeyLenghth_max];
    cout << "key = ";
    for (unsigned short i = 0; i < CipherKeyLenghth; i++) {
        key[i] = (unsigned char)i;
        cout << "0x" << hex << (unsigned short)key[i] << " ";
    } cout << endl << endl;


    // extend key
    unsigned char expandedKey[ExtdCipherKeyLenghth_max];
    KeyExpansion(key, Nk, expandedKey);
    cout << "expandedKey = ";
    for (unsigned short i = 0; i < ExtdCipherKeyLenghth; i++) { cout << dec << (unsigned short)expandedKey[i] << " "; }
    cout << "<=> ";
    for (unsigned short i = 0; i < ExtdCipherKeyLenghth; i++) { cout << "0x" << hex << (unsigned short)expandedKey[i] << " "; }
    cout << endl << endl;


    // create a test input data (plaintext) (ABCDEFGHIJKLMNOP)
    unsigned char plaintext[stt_lng] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P' };
    cout << "plaintext = ";
    for (unsigned short i = 0; i < stt_lng; i++) { cout << plaintext[i] << " "; }
    cout << "<=> ";
    for (unsigned short i = 0; i < stt_lng; i++) { cout << "0x" << hex << (unsigned short)plaintext[i] << " "; }
    cout << endl << endl;


    // encrypt
    unsigned char ciphertext[stt_lng];
    AES_Encrypt(plaintext, expandedKey, Nr, ciphertext);
    cout << "ciphertext = ";
    for (unsigned short i = 0; i < stt_lng; i++) { cout << dec << (unsigned short)ciphertext[i] << " "; }
    cout << "<=> ";
    for (unsigned short i = 0; i < stt_lng; i++) { cout << "0x" << hex << (unsigned short)ciphertext[i] << " "; }
    cout << endl << endl;

/*
    // decrypt
    unsigned char decrypted_plaintext[stt_lng];
    AES_Decrypt(ciphertext, expandedKey, Nr, decrypted_plaintext);
    cout << "decrypted_plaintext = ";
    for (unsigned short i = 0; i < stt_lng; i++) { cout << decrypted_plaintext[i] << " "; }
    cout << "<=> ";
    for (unsigned short i = 0; i < stt_lng; i++) { cout << "0x" << hex << (unsigned short)decrypted_plaintext[i] << " "; }
    cout << endl << endl;
*/
    return 0;
}
