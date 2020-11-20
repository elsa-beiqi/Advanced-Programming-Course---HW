#ifndef DYNAMICSTRINGSTACK_H
#define DYNAMICSTRINGSTACK_H

#include <string>
using namespace std;

struct StackNode
{
	string value;
	StackNode *next;
};


class DynamicStringStack
{
	private:
		StackNode *top;
		StackNode* GetTopPointer(DynamicStringStack myStack);

	public:
		DynamicStringStack(void);
		void push(string);
		void pop(string &);
		bool isEmpty(void);
};


#endif
