#ifndef DYNAMICINTSTACK_H
#define DYNAMICINTSTACK_H

using namespace std;

struct IntStackNode
{
	int value;
	IntStackNode *next;
};


class DynamicIntStack
{
	private:
		IntStackNode *top;

	public:
		DynamicIntStack(void);
		void push(int);
		void pop(int &);
		bool isEmpty(void);
		IntStackNode* GetTopPointer();
		void clear(void);

};


#endif
