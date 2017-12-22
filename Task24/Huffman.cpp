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
#include <stack>

using namespace std;

//Comparator for map sorting
auto cmp = [](pair<char, values> const &x, pair<char, values> const &y)
{
	return x.second.freq < y.second.freq;
};



//Read file into string, count ocurrencies for every symbol. 
void Huffman::ReadDecodedFile()
{
	ifstream fin(decodedFile.path, ios::in | ios::binary);
	if (!fin.is_open())
		ErrorMessage(0);
	
	fin.seekg(0, ios::beg);
	int start = fin.tellg();
	fin.seekg(0, ios::end);
	int stop = fin.tellg();
	int size = stop - start;
	fin.seekg(0, std::ios::beg);
	fin >> decodedFile.data;
	char token;
	fin.seekg(0);
	map<char, values>::iterator it;
	while (!fin.eof())
	{
		fin >> token;
		it = decodedFile.Table.find(token);
		++it->second.freq;
	}
	fin.close();
}

void Huffman::WriteDecodedFile()
{
	ofstream fout(decodedFile.path, ios::out | ios::binary);

	if (!fout.is_open())
		ErrorMessage(0);

	for (int i = 0; i < decodedFile.data.size(); i++)
	fout << decodedFile.data[i];
	fout.close();
}





void Huffman::ReadEncodedFile()
{
	ifstream fin(encodedFile.path, ios::in | ios::binary);
	
	if (!fin.is_open())
		ErrorMessage(0);

	int size;
	fin >> encodedFile.offset;
	char it;
	while (!fin.eof())
	{
		fin >> it;
		if (!fin.eof())
		encodedFile.data.push_back((bitset<BYTE_SIZE>)it);
	}
	for (int i = 0; i < encodedFile.data.size(); i++)
		encodedFile.data_s += encodedFile.data[i].to_string();
	fin.close();
}


//+++++++++++++++
void Huffman::BuildEncodedData()
{
	string str;
	for (int i = 0; i < decodedFile.data.length(); i++)
	{
		str = decodedFile.Table.find(decodedFile.data[i])->second.key;
		encodedFile.data_s += str;
	}

	encodedFile.offset = (8 - encodedFile.data_s.size() % 8) % 8;
	int filesize = (encodedFile.offset + encodedFile.data_s.size()) / 8;


	for (int i = 0; i < encodedFile.offset; i++)
	{
		encodedFile.data_s.insert(encodedFile.data_s.begin(), '0');
	}
	encodedFile.data.resize(filesize);
	for (int i = 0; i < filesize; i++)
		for (int j = 0; j < BYTE_SIZE; j++)
			encodedFile.data[i][BYTE_SIZE - 1 - j] = (encodedFile.data_s[8 * i + j] == '1');
}



void Huffman::WriteEncodedFile()
{
	ofstream fout(encodedFile.path, ios::out | ios::binary);
	if (!fout.is_open())
		ErrorMessage(0);
	else
	{      
		fout << encodedFile.offset;//!
		for (size_t i = 0; i < encodedFile.data.size(); i++)
		{
			unsigned char c = static_cast<unsigned char>(encodedFile.data[i].to_ulong());
			fout.write((char*)&c, sizeof(c));
		}
	}
	fout.close();
}




void Huffman::GetQueue()
{
	for (map<char, values>::iterator i = decodedFile.Table.begin(); i != decodedFile.Table.end(); i++)
	{
		if (i->second.freq)
		{
			TrNd *t = new TrNd(i->first, i->second.freq);
			Head = push(t, Head);
		}
			
	}
}


//Transform queue into tree
void Huffman::GetTree()
{
	while (Head->next)
	{
		Queue *p = Head;
		TrNd *Node = new TrNd;
		Node->l = p->elem;
		Node->r = p->next->elem;
		Node->freq = Node->l->freq + Node->r->freq;

		Queue *q = p;
		while (q->next && q->elem->freq < Node->freq)
			q = q->next;

		Queue *piece = new Queue(Node);
		if (Node->freq < q->elem->freq)
			insert(q->prev, q, piece);
		else insert(q, q->next, piece);

		Head = Head->next;
		delete p;
		p = Head;
		Head = Head->next;
		delete p;
		Head->prev = nullptr;
	}
	encodedFile.tree = Head->elem;
}

