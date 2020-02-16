#ifndef PUZZLE_H
#define PUZZLE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node  {
    int state[9]; //Puzzles current state
	struct node *children[4];
    struct node *parent; //Pointer to parent puzzle
} PNODE;

typedef struct checked  {
    PNODE **check; //List of checked puzzles
	int length;
} CHECK;

int valid_state(int *state); //Checks valid states
int solvable(PNODE* start, PNODE* end); //Checks if puzzle is solvable
void solve_puzzle(PNODE *start, PNODE *end); //Given start and end, solves and prints puzzle if solvable
void print_puzzle(PNODE **root); //Prints puzzle is 3x3 square
int puzz_eql(PNODE *first,PNODE *second); //Checks if 2 puzzles are equal
PNODE *iterative_bf_search(PNODE *root, PNODE *end); //Searches puzzle tree for solution
int visited(CHECK *valid, PNODE *t); //Checks if given PNODE has already been checked
void move_puzz(PNODE *first, CHECK *valid); //Creates branches for each possible move from location of 0
void move_left(PNODE *first, int i, CHECK *valid); //Moves 0 left and puts that as a child of first in index 0
void move_right(PNODE *first, int i, CHECK *valid); //Moves 0 right and puts that as a child of first in index 1
void move_up(PNODE *first, int i, CHECK *valid); //Moves 0 up and puts that as a child of first in index 2 
void move_down(PNODE *first, int i, CHECK *valid); //Moves 0 down and puts that as a child of first in index 3
PNODE *new_puzz(int *state); //Creates a new puzzle node

//Note: Length of state(9) and column length(3) are currently hard coded

typedef struct queue_node {
  void *pdata;
  struct queue_node *next;
} QNODE;

// All adapted from a6q2
void enqueue(QNODE **frontp, QNODE **rearp, void *pdata);
void *dequeue(QNODE **frontp, QNODE **rearp);
void clean_queue(QNODE **frontp, QNODE **rearp);

#endif