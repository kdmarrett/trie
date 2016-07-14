// to do gdb valgrind
// read through build trie sections carefully
// check that alloc is being done efficiently
// make file split into multiple files

/* 
Karl Marrett
CSE 374 HW5
Feb 20th
Main source file which handles client based
interaction and interacts with Trie data structure and functions 
*/

#include "Trie.h"
#include "T9.h"
// symbolic constants
#define MAXLINE 100

int getCode(char currentChar);
struct node* getDict(FILE* fp, char* fileName, struct node* root);

// Convert lowercased letters other than '#' into
// their corresponding T9 integer
// subtracting 2 from T9 int yields the index 
// of the next field in the Trie struct
int getCode(char currentChar)
{
	int t9Index = (int)currentChar - 97;
	int t9Code[26] = {2, 2, 2, 3, 3, 3, 4, 
	4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 
	8, 8, 8, 9, 9, 9, 9};
	return t9Code[t9Index];
}
		
// Takes FILE pointer of dictionary, filename for error messages and 
// pointer to root of Trie.
// processes each line in the file, sending the code to buildTrie
// returns the root after all word insertions
struct node* getDict(FILE* fp, char* fileName, struct node* root)
{
	char* line = (char*)malloc(sizeof(char) * MAXLINE);
	int* code = (int*)malloc(sizeof(int) * MAXLINE);
	char* word = (char*) malloc(sizeof(char) * MAXLINE);
	// check malloc
	if ((line == NULL) || (code == NULL) || (word == NULL))
		fprintf(stderr, MLCFAIL);

	while ((fgets(line, MAXLINE, fp)) != NULL)  {
		int i = 0;
		while ((line[i] != '\0') && 
			(line[i] != '\n') && (i < MAXLINE))  {
			// convert to T9code
			code[i] = getCode(tolower(line[i]));
			i++;
		}
		// trim to word 
		strncpy(word, line, i); 
		root = buildTrie(code, word, i, 0, root);
	}

	free(word);
	free(line);
	free(code);
	return root;
}

int main(int argc, char** argv)
{
	FILE* fp; // declare dict file
	char* programName = argv[0];
	char* dictName = argv[1];
	char* input = (char*)malloc(sizeof(char) * MAXLINE);
	char* oldInput = (char*)malloc(sizeof(char) * MAXLINE);

	if ((input == NULL) || (oldInput == NULL))
		fprintf(stderr, MLCFAIL);
	if ((fp = fopen(dictName, "r")) == NULL)  {
		fprintf(stderr, "%s: %s: No such file or directory\n",
			programName, dictName);
		// exit if dict is not loaded properly
		return 1;
	}

	//build tree from dict
	struct node* root = newNode();
	root = getDict(fp, dictName, root);
	fclose(fp);

	// T9 interaction:
	printf("Enter \"exit\" to quit.\n");
	while (1)  {
		printf("Enter Key Sequence (or \"#\" for next word) :\n");
		scanf("%s", input);
		int len = strlen(input);
		if (!strncmp(input, "exit", len))  
			break;
		if (!strncmp(input, "#", 1))  
			strcat(oldInput, input);
		else
			strcpy(oldInput, input);
		char* word = searchTrie(root, oldInput, 
			strlen(oldInput), 0);
		printf("\t%s\n", word);
		free(word); //free alloc from appendquotes
	}  

	// clean up
	free(input);
	free(oldInput);
	deleteTrie(root);
	return 0; 
} 
