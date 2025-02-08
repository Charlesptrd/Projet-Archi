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

    Instruction* PC[500];
    for (int i=0; i< 500; i++) {
        PC[i] = NULL;
    }

    printf("\033[30;4;47mRécupération des instructions\033[0m\n");
    if (extraire_instructions("hexa.txt", PC) == 0) printf("\033[31mErreur lors de la récupération des instructions\033[0m\n");
    afficher_liste_instructions(PC);



    printf("\033[30;4;47mLibération de la mémoire\033[0m\n");
    tout_supprimer(PC);
    printf("\033[30;4;47mLe programme est fini\033[0m\n");
    return 0;
}
