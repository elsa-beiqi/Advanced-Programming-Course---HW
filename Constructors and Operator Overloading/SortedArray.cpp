#include "SortedArray.h"

SortedArray::SortedArray() {
	front = nullptr;
}

// Implementation of the copy constructor
SortedArray::SortedArray(const SortedArray & copy) {
	front = copy.createClone();
}

// Implementation of the destructor
SortedArray::~SortedArray() {
	ArrayNode * current = front, *next = front;
	while (current) {
		next = current->right;
		delete current;
		current = next;
	}
	front = nullptr;
}

ArrayNode * SortedArray::getFront() const{
	return front;
}

void SortedArray::SetFront(ArrayNode * newFront){
	front = newFront;
}

// Implementation of the createClone function
ArrayNode * SortedArray::createClone() const {

	// check if the front is NULL. if it is, then just return null pointer
	if(front == NULL){
		return nullptr;
	}

	// create newList with the value of front
	ArrayNode* newList = new ArrayNode(front->val);
	newList->right = nullptr;

	ArrayNode* currentNode = front->right; // since I already inserted front's value before, I start with the next element
	ArrayNode* previousNode = newList; // assigning it to newList to iterate over it insted of making it NULL

	while (currentNode)
	{
		previousNode->right = new ArrayNode(currentNode->val); 
		previousNode = previousNode->right;  
		currentNode = currentNode->right;
	}
	return newList;
}

bool SortedArray::search(int Value)
{
	ArrayNode * tmp = front;
	while (tmp) {
		if (tmp->val == Value) {
			return true;
		}
		tmp = tmp->right;
	}
	return false;
}

void SortedArray::insert(int Value) {
	if (!search(Value)) {
		ArrayNode * tmp = front;

		ArrayNode * newNode = new ArrayNode(Value);
		if (!front) {
			front = newNode;
		}
		else if (front && Value < front->val) {
			newNode->right = front;
			front = newNode;
		}
		else {
			while (tmp->right && tmp->right->val < Value) {
				tmp = tmp->right;
			}
			newNode->right = tmp->right;
			tmp->right = newNode;
		}
	}
}

void SortedArray::deleteSortedArray() {
	ArrayNode * current = front, *next = front;
	while (current) {
		next = current->right;
		delete current;
		current = next;
	}
	front = nullptr;
}

void SortedArray::print() {
	if (!front) {
		cout << "Array is empty" << endl;
	}
	ArrayNode * tmp = front;
	while (tmp) {
		cout << tmp->val << " ";
		tmp = tmp->right;
	}
	cout << endl;
}

// Implementation of the assignment (=) and plus (+) operators

SortedArray SortedArray::operator= (const SortedArray &sortedArray){
	front = sortedArray.createClone(); 
	return *this;						
}

// for the case where we add a number to an array (the parameter is the lhs)
SortedArray SortedArray::operator+ (int new_val){
	// new sorted array for array2 = array1 + 20 (or array1 = array1 + 20) - just examples
	SortedArray result(*this);
	result.insert(new_val);
	return result;	
}

// when we add two arrays
SortedArray SortedArray::operator+ (const SortedArray &sortedArray){
	SortedArray result(*this);
	ArrayNode *curr = sortedArray.getFront();
	// looping through the rhs. if curr is not null, then insert that node to the lhs.
	while(curr != NULL){
		result.insert(curr->val);
		curr = curr->right; 
	}
	return result;
}

// when we add a number to an array
SortedArray operator+ (int new_val, const SortedArray &sortedArray){
	// create temp object and insert new_val inside it
	SortedArray temp(sortedArray);
	temp.insert(new_val);
	return temp;
}
