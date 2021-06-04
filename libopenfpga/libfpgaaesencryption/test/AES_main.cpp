#include "AESfunctions.h"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include<cctype>

//String conversion function
string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

//Function to convert binary to Decimal

int binaryToDecimal(int n)
{
    int num = n;
 int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
     int base = 1;

    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }

    return dec_value;
}

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


streampos size;
char *data;
unsigned int plaintext[16];
int j=0;
int k;
int newsize;
int y;
    
//Read input bitstream    
ifstream file ("fabric_bitstream.txt",ios::in|ios::ate);
if (file.is_open())
{
size=file.tellg();
data=new char [size];
file.seekg(0,ios::beg);
file.read(data,size);
file.close();

}
else cout<<"Unable to open file";
std::string s,new_s;
std::stringstream ss;
ss << data;
s = ss.str();
char new_data[4];
int count=0,i=0;
newsize=(size/8) +1;
int z[newsize];
int w[newsize];
//Convert to decimal
while(i<size)
{
        while(count<4)
        {
            if(s[i]=='1')
            {
                new_data[count]=s[i];
                count=count+1;
                i++;
            }
            if(s[i]=='0')
            {

                new_data[count]=s[i];
                count=count+1;
                i++;
            }
             else
                i++;
        }

    new_s = convertToString(new_data,4);
    stringstream ssn(new_s);
    int x=0;
    ssn>>x;
    count=0;
    y=binaryToDecimal(x);
    z[j]=y;
    j++;

}

int arr_size=((sizeof z) / (sizeof *z));
int l=0,n=0,counter=0;
k=0;

while(n<arr_size)
{
    for (l=0;l<16;l++)
    {
        if(n<arr_size)
        {
            plaintext[l]=z[n];
            n++;
        }
        else
            plaintext[l]=0;

    }

    cout<<"round"<<dec<<counter<<":"<<"plaintext:";
    for(l=0;l<16;l++)
    {
        cout<<dec<<plaintext[l]<<"";
    }
    cout<<"<=>";
    for (int k=0;k<16;k++)
    {
        cout<<"0X"<<hex<<plaintext[k]<<"";
    }
    cout<<endl;

// encrypt
    unsigned char ciphertext[stt_lng];
    AES_Encrypt(plaintext, expandedKey, Nr, ciphertext);
    cout << "ciphertext = ";
    for (unsigned short i = 0; i <stt_lng; i++) { cout << dec << (unsigned short)ciphertext[i] << " "; }
    cout << "<=> ";
    for (unsigned short i = 0; i < stt_lng; i++) { cout << "0x" << hex << (unsigned short)ciphertext[i] << " "; }
    cout << endl << endl;
    counter++;
  }
/*
 *
 *     // decrypt
 *     unsigned char decrypted_plaintext[stt_lng];
 *         AES_Decrypt(ciphertext, expandedKey, Nr, decrypted_plaintext);
 *             cout << "decrypted_plaintext = ";
 *                 for (unsigned short i = 0; i < stt_lng; i++) { cout << decrypted_plaintext[i] << " "; }
 *                     cout << "<=> ";
 *                         for (unsigned short i = 0; i < stt_lng; i++) { cout << "0x" << hex << (unsigned short)decrypted_plaintext[i] << " "; }
 *                          cout << endl << endl;
 *                          */
    return 0;
}
