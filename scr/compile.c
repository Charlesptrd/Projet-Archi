#include "compile.h"


char *Instruction_Name[] = {"pop", "ipop", "push", "ipush", "push#", "jmp", "jnz", "call", "ret", "read", "write", "op", "rnd", "dup", "halt"};
int Instruction_nb[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 99};


Liste_instructions* creation_liste_instructions(){
    Liste_instructions* instructions = malloc(sizeof(Liste_instructions));
    instructions->debut = NULL;
    instructions->fin = NULL;
    instructions->PC = NULL;
    instructions->SP = NULL;
    return instructions;
}

Instruction* creation_instruction(int adresse, char code, short donnée){
    Instruction* instruction = malloc(sizeof(Instruction));
    instruction->adresse = adresse;
    instruction->code = code;
    instruction->donnée = donnée;
    instruction->next = NULL;
    instruction->prev = NULL;
    return instruction;
}

void ajout_instruction(Liste_instructions* l_instructions,Instruction* instruction){
    if (l_instructions->fin == NULL){ // la liste est vide.
        l_instructions->debut = instruction;
        l_instructions->fin = instruction;
        l_instructions->PC = instruction;
    }
    
    else { //s'il n'est pas vide, on ajoute a la fin.
        instruction->prev = l_instructions->fin;
        instruction->next = NULL;
        l_instructions->fin->next = instruction;
        l_instructions->fin = instruction;
    }
    return;
}

void tout_supprimer(Instruction* PC[500]){
    Instruction* tmp;
    int i=0;
    while (PC[i] != NULL){
        free(PC[i]);
        i++;
    }
    return;
}



void afficher_instruction(Instruction* instruction) {
    printf("Instruction -> addr : ");
    printf("\033[32m%d\033[0m", instruction->adresse);
    printf(" code : ");
    printf("\033[31m%02x\033[0m", instruction->code);
    printf(" donnee : ");
    printf("\033[32m%04hx\033[0m\n", instruction->donnée);
    return;
}

void afficher_liste_instructions(Instruction* PC[500]){
    int i=0;
    while (PC[i] != NULL) {
        afficher_instruction(PC[i]);
        i++;
    }
    return;
}


Label *creer_label(int adr, char* name) {
    Label *res = malloc(sizeof(Label));
    res->adr = adr; //adresse de l'instruction a laquelle est associée le label.
    res->name = malloc(sizeof(char)*(strlen(name)));
    strncpy(res->name, name, strlen(name)-1);
    res->name[strlen(name)-1] = '\0';
    res->next = NULL;
    return res;
}
Label *supprimer_label(Label* label) {
    Label* tmp = label->next;
    free(label->name);
    free(label);
    return tmp;
}



Labels *creer_labels() {
    Labels *res = malloc(sizeof(Labels));
    res->card = 0;
    res->debut = NULL;
    return res;
}
void supprimer_labels(Labels* labels) {
    while (estVide_Labels(labels) == 0) {

        labels->debut = supprimer_label(labels->debut);
        labels->card -= 1;
    } 
    free(labels);
    return;
}


int estVide_Labels(Labels *l) {

    if (l->card == 0) return 1;
    else return 0;
}

void ajouter_label(Labels *labels, int adr, char *name) {
    Label *l = creer_label(adr, name);
    l->next = labels->debut;
    labels->debut = l;
    labels->card +=1;
    return;
}

void afficher_labels(Labels *labels) {
    if (estVide_Labels(labels)) {printf("Il n'y a aucun label"); return;}
    Label *l = labels->debut;
    while (l != NULL) {
        printf("Nom : '%s' , adresse : %d\n", l->name, l->adr);
        l = l->next;
    }
    return;
}


int InstructionName_to_InstructionNB(char *str) {
    for (int i=0; i<15; i++) {
        if (strcasecmp(str, Instruction_Name[i])==0) return Instruction_nb[i];
    }
    return -1;
}

int est_label(char* chaine) {
    if (chaine[strlen(chaine)-1] == ':') return 1;
    return 0;
}

Label* chercher_label(Labels* labels, char* nom) {
    Label* courant = labels->debut;
    while (courant != NULL) {
        if (strcmp(courant->name, nom) == 0) {return courant;}
        courant = courant->next;
    }
    return NULL;
}


