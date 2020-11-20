#include "Trie.h"
#include "TrieIterator.h"
#include <iostream>
#include <sstream>

Trie::Trie(){
	this->root = new TrieNode();
	this->size = 0;
}

// TODO: Implement this function
Trie::Trie(const Trie & copy){

	cout << "Copy constructor called" << endl;
}

// TODO: Implement this function
Trie::Trie(Trie && move):root(nullptr), size(0){

	cout << "Move constructor called" << endl;
}

// TODO: Implement this function
Trie::~Trie(){

	cout << "Destructor called" << endl;
}


// TODO: Implement this function
TrieNode * Trie::createClone() const {

}

// TODO: Implement this function
void Trie::cloneHelper(TrieNode* source, TrieNode*& destination) const {

}


/*******************************************************************/
/*                      DEFINE YOUR OPERATORS HERE                 */


/*******************************************************************/



bool Trie::searchWord(string word) {
	lower(word);
	TrieNode * ptr = root;
	int len = word.length();
	for (int i = 0; i < len; i++) {
		int targetIndex = word.at(i) - 'a';
		if (!ptr->children[targetIndex]) {
			return false;
		}
		ptr = ptr->children[targetIndex];
	}
	if (ptr->isWord) {
		return true;
	}
	return false;
}

void Trie::lower(string & word) {
	string res;
	for (char c : word) {
		if (c >= 'A' && c < 'a') {
			res += (c - 'A' + 'a');
		}
		else {
			res += c;
		}
	}
	word = res;
}


void Trie::insertWord(string word) {
	lower(word);
	if (!searchWord(word)) {
		TrieNode * ptr = root;
		int len = word.length();
		for (int i = 0; i < len; i++) {
			int targetIndex = word.at(i) - 'a';
			if (!ptr->children[targetIndex]) {
				ptr->children[targetIndex] = new TrieNode();
			}
			//			ptr->isLeaf = false;
			ptr = ptr->children[targetIndex];
		}
		ptr->isWord = true;
		size += 1;
	}
}

bool Trie::hasChildren(TrieNode * node, int i = 0) const {
	if (node) {
		for (i; i < ALPHABET_SIZE; i++) {
			if (node->children[i]) {
				return true;
			}
		}
	}
	return false;
}

void Trie::deleteWord(string word) {
	lower(word);
	if (searchWord(word)) {
		if (deleteWordHelper(root, word)) {
			size--;
		}
	}
}

bool Trie::isEmpty() const {
	return !(this->hasChildren(root));
}

bool Trie::deleteWordHelper(TrieNode *& node, string word) {
	if (word.length()) {
		if (node != nullptr &&
			node->children[word[0] - 'a'] != nullptr &&
			deleteWordHelper(node->children[word[0] - 'a'], word.substr(1)) &&
			!node->isWord
			) {
			if (!hasChildren(node)) {
				delete node;
				node = nullptr;
				return true;
			}
			else {
				node->isWord = false;
				return true;
			}

		}
	}
	if (word.length() == 0 && node->isWord) {
		if (!hasChildren(node)) {
			delete node;
			node = nullptr;
			return true;
		}
		else {
			node->isWord = false;
			return false;
		}
	}
}

void Trie::printHelper(ostream & os, TrieNode * node, string & word) const {
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i]) {
			word += char(i + 'a');
			if (node->children[i]->isWord) {
				os << word << endl;
				printHelper(os, node->children[i], word);
			}
			else {
				printHelper(os, node->children[i], word);
			}
		}
		else if (i == 26) {
			word = word.substr(0, word.length() - 1);
		}
	}
	word = word.substr(0, word.length() - 1);
}

void Trie::deleteTrie() {
	if (root) {
		deleteTrieHelper(root);
		root = nullptr;
		size = 0;
	}
}

void Trie::deleteTrieHelper(TrieNode * node) {
	if (hasChildren(node)) {
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			if (node->children[i]) {
				deleteTrieHelper(node->children[i]);
			}
		}
	}
	delete node;
}

int Trie::length() {
	return size;
}
