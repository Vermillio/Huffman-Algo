#ifndef HUFFMAN_H
#define HUFFMAN_H
#define BYTE_SIZE 8

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



struct ef
{
	char TNum;
	vector<bitset<BYTE_SIZE>> data;
	string data_s;
	string path;
	char offset; //offset to be divisible by 8 (byte size)

	string path_tf;
	ofstream fout;
	TrNd *tree;
};

struct df
{
	string path;
	string data;
	map<char, values> Table;
};




class Huffman
{
	private:
		string TREE_DIR;
		df decodedFile;
		ef encodedFile;
		Queue *Head;



		void loadASCII();

		void ReadDecodedFile();
		void ReadEncodedFile();
		void WriteDecodedFile();
		void WriteEncodedFile();

		void LoadTreeFile();
		void WriteTreeToFile();
		void Write(TrNd* t);

		void GetQueue();
		void GetTree();
		void GetKeys(TrNd *t, string curr);
		void BuildEncodedData();
		void BuildDecodedData();

		void ErrorMessage(int n);

	public:
		Huffman();
		~Huffman();

		void Encode();
		void Decode();

		void set_dFpath(string filename);
		void set_eFpath(string filename);

};




#endif