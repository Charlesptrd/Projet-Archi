#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"
#include "compile.h"
#include "execution.h"


int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("\033[31mErreur : Aucun fichier spécifié.\033[0m\n");
        printf("Utilisation : %s <nomfichier>\n", argv[0]);
        return EXIT_FAILURE; 
    }

    printf("\033[32mFichier fourni : %s\033[0m\n", argv[1]);

    printf("\033[30;4;47mTranscription\033[0m\n");
    Texte *texte = transcription(argv[1]);
    if (texte == NULL) {return EXIT_FAILURE;}
    printf("\033[30;4;47mDétection des étiquettes\033[0m\n");
    Labels *labels = Detecter_Label(texte);
    printf("\033[30;4;47mCréation des instructions\033[0m\n");

    Instruction* l_instructions_1[500];
    for (int i=0; i<500; i++) {
        l_instructions_1[i] = NULL;
    }


    if (initialiser_Instructions_Depuis_Texte(texte, labels, l_instructions_1) == 0) {printf("\033[31mErreur lors de la creation des instructions\033[0m\n");
    supprimer_texte(texte);
    supprimer_labels(labels);
    tout_supprimer(l_instructions_1); return 0;}

    ecrire_programme_tranforme(l_instructions_1);

    printf("\033[30;4;47mLibération de la mémoire de la premiere partie.\033[0m\n");
    supprimer_texte(texte);
    supprimer_labels(labels);
    tout_supprimer(l_instructions_1);
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

    printf("\033[30;4;47mExecution des instructions\033[0m\n");

    int dernier;

    do  {
        dernier = execution_instruction(&PC, &SP, l_instructions, memoire);
    } while (dernier == 0);
    if (dernier == 1)printf("\033[30;4;47mExecution terminee.\033[0m\n");
    else printf("\033[31mErreur lors de l'execution des instructions\033[0m\n");
    




    printf("\033[30;4;47mLibération de la mémoire\033[0m\n");
    tout_supprimer(l_instructions);
    printf("\033[30;4;47mLe programme est fini\033[0m\n");
    return 0;
}
