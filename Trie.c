/* 
Karl Marrett
CSE 374 HW5
Feb 20th
Source file for Trie data structure and functions 
*/

#include "T9.h"
#include "Trie.h"

void printTrie(struct node* currentNode);
char* handleNoMatch(char lastChar);  
char* appendQuotes(char* str, int length);  

// allocate memory and initialize fields to NULL
struct node* newNode(void)
{ 
	struct node* n = (struct node*) malloc(sizeof(struct node)); 
	if (n == NULL)  {
		fprintf(stderr, MLCFAIL);
	}
	int i;
	for (i = 0; i < KEYS; i++)  
		n->next[i] = NULL;
	n->val = NULL;
	return n;
}

// Print contents of val in preorder to stdout
void printTrie(struct node* currentNode)  
{
	if (currentNode->val != NULL)  {
		//puts(currentNode->val);	
		printf("%s\n", currentNode->val);	
	}
	int i;
	for (i = 0; i < KEYS; i++) {
		if (currentNode->next[i] != NULL)  {
			printTrie(currentNode->next[i]);	
		}
	}
}

// inserts new word into Trie, traversing trie according to code
// array and setting val field to line
// assumes passed initialized root
struct node* buildTrie(int* code, char* line, int length, 
	int counter, struct node* currentNode)
{  
	// if (currentNode == NULL)
	// 	currentNode = newNode();
	// if at the final code character
	if (counter >= length)  {
		// if no other t9onym exists
		if (currentNode->val == NULL)  {
			currentNode->val = 
				(char*)malloc(sizeof(char) * length);
			strncpy(currentNode->val, line, length);
		}  else  {
			// move along t9onym linked list until new 
			//word inserted
			if (!currentNode->next[KEYS - 1])  
				currentNode->next[KEYS - 1] = newNode();
			currentNode->next[KEYS - 1] =   
			buildTrie(code, line, length, 
			counter, currentNode->next[KEYS - 1]);
			// next[KEYS - 1] means next['#']
		}
	}  else {  // traverse tree further
		// convert code to index of 'next' array
		int nextIndex;
		nextIndex = code[counter] - 2;
		if (!currentNode->next[nextIndex])  {
			currentNode->next[nextIndex] = newNode();
		}
		currentNode->next[nextIndex] = buildTrie(code, 
		line, length, ++counter, currentNode->next[nextIndex]);
	}
	return currentNode;
}

// Recursively free all child branches, value stored
// and pointer to self
// Deletes all (sub)trie data passed to it
void deleteTrie(struct node* currentNode)  
{
	int i;
	for (i = 0; i < KEYS; i++)  {
		if (currentNode->next[i] != NULL)  {
			// recursively free children
			deleteTrie(currentNode->next[i]);
		}
	}
	free(currentNode->val);
	free(currentNode);
}

// check whether word does not exist on account of 
// no other t9onyms returns appropriate char* message
char* handleNoMatch(char lastChar)  
{
	if (lastChar == '#')  {
		return "There are no more T9onyms";
	}  else  { return "Not found in current dictionary.";	}
}

// manually add quotes around string
// return new char* created via malloc
char* appendQuotes(char* str, int length)  
{
	char* temp = (char*) malloc(sizeof(char) * (length + 3));
	temp[0] = '\'';
	strcpy(temp + 1, str);
	temp[length + 1] = '\'';
	temp[length + 2] = '\0';
	return temp;
}

// Passed node pointer and T9input code of user to search child branches for 
// match takes the input of user the length of the input and an integer counter.
char* searchTrie(struct node* currentNode, char* input, int length, 
	int counter)
{
	// stop if arrived at null branch or beyond input
	if ((currentNode == NULL) || (counter > length))  {
		if (counter == 0) {
			//case of a null tree
			return handleNoMatch(input[counter]);
		}  else {
			return handleNoMatch(input[counter - 1]);
		}
	}
	// arrived at final node encoded by input
	if (counter == length )  {
		if (currentNode->val != NULL)  {
			int length = strlen(currentNode->val);
			return appendQuotes(currentNode->val, length);
		}  else  {
			return handleNoMatch(input[counter - 1]);
		}
	}  else {
		if (input[counter] == '#')  {
			// search # child
			return searchTrie(
			currentNode->next[KEYS - 1], input, 
				length, ++counter);
		}  else  { // search corresponding child
			// T9 code related to ind by subtr. 2
			int index = (input[counter] - '0') - 2;
			return searchTrie(currentNode->next[index], 
				input, length, ++counter);
		}
	}
}

