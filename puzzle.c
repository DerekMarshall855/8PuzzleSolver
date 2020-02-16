#include "puzzle.h"

/*
Checks to see if the puzzle state given is valid (0-8 all show up exactly once)

Param: 	int *state - List of integers representing a game state
		
Return: 1/true if it is valid, 0/false otherwise (integer)
*/
int valid_state(int *state) {
	int i;
	int seen[9];
	for (i = 0; i < 9; i++) {
		if (state[i] > 8 || state[i] < 0) return 0;
		seen[i] = 0;
	}
	for (i = 0; i < 9; i++) {
		if (seen[state[i]] == 0) {
			seen[state[i]] = 1;
		}
		else return 0;
	}
	return 1;
}
/*
Counts total inversions from default state, if start + end inversions is even then it is solvable

Param: 	PNODE *start - start state of puzzle
		PNODE *end - end/goal state of puzzle
		
Return: 1/true if it is solvable, 0/false otherwise (integer)
*/
int solvable(PNODE* start, PNODE* end) {
	int count = 0, i, j;
	for (i = 0; i < 9; i++) {
		for (j = i + 1; j < 9; j++) {
			if (start -> state[i] && start -> state[j] && start -> state[i] > start -> state[j]) {
				count += 1;
			}
			if (end -> state[i] && end -> state[j] && end -> state[i] > end -> state[j]) {
				count += 1;
			}
		}
	}
	if (count % 2 == 0) {
		return 1;
	}
	
	return 0;
}

/*
Attempts to solve given puzzle with start state and end state
If solvable it will print step by step solution

Param: 	PNODE *start - start state of puzzle
		PNODE *end - end/goal state of puzzle
		
Return: void
*/
void solve_puzzle(PNODE *start, PNODE *end) {
	if (valid_state(start -> state) == 0 || valid_state(end -> state) == 0) {
		printf("Invalid start or end state\n");
		return;
	} 
	printf("start state");
	print_puzzle(&start);
	printf("\ngoal state");
	print_puzzle(&end);
	PNODE *final;
	if (solvable(start, end) == 1) final = iterative_bf_search(start, end);
	else final = NULL;
	if (final == NULL) {
		printf("\nno solution!\n");
	} else {
		printf("\nsolution exists!\n");
		printf("solution found!");
		PNODE *printer = new_puzz(final -> state);
		while (final -> parent) {
			printer -> children[0] = new_puzz(final -> parent -> state);
			printer -> children[0] -> parent = printer;
			printer = printer -> children[0];
			final = final -> parent;
		}
		while (printer) {
			print_puzzle(&printer);
			printer = printer -> parent;
		}
	}
}

/*
Prints state of a given puzzle node

Param: 	PNODE **root - Pointer to PNODE we want to print
		
Return: void
*/
void print_puzzle(PNODE **root) {
	PNODE *a = *root;
	int i;
	for (i = 0; i < 9; i++) {
		if (i % 3 == 0) printf("\n");
		printf("%d ", a -> state[i]);
	}
	printf("\n");
}

/*
Compares 2 puzzle nodes to check  for equality

Param: 	PNODE *first - first puzzle node to compare
		PNODE *second - second puzzle node to compare
		
Return: 1 if equal, 0 otherwise (int)
*/
int puzz_eql(PNODE *first, PNODE *second) {
	int i;
	for (i = 0; i < 9; i++) {
		if (first -> state[i] != second -> state[i]) return 0;
	}
	return 1;
}

/*
Iterative breadthe first solution for puzzle with given start and end state

Param: 	PNODE *root - start state of puzzle
		PNODE *end - end/goal state of puzzle
		
Return: The PNODE* equal to the PNODE* end (PNODE*)
*/
PNODE *iterative_bf_search(PNODE *root, PNODE *end) {
	if (root == NULL) return NULL;
	QNODE *front = NULL, *rear = NULL; //queue for traversal
	PNODE *p = NULL;
	CHECK *valid = (CHECK *) malloc (sizeof(CHECK));
	valid -> length = 0;
	valid -> check = (PNODE**) malloc(sizeof(PNODE*) * (181440)); //Space for every possible solvable state (9!/2)
	valid -> check[valid -> length] = root;
	valid -> length++;
	int i;
	enqueue(&front, &rear, root);
	while (front) {
		p = dequeue(&front, &rear);
		if (puzz_eql(p, end) == 1) {
			clean_queue(&front, &rear);
			free(valid -> check); //Free memory reserved for valid check & valid
			free(valid);
			return p;
		}
		move_puzz(p, valid);
		if (p -> children[0] != NULL) {
			enqueue(&front, &rear, p -> children[0]);
		}
		if (p -> children[1] != NULL) {
			enqueue(&front, &rear, p -> children[1]);
		}
		if (p -> children[2] != NULL) {
			enqueue(&front, &rear, p -> children[2]);
		}
		if (p -> children[3] != NULL) {
			enqueue(&front, &rear, p -> children[3]);
		}
	}
	clean_queue(&front, &rear);
	return NULL;
}

/*
Checks to see if a given PNODE has already been visited

Param: 	CHECK *valid - Contains list of checked puzzle nodes
		PNODE *t - Puzzle node to look for in valid
		
Return: 1 if it has been visited, 0 otherwise (int)
*/
int visited(CHECK *valid, PNODE *t) {
	int i;
	for (i = 0; i < valid -> length; i++) {
		if (puzz_eql(valid->check[i], t) == 1) return 1;
	}
	return 0;
}

