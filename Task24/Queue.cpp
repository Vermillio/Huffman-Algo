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




//Queue insertion between two elements
void insert(Queue *l, Queue *r, Queue *m)
{
	if (l)
	{
		l->next = m;
		m->prev = l;
		m->next = r;
		if (r)
			r->prev = m;
	}
	else if (r)
	{
		r->prev = m;
		m->next = r;
	}
	else return;
}



//Queue general insertion
Queue *push(TrNd *t, Queue *Head)
{
	if (t != nullptr)
	{
		if (Head->elem == nullptr)
		{
			Head = new Queue(t);
			return Head;
		}
		else
		{
			Queue *p = Head;
			while (p->next != nullptr && t->freq > p->elem->freq)
				p = p->next;
			Queue *piece = new Queue(t);
			if (t->freq > p->elem->freq)
				insert(p, p->next, piece);
			else insert(p->prev, p, piece);
			Queue *r = Head;
			while (r->prev)
				r = r->prev;
			return r;
		}
	}
	else return nullptr;
}


//constructors
Queue::Queue() : elem(nullptr), next(nullptr), prev(nullptr) {};

Queue::Queue(TrNd *Node) : elem(Node), next(nullptr), prev(nullptr) {};

//standard destructor
Queue::~Queue()
{
	if (this->next && this->prev)
	{
		this->next->prev = this->prev;
		this->prev->next = this->next;
	}
}
