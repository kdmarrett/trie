# Karl Marrett
# CSE 374 HW5
# Feb 20th
# Makefile for T9 program

# default target
T9: Main.o Trie.o
	gcc -Wall -g -o T9 Main.o Trie.o

# individual source files
Trie.o: Trie.c Trie.h T9.h
	gcc -Wall -g -c Trie.c

Main.o: Main.c Trie.h T9.h 
	gcc -Wall -g -c Main.c
