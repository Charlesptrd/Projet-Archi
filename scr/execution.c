#include "execution.h"





int extraire_instructions(char* nom, Instruction* PC[500]) {
    FILE* fichier = fopen(nom, "r");
    if (!fichier) {printf("\033[31mImpossible d'ouvrir le fichier '%s'.\033[0m\n", nom); return 0;}
    int code;
    int donnee;
    int i=0;
    Instruction* courant;
    while (fscanf(fichier, "%x %x\n", &code, &donnee) != EOF) {
        courant = creation_instruction(i, code, (short int)donnee);
        PC[i] = courant;
        i++;
    }
    return 1;
}