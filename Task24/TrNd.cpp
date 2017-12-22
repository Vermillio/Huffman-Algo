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




//Standard constructor
TrNd::TrNd() : symbol(-1), freq(0), l(nullptr), r(nullptr) {};

//Standard destructor
TrNd::~TrNd()
{

}


//Make tree node from map element
//TrNd::TrNd(pair<char, values> x) : symbol(x.first), freq(x.second.freq) {};
TrNd::TrNd(char x, int y) : symbol(x), freq(y), l(nullptr), r(nullptr) {};

TrNd::TrNd(char x, TrNd *left, TrNd *right) : symbol(x), l(left), r(right) {};


TrNd *Clear(TrNd *t)
{
	if (t)
	{
		t->l = Clear(t->l);
		t->r = Clear(t->r);
		delete t;
		t = nullptr;
	}
	return t;
}