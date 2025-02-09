

#include "lecture.h"


Mot* creer_mot(char str[]) {
    Mot *res = malloc(sizeof(Mot));
    char *new = malloc(sizeof(char)*(strlen(str)+1));
    strcpy(new, str);
    res->next = NULL;
    res->str = new;
    return res;
}
Mot* supprimer_mot(Mot* mot) {
    Mot* tmp = mot->next;
    free(mot->str);
    free(mot);
    return tmp;
}

int estVide_ligne(Ligne *l){
    if (l->debut == NULL) return 1;
    else return 0;
}

Ligne *creer_ligne() {
    Ligne *res = malloc(sizeof(Ligne));
    res->card = 0;
    res->debut = NULL;
    res->fin = NULL;
    res->next = NULL;
    res->adresse = -1;
    return res;
}
Ligne* supprimer_ligne(Ligne* ligne) {
    Ligne* tmp = ligne->next;
    while (estVide_ligne(ligne) == 0) {
        ligne->debut = supprimer_mot(ligne->debut);
    }
    free(ligne);
    return tmp;
}

void ajouter_mot_ligne(Ligne *l, char *str) {
    Mot *mot = creer_mot(str);

    if (estVide_ligne(l)) l->debut = mot;
    else l->fin->next = mot;
    l->fin = mot;
    l->card += 1;
    
}

void afficher_ligne(Ligne *l) {
    if (estVide_ligne(l) == 1) {
        printf("La ligne est vide\n");
        return;}

    printf("La ligne possede %d mot(s) : ", l->card);
    Mot *m = l->debut;
    for (int i=0; i < l->card; i++) {
        printf("%d : %s/", (i+1), m->str);

        if (m->next != NULL) {m = m->next;}
        else break;
    }
    printf("\n");
}



Texte *creer_texte() {
    Texte *res = malloc(sizeof(Texte));
    res->card = 0;
    res->debut = NULL;
    res->fin = NULL;
    return res;
}

void supprimer_texte(Texte* texte) {
    while (estVide_texte(texte) == 0) {
        texte->debut = supprimer_ligne(texte->debut);
    }
    free(texte);
    return;
}

void ajouter_ligne_texte(Texte *t, Ligne* l) {

    if (estVide_texte(t)) {t->debut = l;}
    else t->fin->next = l;
    t->fin = l;
    t->card += 1;
    return;
}

int estVide_texte(Texte *t) {
    if (t->debut == NULL) {
        return 1;
    }
    return 0;
}

void afficher_text(Texte *t) {
    if (estVide_texte(t)) {printf("Le texte est vide\n"); return;}

    printf("\nLe texte contient %d lignes.\n", t->card);
    Ligne* c=t->debut;
    while (c != NULL) {
        afficher_ligne(c);
        c = c->next;
    }
    return;
}

void supprimer_derniere_ligne_texte(Texte *texte) {
    Ligne *l = texte->debut;
    while (l->next != texte->fin) l = l->next;
    free(l->next);
    l->next = NULL;
    texte->fin = l;
    texte->card -=1;
    return;
}

char *ajouter_caractere_str(char *str, char c) {
    size_t taille = strlen(str);
    char *new = malloc(sizeof(char)*(taille+2));
    strcpy(new, str);
    new[taille] = c;
    new[taille+1] = '\0';
    free(str);
    return new;
}



int est_char(char c) {
    if (c <= 'z' && c>= 'a') return 1;
    else return 0;
}

int est_int(char* str) {
    char *endptr = NULL;
    long value = strtol(str, &endptr, 10);

    if (value < -32768 || value >32767)  {
        return -1; //si la valeur ne peut pas etre stockée sur 2 octer alors on affichera un waring de dépassement
    }

    if (endptr == &str[strlen(str)]) return 1;



    return 0;
}


Texte *transcription(char *name) {
    FILE *file = fopen(name, "r");
    if (!file) {
        printf("\033[31mErreur, impossible d'ouvrir le fichier : %s.\033[0m\n", name);
        return NULL;
    }
    int i=0;
    Texte *texte = creer_texte();
    Ligne* l = creer_ligne();
    l->adresse = i;

    ajouter_ligne_texte(texte, l);
    char c;
    int nb = 0;
    char *str = malloc(1);
    str[0] = '\0';
    while ((c=fgetc(file))) {

        if (c!='\0' && c!=' ' && c != '\n') {
            str = ajouter_caractere_str(str, c); 
            nb+=1;

        }
        
        if (c == ' ' || c == '\n' || c == '\0') {
            if (nb > 0) {
                ajouter_mot_ligne(texte->fin, str);
                nb = 0;
                free(str);
                str = malloc(1);
                str[0] = '\0';
            }   
        }

        if (c == '\n') {
            if (estVide_ligne(texte->fin)) {supprimer_derniere_ligne_texte(texte);}
            else i++;
            l = creer_ligne();
            l->adresse = i;
            ajouter_ligne_texte(texte, l);
        }
        

        if (c == EOF) break;
    }
    if (estVide_ligne(texte->fin)) {supprimer_derniere_ligne_texte(texte);}
    if (str != NULL)free(str);

    fclose(file);

    return texte;

}




short int str_en_short_int(char *str) {
    char *endptr = str+strlen(str);
    long value = strtol(str, &endptr, 10);

    return (short int)value;
}


int ecrire_programme_tranforme(Instruction* PC[500]) {
    FILE* fichier = fopen("hexa.txt", "w");
    if (!fichier) {
        printf("\033[31mErreur lors de la création du fichier.\033[0m\n");
        return 0;
    }
    int i=0;
    while (PC[i] != NULL) {
        fprintf(fichier, "%02hx %04hx\n", PC[i]->code, PC[i]->donnée);
        i++;
    }
    fclose(fichier);
    return 1;
}






