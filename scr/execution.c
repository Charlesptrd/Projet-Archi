#include "execution.h"



void afficher_PC(short int PC, Instruction* l_instruction[500]) {
    Instruction* instruction = l_instruction[PC];
    printf("Instruction -> addr : ");
    printf("\033[32m%d\033[0m", instruction->adresse);
    printf(" code : ");
    printf("\033[31m%02x\033[0m", instruction->code);
    printf(" donnee : ");
    printf("\033[32m%04hx\033[0m\n", instruction->donnée);
    return;
}


void afficher_memoire(short int memoire[5000], short int SP) {
    if (SP == 0) {printf("\033[31mPile Vide\n\033[0m");printf("adr : %d -> %d\n", 1000, memoire[1000]); return;}
    printf("\033[31mPile\n\033[0m");
    for (int i=0; i< SP; i++) {
        printf("adr : %d -> %d\n", i, memoire[i]);
    }
    printf("adr : %d -> %d\n", 1000, memoire[1000]);
    return;
}


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




int execution_instruction(short int* p_PC , short int* p_SP, Instruction* l_instructions[500], short int memoire[5000]) {
    int code = l_instructions[*p_PC]->code;
    int donnee = l_instructions[*p_PC]->donnée;
    afficher_PC(*p_PC, l_instructions);
    (*p_PC) ++; //on incremente directement PC de maniere a ce qu'il indique toujours la prochaine instruction à executer.
    if (code == 0){ //pop x
        memoire[donnee] = memoire[--(*p_SP)];
        return 0;
    }

    else if (code == 1){ //ipop
        memoire[memoire[(*p_SP)-1]] = memoire[(*p_SP)-2];
        (*p_SP)-=2;
        return 0;
    }

    else if (code == 2){ //push x
        memoire[(*p_SP)++] = memoire[donnee];
        return 0;
    }

    else if (code == 3){ //ipush
        memoire[(*p_SP)-1] = memoire[memoire[(*p_SP)-1]];
        return 0;
    }

    else if (code == 4){ //push #i
        memoire[(*p_SP)++] = donnee;
        return 0;
    }

    else if (code == 5){ //jmp adr
        (*p_PC) += donnee;
        return 0;
    }

    else if (code == 6){ //jnz adr
        if (memoire[--(*p_SP)] != 0) {*p_PC += donnee;}
        return 0;
    }

    else if (code == 7){ //call adr
        memoire[(*p_SP)++] = *p_PC+1;
        *p_PC += donnee;
        return 0;
    }

    else if (code == 8){ //ret
        *p_PC = memoire[--(*p_SP)];
        return 0;
    }

    else if (code == 9){ //read x
        printf("Veuillez entrer une valeur : ");
        scanf("%hd", &memoire[donnee]);
        return 0;
    }

    else if (code == 10){ //write x
        printf("\033[35m            Affichage programme -> %d\033[0m\n", memoire[donnee]);
        return 0;
    }

    else if (code == 11){ //op i
        if (donnee == 0) { //égalité
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] == memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 1) { //inégalité
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] != memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 2) { //sup ou égal
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] >= memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 3) { //inf ou égal
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] <= memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 4) { //sup
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] > memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 5) { //inf
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] < memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 6) { //OU
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] | memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 7) { //XOR
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] ^ memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 8) { //ET
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] & memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 9) { //NON
            memoire[(*p_SP)-1] = ~memoire[(*p_SP)-1];
        }
        if (donnee == 10) { //addition
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] + memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 11) { //soustraction
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] - memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 12) { //multiplication
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] * memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 13) { //division
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] / memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 14) { //modulo
            memoire[(*p_SP)-2] = memoire[(*p_SP)-2] % memoire[(*p_SP)-1];
            (*p_SP)--;
        }
        if (donnee == 15) { //inversion
            memoire[(*p_SP)-1] = -memoire[(*p_SP)-1];
        }
        return 0;
    }

    /*
    else if (code == 12){ //rnd x
        srandom(time(NULL));
        memoire[SP++] = random()%(donnee);
        return 0;
    }
    */

    else if (code == 13){ //dup
        memoire[(*p_SP)] = memoire[(*p_SP)-1];
        (*p_SP)++;
        return 0;
    }

    else if (code == 99){ //halt
        return 1;
    }
    return 0;
}