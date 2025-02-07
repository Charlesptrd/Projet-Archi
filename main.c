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
    printf("\033[30;4;47mInitialisation de la liste des instructions\033[0m\n");

    Liste_instructions* l_instructions = creation_liste_instructions();
    if (initialiser_Instructions_Depuis_Texte(texte, labels, l_instructions) == 0) {printf("\033[31mErreur lors de l'initialisation des instructions\033[0m\n"); return 1;}


    printf("\033[32mSucces de l'initialisation de la liste des intructions\033[0m\n");


    afficher_liste_instructions(l_instructions);

    printf("\033[30;4;47mLibération de la mémoire\033[0m\n");
    supprimer_texte(texte);
    supprimer_labels(labels);
    tout_supprimer(l_instructions);
    printf("\033[30;4;47mLe programme est fini\033[0m\n");
    return 0;
}