#ifndef EXECUTION
#define EXECUTION
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "compile.h"

int extraire_instructions(char* nom, Instruction* l_instructions[500]);
int execution_instruction(short int* PC , short int* SP, Instruction* l_instruction[500], short int memoire[5000]);

void afficher_PC(short int PC, Instruction* l_instruction[500]);
void afficher_memoire(short int memoire[5000], short int SP);





#endif



