#ifndef LECTURE 
#define LECTURE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ajouter_caractere_str(char *str, char c);

typedef struct mot{
    char *str;
    struct mot *next;
} Mot;

Mot *creer_mot(char str[]);
Mot* supprimer_mot(Mot* mot);

typedef struct ligne{
    int card;
    int adresse;
    Mot* debut;
    Mot* fin;
    struct ligne *next;
} Ligne;
int estVide_ligne(Ligne *l);
Ligne *creer_ligne();
Ligne* supprimer_ligne(Ligne* ligne);
void ajouter_mot_ligne(Ligne *l, char *str);

typedef struct texte{
    int card;
    Ligne* debut;
    Ligne* fin;
} Texte;
Texte *creer_texte();
void supprimer_texte(Texte* texte);
int estVide_texte(Texte *t);
void supprimer_derniere_ligne_texte(Texte *texte);
Texte *transcription(char *name);
void afficher_text(Texte *t);




int estVide_ligne(Ligne *l);
Ligne *creer_ligne();
void ajouter_mot_ligne(Ligne *l, char *str);

typedef struct texte Texte;
Texte *creer_texte();
int estVide_texte(Texte *t);
void ajouter_ligne_texte(Texte *t, Ligne* l);
void supprimer_derniere_ligne_texte(Texte *texte);
Texte *transcription(char *name);

short int str_en_short_int(char *str);
int est_int(char* str);
int est_char(char c);

#endif