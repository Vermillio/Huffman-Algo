#include "Huffman.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <utility>
#include <algorithm>
#include <vector>
#include "TrNd.h"
#include "Queue.h"
#include <bitset>

using namespace std;

int main()
{
	Huffman f;
	string filename;
	
	bool choise;

	cout << "ENCODE - 0, DECODE - 1: ";
	cin >> choise;
	if (choise == 0)
	{
		cout << "Enter filename (file will be encoded with Huffman Algo): ";
		cin >> filename;
		f.set_dFpath(filename);
		f.Encode();
		cout << "Encoding completed. File saved in same file directory." << endl;
	}
	else
	{
		cout << "Enter filename to get the original, non-compressed file: ";
		cin >> filename;
		f.set_eFpath(filename);
		f.Decode();
		cout << "Decoding completed. File saved in same file directory." << endl;
	}
	
	system("pause");
	return 0;
}