/*
 Project	 : cp264 Assignment 10
 Name        : a10q4.c -- test driver
 Author      : Derek Marshall
 Student ID	 : 170223090
 Email		 : mars3090@mylaurier.ca
 Version     : 2019-04-02
 */

#include "puzzle.h"
#include <string.h>

int main(int argc, char *args[]){
  //create a puzzle
  int state[9] = {1, 2, 3, 4, 5, 6, 0, 7, 8}; //default start state
  int last[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0}; //default goal state
  char temp[20], delimiters[] = ".,\n\t\r", *token;
  int i;
  if (argc > 1) {
	if (argc >= 2) {
	  i = 0;
	  strcpy(temp, args[1]);
	  token = (char*) strtok(temp,  delimiters);
	  state[i] = atoi(token);
	  i++;
	  while (token && i < 9) {
		  token = (char*) strtok(NULL,  delimiters);
		  state[i] = atoi(token);
		  i++;
	  }
	}
	if (argc >= 3) {
	  i = 0;
	  strcpy(temp, args[2]);
	  token = (char*) strtok(temp,  delimiters);
	  last[i] = atoi(token);
	  i++;
	  while (token && i < 9) {
		  token = (char*) strtok(NULL,  delimiters);
		  last[i] = atoi(token);
		  i++;
	  }
	}
  }  
  PNODE *first = new_puzz(state);
  PNODE *end = new_puzz(last);
  solve_puzzle(first, end);

  return 0;
}