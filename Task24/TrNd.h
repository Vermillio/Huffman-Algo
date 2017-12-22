#ifndef TRND_H
#define TRND_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <utility>
#include <algorithm>
#include <vector>
#include <bitset>

using namespace std;

typedef struct values
{
	int freq;
	string key;
	values() : freq(0) {};
};


typedef struct TrNd
{
	TrNd(char x, int y);

	TrNd(char x, TrNd * left, TrNd * right);

	char symbol;
	int freq;
	TrNd *l, *r;

	TrNd(), ~TrNd();
};

TrNd *Clear(TrNd *t);

#endif