//+
void Huffman::GetKeys(TrNd *t, string curr)
{
	if (t)
	{
		GetKeys(t->l, curr + "0");
		GetKeys(t->r, curr + "1");
		if (t->freq && !t->l && !t->r)
		{
			decodedFile.Table.find(t->symbol)->second.key = curr;
		}
	} 
}







// ?
void Huffman::LoadTreeFile()
{
	encodedFile.path_tf = TREE_DIR + "\\1.bin"; // maybe + "/" or + "\"
	ifstream fin(encodedFile.path_tf, ios::in | ios::binary);
	
	if (!fin.is_open())
		ErrorMessage(0);

	fin.seekg(0);


	stack<TrNd*> stck;
	TrNd *node1 = nullptr, *node2 = nullptr;

	char token;
	while (!fin.eof())
	{
		fin >> token;
		if (token == '1')
		{
			fin >> token;
			node1 = new TrNd(token, 0);
			stck.push(node1);
		}
		else if (token == '0' && !fin.eof()) {
			node2 = stck.top();
			stck.pop();
			node1 = stck.top();
			stck.pop();
			node1 = new TrNd(-1, node1, node2);
			stck.push(node1);
		}
		//else incorrect tree file
	}
	encodedFile.tree = stck.top();
	stck.pop();

	fin.close();
}


void Huffman::Write(TrNd *t)
{
	if (!t)
	{
		return;
	}
	if (!t->l && !t->r)
	{
		encodedFile.fout << '1';
		encodedFile.fout << t->symbol;
	}
	else {

		Write(t->l);
		Write(t->r);
		encodedFile.fout << '0';
	}
}

//+
void Huffman::WriteTreeToFile()
{
	encodedFile.path_tf = TREE_DIR + "\\1.bin";
	encodedFile.fout.open(encodedFile.path_tf, ios::out | ios::binary);
	if (!encodedFile.fout.is_open())
		ErrorMessage(0);
	Write(encodedFile.tree);
}



void Huffman::BuildDecodedData()
{
	TrNd *t = encodedFile.tree;
	for (int i = encodedFile.offset; i <= encodedFile.data_s.size(); i++)
	{
		if (!t->l && !t->r)
		{
			decodedFile.data += t->symbol + '\0';
			t = encodedFile.tree;
		}
		if (encodedFile.data_s[i] == '0')
			t = t->l;
		else t = t->r;
	}
}

//Main encoding method
void Huffman::Encode()
{
	ReadDecodedFile();
	//sort(tableFile.Table.begin(), tableFile.Table.end(), cmp); // ?

	GetQueue();
	GetTree();
	GetKeys(encodedFile.tree, "");


	BuildEncodedData();

	WriteEncodedFile();
	WriteTreeToFile();
	cout << encodedFile.path_tf << endl;
}

void Huffman::Decode()
{
	ReadEncodedFile();
	LoadTreeFile();

	BuildDecodedData();
	WriteDecodedFile();
}







void Huffman::set_dFpath(string filename)
{
	decodedFile.path = filename;
	encodedFile.path = filename;
	encodedFile.path.erase(encodedFile.path.end() - 4, encodedFile.path.end());
	encodedFile.path += "_encoded.bin";
}

void Huffman::set_eFpath(string filename)
{
	encodedFile.path = filename;
	decodedFile.path = filename;
	decodedFile.path.erase(decodedFile.path.end() - 12, decodedFile.path.end() - 4);
}





//fill map with ASCII symbols as values
void Huffman::loadASCII()
{
	for (int i = 0; i < 256; i++)
	{
		decodedFile.Table.insert(pair<char, values>((char)i, values()));
	}
}


Huffman::Huffman()
{
	Head = new Queue;
	encodedFile.tree = new TrNd;
	decodedFile.data = "";
	loadASCII();

	wchar_t* w = nullptr;
	wstring ws(_wgetcwd(w, _MAX_PATH));
	TREE_DIR = string(ws.begin(), ws.end());
}


Huffman::~Huffman()
{
	encodedFile.fout.close();
	encodedFile.tree = Clear(encodedFile.tree);
}


void Huffman::ErrorMessage(int n)
{
	switch (n)
	{
	case 0:
	{
		cout << "Couldn't open file." << endl;
	}
	case 1:
	{

	}
	case 2:
	{

	}
	}
}