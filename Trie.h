/* 
Karl Marrett
CSE 374 HW5
Feb 20th
Header for source file of Trie data structure and functions 
*/

#ifndef	TRIE_H 
#define TRIE_H
#include "T9.h"
#define KEYS 9 // 2-9 available keys + #

// basic node of Trie holds a potential word in val and
// pointers to child struct nodes for each possible key
// '2-9' and '#'
struct node {
	char* val;
	struct node* next[KEYS];
};

char* searchTrie(struct node* currentNode, char* input, int length, 
	int counter);
struct node* buildTrie(int* code, char* line, int length, 
	int counter, struct node* currentNode);
void deleteTrie(struct node* currentNode);
struct node* newNode(void);

#endif
