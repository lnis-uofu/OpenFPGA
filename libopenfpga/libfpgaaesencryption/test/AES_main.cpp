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
	/*
	cout << "AES with Nb = " << Nb << " columns, Nk = " << Nk << " (32-bit) words i.e. CipherKeyLenghth = "
        << CipherKeyLenghth << " bytes (or " << CipherKeyLenghth * 8 << " bits), Nr = " << Nr << " rounds" << endl << endl;
	*/

    // create a dummy test cipher key
    unsigned char key[CipherKeyLenghth_max];
	cout << "key = ";
    for (unsigned short i = 0; i < CipherKeyLenghth; i++) {
        key[i] = (unsigned char)i;
    //    cout << "0x" << hex << (unsigned short)key[i] << " ";
    } //  cout << endl << endl;


    // extend key
    unsigned char expandedKey[ExtdCipherKeyLenghth_max];
    KeyExpansion(key, Nk, expandedKey);
	/*  cout << "expandedKey = ";
    for (unsigned short i = 0; i < ExtdCipherKeyLenghth; i++) { cout << dec << (unsigned short)expandedKey[i] << " "; }
    cout << "<=> ";
    for (unsigned short i = 0; i < ExtdCipherKeyLenghth; i++) { cout << "0x" << hex << (unsigned short)expandedKey[i] << " "; }
    cout << endl << endl;
	*/


	//Read Input Bitstream
	streampos input_file_size;
	char *data_in_file;
	unsigned int plaintext[16];
	int dec_array_size=0;
	int dec_file_size;
	int dec_data;
	ifstream file ("/research/ece/lnis/USERS/sampathkumar/OpenFPGA/openfpga_flow/tasks/fpga_bitstream
					/generate_bitstream/device_auto/run001/k6_frac_N10_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/fabric_bitstream.txt",ios::in|ios::ate);
	if (file.is_open())
	{
		input_file_size=file.tellg();
		data_in_file=new char [input_file_size];
		file.seekg(0,ios::beg);
		file.read(data_in_file,input_file_size);
		file.close();
	}
	else cout<<"Unable to open file";

	std::string data_in_string,new_s;
	std::stringstream ss;
	ss << data_in_file;
	data_in_string = ss.str();
	char Byte_data[4];
	int bit_count=0;
	dec_file_size=(input_file_size/8) +1;
	int dec_array[dec_file_size];
	int w[dec_file_size];
	while(int rd_data_cnt<input_file_size)
	{
        while(bit_count<4)
        {
			if(data_in_string[rd_data_cnt]=='1')
			{
				Byte_data[bit_count]=data_in_string[rd_data_cnt];
				bit_count=bit_count+1;
				rd_data_cnt++;
			}
			if(data_in_string[rd_data_cnt]=='0')
			{

				Byte_data[bit_count]=data_in_string[rd_data_cnt];
				bit_count=bit_count+1;
				rd_data_cnt++;
			}
    			else
			rd_data_cnt++;
		}
		new_s = convertToString(Byte_data,4);
		stringstream ssn(new_s);
		int binary_data=0;
		ssn>>binary_data;
		bit_count=0;
		dec_data=binaryToDecimal(binary_data);
		dec_array[dec_array_size]=dec_data;
		dec_array_size++;

	}

	int arr_size=((sizeof dec_array) / (sizeof *dec_array));	//size of the array
	int input_data_cnt=0,cipher_array_size=0;;
	while(input_data_cnt<arr_size)
	{
		for (int i=0;i<16;i++)
		{
			if(input_data_cnt<arr_size)
			{
				plaintext[i]=dec_array[input_data_cnt];
				input_data_cnt++;
			}
			else
				plaintext[i]=0;

		}
		
		// encrypt
		unsigned char ciphertext[stt_lng];
		AES_Encrypt(plaintext, expandedKey, Nr, ciphertext);
		for (unsigned short i=0;i<stt_lng;i++)
		{
			w[cipher_array_size]= (unsigned short)ciphertext[i];
			cipher_array_size++;
		}

		/*  
		cout << "ciphertext = ";
		for (unsigned short i = 0; i <stt_lng; i++) { cout << dec << (unsigned short)ciphertext[i] << " "; }
		cout << "<=> ";
		for (unsigned short i = 0; i < stt_lng; i++) { cout << "0x" << hex << (unsigned short)ciphertext[i] << " "; }
		cout << endl << endl;
		*/
		
	}
	
	//Output the Encrypted data in a File
	ofstream myfile ("Encrypted_bitstream.txt");
	if (myfile.is_open())
	{
		for(dec_array_size =0;dec_array_size<arr_size;dec_array_size++)
		{
			myfile << w[dec_array_size] <<endl ;
		}
		myfile.close();
	}
	else cout << "Unable to open file";

	ofstream myfile1 ("Fabricdec_bitstream.txt");
	if (myfile1.is_open())
	{
		for(dec_array_size =0;dec_array_size<arr_size;dec_array_size++)
		{
			myfile1 << dec_array[dec_array_size] <<endl ;
		}
		myfile1.close();
	}
	else cout << "Unable to open file";

	/*
 
    // decrypt
    unsigned char decrypted_plaintext[stt_lng];
    AES_Decrypt(ciphertext, expandedKey, Nr, decrypted_plaintext);
    cout << "decrypted_plaintext = ";
    for (unsigned short i = 0; i < stt_lng; i++) 
	{ 
		cout << decrypted_plaintext[i] << " "; 
	}
    cout << "<=> ";
    for (unsigned short i = 0; i < stt_lng; i++) 
	{	
		cout << "0x" << hex << (unsigned short)decrypted_plaintext[i] << " "; 
	}
    cout << endl << endl;
    */
    return 0;
}
