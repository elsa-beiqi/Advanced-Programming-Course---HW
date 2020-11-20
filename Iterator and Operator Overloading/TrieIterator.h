#ifndef _ITERATOR
#define _ITERATOR

#include "Stack.h"

class TrieIterator {
public:

	TrieIterator(const Trie & myTrie);

	void Init();
	bool HasMore() const;
	string Current() const;
	void Next();

private:
	const Trie & myTrie;
	TrieNode* currentNode;
	Stack stack;
	string currentWord;
};
#endif
