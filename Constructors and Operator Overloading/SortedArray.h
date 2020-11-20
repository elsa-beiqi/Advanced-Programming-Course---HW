#ifndef _SORTEDARRAY
#define _SORTEDARRAY

#include <iostream>

using namespace std;

class ArrayNode {
public:
	int val;
	ArrayNode *right;
	ArrayNode() {};
	ArrayNode(int val) :val(val), right(nullptr) {};
};


class SortedArray {
public:
	SortedArray();

	SortedArray(const SortedArray &);

	~SortedArray();

	bool search(int Value);
	void insert(int Value);
	void deleteSortedArray();
	void print();
	ArrayNode * getFront()const;
	void SetFront(ArrayNode * newFront);

	ArrayNode* createClone() const;

	//Define and implement = operator for assignments and cascading assignment
	SortedArray SortedArray::operator= (const SortedArray &sortedArray);

	// for the case where we add a number to an array (the parameter is the lhs)
	SortedArray SortedArray::operator+ (int new_val);
	//for the case where we add two arrays
	SortedArray SortedArray::operator+ (const SortedArray &SortedArray);
	

private:

	void cloneHelper(ArrayNode* source, ArrayNode*& destination) const;

	ArrayNode *front;

};

// when we add a number to an array
SortedArray operator+ (int new_val, const SortedArray &SortedArray);

#endif
