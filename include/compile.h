#ifndef COMPILE
#define COMPILE
#include "lecture.h"



typedef struct instruction{
    int adresse; // 0 -> 4999
    char code; // code de l'instruction de 0 à 13 ou 99 pour signaler la fin (1 octet)
    short int donnée; // paramètres de l'instruction (2 octets)
    struct instruction* next;
    struct instruction* prev;
}Instruction;
Instruction* creation_instruction(int adresse, char code, short donnée);

typedef struct {
    Instruction* debut;
    Instruction* fin;
    Instruction* PC;
    Instruction* SP;
}Liste_instructions;
Liste_instructions* creation_liste_instructions();
void tout_supprimer(Liste_instructions* l_instructions);

void afficher_instruction(Instruction* instruction);
void afficher_liste_instructions(Liste_instructions* l_instructions);


void ajout_instruction(Liste_instructions* l_instructions,Instruction* instruction);
void execution_instruction(Liste_instructions* l_instructions);

typedef struct label {
    short int adr; //nombre entier signé sur 2 octets
    char* name;
    struct label *next;
} Label;

Label *creer_label(int adr, char* name);
Label *supprimer_label(Label* label);

typedef struct labels {
    Label *debut;
    int card;
} Labels;

Labels *creer_labels();
void supprimer_labels(Labels* labels);
int estVide_Labels(Labels *l);
void ajouter_label(Labels *labels, int adr, char *name);
void afficher_labels(Labels *labels);

Labels *Detecter_Label(Texte *t);
Label* chercher_label(Labels* labels, char* nom);

int calcul_saut_label(Instruction* depart, Label* label);



int InstructionName_to_InstructionNB(char *str);

int initialiser_Instructions_Depuis_Texte(Texte *texte, Labels* labels, Liste_instructions* l_inctructions);
#endif