Labels *Detecter_Label(Texte *t) { // attribut aussi a une ligne son adresse. Permet par la suite le calcul des sauts.
    Labels *labels = creer_labels();
    if (estVide_texte(t)){return labels;}
    Ligne *l = t->debut;
    int i=0;
    while (l != NULL) {
        Mot *m = l->debut;
        while (m != NULL) {
            if (est_label(m->str) == 1)ajouter_label(labels, i, m->str);
            m = m->next;
        }
        l->adresse = i;
        l = l->next;
        i+=1;
    }
    return labels;
}

int calcul_saut_label(Instruction* depart, Label* label) {
    if (depart->adresse < label->adr) return label->adr - depart->adresse -1;
    if (depart->adresse > label->adr) return label->adr - depart->adresse -1;
    else return 0;
}

int initialiser_Instructions_Depuis_Texte(Texte *texte, Labels* labels, Instruction* PC[500]) { //s'il y a un probeleme lors de cette phase, erreur de synthace de code ... la fonction renverra null avec un message d'erreur.
    Ligne* ligne = texte->debut;
    Mot* mot;
    int i=0;
    while (ligne != NULL) {
        mot = ligne->debut;

        if (est_label(mot->str) == 1) {mot = mot->next;} // si le premier mot de la ligne est une etiquelle, on la passe car elle n'apparait pas dans le code machine mais serre a calculer les saut.
        Instruction* instruction = creation_instruction(ligne->adresse, InstructionName_to_InstructionNB(mot->str), 0);

        //instructions de saut : jmp, jnz, call
        if (instruction->code == 5 || instruction->code == 6 || instruction->code == 7) { 
            if (mot->next == NULL) {printf("\033[31mErreur ligne %d : le code instruction : %d necessite un argument (etiquette ou saut d'instructions).\033[0m\n", ligne->adresse, instruction->code); free(instruction); return 0;}
            Label* l=chercher_label(labels, mot->next->str);

            if (est_char(mot->next->str[0]) == 1 && l != NULL) instruction->donnée = calcul_saut_label(instruction, l);
            else if (est_int(mot->next->str) == 1 ) instruction->donnée = str_en_short_int(mot->next->str);
            
            else {
                printf("\033[31mErreur ligne %d : argument invalide pour un code instruction : %d.\033[0m\n", ligne->adresse, instruction->code);
                free(instruction);
                return 0;
            }
        }

        //instruction avec adresse x : pop, push, read, write , rnd
        if (instruction->code == 0 || instruction->code == 2 || instruction->code == 9 || instruction->code == 10 || instruction->code == 12) {
            if (mot->next == NULL) {printf("\033[31mErreur ligne %d : le code instruction : %d necessite un argument (adresse).\033[0m\n", ligne->adresse, instruction->code); free(instruction); return 0;}

            short int nb = str_en_short_int(mot->next->str);
            if (nb < 0 || nb > 4999) {printf("\033[31mErreur ligne %d : une adresse doit etre contenue entre 0 et 4999.\033[0m\n", ligne->adresse); free(instruction); return 0;}
            instruction->donnée = nb;
        }


        //instruction avec une valeur push#
        if (instruction->code == 4 || instruction->code == 11) {
            if (mot->next == NULL) {printf("\033[31mErreur ligne %d : le code instruction : %d necessite un argument (valeur entiere).\033[0m\n", ligne->adresse, instruction->code); free(instruction); return 0;}
            if (est_int(mot->next->str) == 0 || est_char(mot->next->str[0])) {printf("\033[31mErreur ligne %d : le code instruction : %d necessite un argument de type short int.\033[0m\n", ligne->adresse, instruction->code); free(instruction); return 0;}
            if (est_int(mot->next->str) == -1) {printf("\033[35mWarning ligne %d : le code instruction : %d necessite un argument de type short int compris entre -32768 et 32767.\033[0m\n", ligne->adresse, instruction->code);}
            short int nb = str_en_short_int(mot->next->str);
            instruction->donnée = nb;
        }

            


        PC[i] = instruction;

        ligne = ligne->next;
        i++;
    }


    return 1;
}


