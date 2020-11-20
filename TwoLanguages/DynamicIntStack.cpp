#include <iostream>
#include "DynamicIntStack.h"
using namespace std;

//Constructor
DynamicIntStack::DynamicIntStack()
{
	top=NULL;
}

IntStackNode* DynamicIntStack::GetTopPointer()
{
	return top;
}

//Push back elements to the stack
void DynamicIntStack::push(int elmt)
{
	IntStackNode *newNode;

	newNode = new IntStackNode;
	newNode->value = elmt;

	if(isEmpty())
	{
		top = newNode;
		newNode->next = NULL;
	}
	else
	{
		newNode->next = top;
		top = newNode;
	}
}
//Pop up elements from the stack
void DynamicIntStack::pop(int &elmt)
{
	IntStackNode *temp;

	if(isEmpty())
	{
		cout<<"Stack is empty!\n";
	}

	else 
	{
		elmt = top->value;
		temp = top->next;
		delete top;
		top = temp;
	}
}

//If the stack is empty check function
bool DynamicIntStack::isEmpty()
{
	bool status;

	if(top==NULL)
		status=true;

	else 
		status=false;

	return status;
}

void DynamicIntStack::clear() {
	int tmp;
	while (!isEmpty()) {
		pop(tmp);
	}
}
