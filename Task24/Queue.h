#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <utility>
#include <algorithm>
#include <vector>
#include <bitset>



typedef struct Queue
{
	TrNd * elem;
	Queue *next, *prev;

	
	Queue(), ~Queue();
	Queue(TrNd * Node);
	

};

Queue *push(TrNd *t, Queue *Head);
void insert(Queue *l, Queue *r, Queue *m);

#endif