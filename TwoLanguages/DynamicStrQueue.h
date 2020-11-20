#ifndef DYNAMICSTRQUEUE_H
#define DYNAMICSTRQUEUE_H

#include <string>
using namespace std;

struct StrQueueNode
{
	string value;
	StrQueueNode *next;
	StrQueueNode(string str, StrQueueNode *ptr = NULL)
	{
		value = str;
		next = ptr;
	}
};

class DynamicStrQueue
{
	private:
		StrQueueNode *front;
		StrQueueNode *rear;

	public:
		DynamicStrQueue();
		~DynamicStrQueue();

		void enqueue(string);
		void dequeue(string &);
		bool isEmpty() const; 
		void clear(void);

		StrQueueNode* GetFront();
		StrQueueNode* GetRear();
};


#endif