/*
Finds location of 0 in given puzzle node and generates movement states for it

Param: 	PNODE *first - Puzzle node to generate movement states of
		CHECK *valid - Contains list of checked puzzle nodes
		
Return: void
*/
void move_puzz(PNODE *first, CHECK *valid) {
	int i, x;
	for (i = 0; i < 9; i++) {
		if (first -> state[i] == 0) {
			x = i;
			break;
		}
	}
	move_left(first, x, valid);
	move_right(first, x, valid);
	move_up(first, x, valid);
	move_down(first, x, valid);
}

/*
Moves 0 in puzzle node first to the left if valid

Param: 	PNODE *first - Puzzle node to generate movement states of
		int i - Location of 0 in Puzzle node
		CHECK *valid - Contains list of checked puzzle nodes
		
Return: void
*/
void move_left(PNODE *first, int i, CHECK *valid) {
	if (i % 3 > 0) {
		PNODE *left = new_puzz(first -> state);
		int temp = left -> state[i-1];
		left -> state[i-1] = left -> state[i];
		left -> state[i] = temp;
		if (visited(valid, left) == 0) {
			valid -> check[valid->length] = left;
			valid -> length++;
			left -> parent = first;
			first -> children[0] = left;
		} else free(left);
	}
}
/*
Moves 0 in puzzle node first to the right if valid

Param: 	PNODE *first - Puzzle node to generate movement states of
		int i - Location of 0 in Puzzle node
		CHECK *valid - Contains list of checked puzzle nodes
		
Return: void
*/
void move_right(PNODE *first, int i, CHECK *valid) {
	if (i % 3 < 2) {
		PNODE *right = new_puzz(first -> state);
		int temp = right -> state[i+1];
		right -> state[i+1] = right -> state[i];
		right -> state[i] = temp;
		if (visited(valid, right) == 0) {
			valid -> check[valid->length] = right;
			valid -> length++;
			right -> parent = first;
			first -> children[1] = right;
		} else free(right);
	}
}
/*
Moves 0 in puzzle node first to the up if valid

Param: 	PNODE *first - Puzzle node to generate movement states of
		int i - Location of 0 in Puzzle node
		CHECK *valid - Contains list of checked puzzle nodes
		
Return: void
*/
void move_up(PNODE *first, int i, CHECK *valid) {
	if (i - 3 >= 0) {
		PNODE *up = new_puzz(first -> state);
		int temp = up -> state[i-3];
		up -> state[i-3] = up -> state[i];
		up -> state[i] = temp;
		
		if (visited(valid, up) == 0) {
			valid -> check[valid->length] = up;
			valid -> length++;
			up -> parent = first;
			first -> children[2] = up;
		} else free(up);
	}
}
/*
Moves 0 in puzzle node first to the down if valid

Param: 	PNODE *first - Puzzle node to generate movement states of
		int i - Location of 0 in Puzzle node
		CHECK *valid - Contains list of checked puzzle nodes
		
Return: void
*/
void move_down(PNODE *first, int i, CHECK *valid) {
	if (i + 3 < 9) {
		PNODE *down = new_puzz(first -> state);
		int temp = down -> state[i+3];
		down -> state[i+3] = down -> state[i];
		down -> state[i] = temp;
		
		if (visited(valid, down) == 0) {
			valid -> check[valid->length] = down;
			valid -> length++;
			down -> parent = first;
			first -> children[3] = down;
		} else free(down);
	}
}

/*
Creates a new puzzle node with no parent or children using given state

Param: 	int *state - Puzzle state to be put into a new puzzle node
		
Return: new puzzle node (PNODE*)
*/
PNODE *new_puzz(int *state) {
	int i;
	PNODE *new = (PNODE *) malloc(sizeof(PNODE));
	for (i = 0; i < 9; i++) {
		new -> state[i] = state[i];
	}
	new -> parent = NULL;
	for (i = 0; i < 4; i++) {
		new -> children[i] = NULL;
	}
	return new;
}

// queue functions adapted and modified from a6q2
void enqueue(QNODE **frontp, QNODE **rearp, void *pdata) {
   QNODE *qnp = (QNODE*) malloc(sizeof(QNODE));
   if (qnp == NULL) return; 
   else { 
     qnp->pdata = pdata;
     qnp->next = NULL;
     
     if (*frontp == NULL) {
      *frontp = qnp;
      *rearp = qnp;
     } else {
      (*rearp)->next = qnp;
      *rearp = qnp; 
    }
   }
}
void *dequeue(QNODE **frontp, QNODE **rearp) {
  void *tnp = NULL;
  if (*frontp) {
    QNODE *qnp = *frontp;
    tnp = qnp->pdata;
    if (*frontp == *rearp) {
      *frontp = NULL;
      *rearp = NULL;
    } else {
      *frontp = qnp->next;
    }
    free(qnp);
    return tnp;
  }
  return NULL;
}
void clean_queue(QNODE **frontp, QNODE **rearp) {
  QNODE *temp, *qnp = *frontp; 
  while (qnp != NULL) {
  temp = *frontp;
  qnp = qnp->next;
  free(temp);
  }
}