#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

template <typename T>
class Encrypter {
private:
	const T key; // for XOR operation -- It  will  take the input and  operate  bitwise  xor  operation  on  it  with  the key.
	const int nShifts; // for number of shifts in circular shifting -- Then, the output of the xor operation will be given to Circular shifting operation as input.
	//At the ENCRYPTION stage, it will shift the bits nShifts many times to LEFT. 
	//At the DECRYPTION stage, it will shift the bits nShifts many times to RIGHT.

public:
	vector<int> listFlips; // for the list of bits to flip 

	Encrypter(T k, int s, vector<int> v):key(k),nShifts(s),listFlips(v){
		for(int i = listFlips.size()-1; i >= 0; i--){
			//If there is a bit greater than the bit count of the data type,
			//they will be neglected and won’t be in  the listFlips vector.
			if(listFlips[i] > sizeof(T)*8){
				listFlips.pop_back();
			}
		}
	}; // constructor
	T encrypt(const T decrypted); // this member function will do the operations in this order: key, nShifts, listFlips
	T decrypt(const T encrypted); // this member function will do the operations in the reverse order: listFlips, nShifts, key
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
T Encrypter<T>::encrypt(const T data){ 
	T xorResult = 0;
	xorResult = data ^ key;
	cout << "XOR result: " << xorResult << endl;

	//At the ENCRYPTION stage, it will shift the bits nShifts many times to LEFT. 
	//cycleLeftResult
	T cycleLeftResult = 0;
	//The output of the xor operation will be given to Circular shifting operation as input.
	T temp = xorResult;

	//If nShiftsis GREATER than or EQUAL to the bit size of data to be shifted, it will take the MOD(%) of nShifts and then shift that many times.*/ 
	if(nShifts >= sizeof(T)*8){
		cycleLeftResult = (temp << (nShifts%(sizeof(T)*8))) | (xorResult >> (sizeof(T)*8 - (nShifts%(sizeof(T)*8))));
	}
	else{
		cycleLeftResult = (temp << nShifts) | (xorResult >> (sizeof(T)*8 - nShifts));
	}
	cout << "Shift result: " << cycleLeftResult << endl;


		//flipBits
	//flip the bits of the resulting circular shift operation (cycleLeftResult)
	T flipBitsResult=cycleLeftResult;
	T temp1=flipBitsResult;

	for(int i = 0; i < listFlips.size(); i++){
		//shift to the right
		T temp = (temp1 >> listFlips[i] | (flipBitsResult << ((sizeof(T)*8) - listFlips[i])));

		T val = 1;

		//get new value
		T newVal = val^temp;
		T tempNewVal=newVal;

		//shift to the left to go back to the position
		flipBitsResult = (tempNewVal << listFlips[i] | (newVal >> ((sizeof(T)*8) - (listFlips[i]))));
	}
	return flipBitsResult;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
T Encrypter<T>::decrypt(const T encrypted){

		//flipBits
	//flip the bits of the resulting circular shift operation ('encrypted' variable)
	T flipBitsResult=encrypted;
	T temp1=flipBitsResult;

	for(int i = 0; i < listFlips.size(); i++){
		//shift to the right
		T temp = (temp1 >> listFlips[i] | (flipBitsResult << ((sizeof(T)*8) - listFlips[i])));

		T val = 1;
		
		//get new value
		T newVal = val^temp;
		T tempNewVal=newVal;

		//shift to the left to go back to the position
		flipBitsResult = (tempNewVal << listFlips[i] | (newVal >> ((sizeof(T)*8) - (listFlips[i]))));
	}

	//At the DECRYPTION stage, it will shift the bits nShifts many times to RIGHT.
	//cycleRightResult
	T cycleRightResult = 0;
	T temp = flipBitsResult;

	//If nShiftsis GREATER than or EQUAL to the bit size of data to be shifted, it will take the MOD(%) of nShifts and then shift that many times.*/ 
	if(nShifts >= sizeof(T)*8){
		cycleRightResult = (temp >> (nShifts%(sizeof(T)*8))) | (flipBitsResult << (sizeof(T)*8 - (nShifts%(sizeof(T)*8))));
	}
	else{
		cycleRightResult = (temp >> nShifts) | (flipBitsResult << (sizeof(T)*8 - nShifts));
	}

	//XOR
	T xorResult=0;
	xorResult = cycleRightResult ^ key;
	return xorResult;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

	int nShifts = 12;
	//vector<int> listFlips = { 0,1,2,3,4,9,15,30,35 };
	vector<int> listFlips;
	listFlips.push_back(0);
	listFlips.push_back(1);
	listFlips.push_back(2);
	listFlips.push_back(3);
	listFlips.push_back(4);
	listFlips.push_back(9);
	listFlips.push_back(15);
	listFlips.push_back(30);
	listFlips.push_back(35);

	unsigned char key1 = 101;
	unsigned char chClear = 'b';
	Encrypter<unsigned char> encrypterChar(key1, nShifts, listFlips); 

	cout << "Char to encrypt: " << chClear << endl;
	unsigned char encryptedCh = encrypterChar.encrypt(chClear);
	unsigned char decryptedCh = encrypterChar.decrypt(encryptedCh);

	cout << "Char encrypted: " << encryptedCh << endl;
	cout << "Char decrypted: " << decryptedCh << endl;
	cout << "List of flipped bits: ";
	for (int i : encrypterChar.listFlips) {
		cout << i << " ";
	}
	cout << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	unsigned short int key2 = 101;
	unsigned short int intShortClear = 500;
	Encrypter<unsigned short int> encrypterShortInt(key2, nShifts, listFlips);

	cout << "Short Int to encrypt: " << intShortClear << endl;
	unsigned short int encryptedShortInt = encrypterShortInt.encrypt(intShortClear);
	unsigned short int decryptedShortInt = encrypterShortInt.decrypt(encryptedShortInt);

	cout << "Short Int encrypted: " << encryptedShortInt << endl;
	cout << "Short Int decrypted: " << decryptedShortInt << endl;
	cout << "List of flipped bits: ";
	for (int i : encrypterShortInt.listFlips) {
		cout << i << " ";
	}
	cout << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	unsigned int key3 = 101;
	unsigned int intClear = 32800;
	Encrypter<unsigned int> encrypterInt(key3, nShifts, listFlips);

	cout << "Int to encrypt: " << intClear << endl;
	unsigned int encryptedInt = encrypterInt.encrypt(intClear);
	unsigned int decryptedInt = encrypterInt.decrypt(encryptedInt);

	cout << "Int encrypted: " << encryptedInt << endl;
	cout << "Int decrypted: " << decryptedInt << endl;
	cout << "List of flipped bits: ";
	for (int i : encrypterInt.listFlips) {
		cout << i << " ";
	}
	cout << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	unsigned long long int key4 = 101;
	unsigned long long int intLLClear = 34359738368;
	Encrypter<unsigned long long int> encrypterLLInt(key4, nShifts, listFlips);

	cout << "Long Long Int to encrypt: " << intLLClear << endl;
	unsigned long long int encryptedLLInt = encrypterLLInt.encrypt(intLLClear);
	unsigned long long int decryptedLLInt = encrypterLLInt.decrypt(encryptedLLInt);

	cout << "Long Long Int encrypted: " << encryptedLLInt << endl;
	cout << "Long Long Int decrypted: " << decryptedLLInt << endl;
	cout << "List of flipped bits: ";
	for (int i : encrypterLLInt.listFlips) {
		cout << i << " ";
	}
	cout << endl << endl;


	system("pause");
	return 0;
  }
