#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"
#include "compile.h"
#include "execution.h"


int main(void) {

    char name[] = "assembleur.txt";
    printf("\033[30;4;47mTranscription\033[0m\n");
    Texte *texte = transcription(name);
    printf("\033[30;4;47mDétection des étiquettes\033[0m\n");
    Labels *labels = Detecter_Label(texte);
    printf("\033[30;4;47mCréation des instructions\033[0m\n");

    Instruction* PC1[500];
    for (int i=0; i<500; i++) {
        PC1[i] = NULL;
    }


    if (initialiser_Instructions_Depuis_Texte(texte, labels, PC1) == 0) {printf("\033[31mErreur lors de la creation des instructions\033[0m\n"); return 1;}


    printf("\033[32mSucces de la creation de la liste des intructions\033[0m\n");

    printf("\033[30;4;47mEcriture du programme en language machine.\033[0m\n");
    ecrire_programme_tranforme(PC1);

    printf("\033[30;4;47mLibération de la mémoire de la premiere partie.\033[0m\n");
    supprimer_texte(texte);
    supprimer_labels(labels);
    tout_supprimer(PC1);
    printf("\033[30;4;47mDémarrage de l'éxécution\033[0m\n");

    Instruction* l_instructions[500];
    short int PC = 0;
    for (int i=0; i< 500; i++) {
        l_instructions[i] = NULL;
    }

    printf("\033[30;4;47mRécupération des instructions\033[0m\n");

    short int memoire[5000];
    short int SP = 0;

    if (extraire_instructions("hexa.txt", l_instructions) == 0) printf("\033[31mErreur lors de la récupération des instructions\033[0m\n");
    afficher_liste_instructions(l_instructions);

    printf("\033[30;4;47mExecution des instructions\033[0m\n");

    while (execution_instruction(&PC, &SP, l_instructions, memoire) == 0) {
        
        afficher_memoire(memoire, SP);
        continue;
    }
    printf("\033[30;4;47mExecution terminee.\033[0m\n");




    printf("\033[30;4;47mLibération de la mémoire\033[0m\n");
    tout_supprimer(l_instructions);
    printf("\033[30;4;47mLe programme est fini\033[0m\n");
    return 0;
}
