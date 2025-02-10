#include "execution.h"



void afficher_PC(short int PC, Instruction* l_instructions[500]) {
    Instruction* instruction = l_instructions[PC];
    printf("Instruction -> addr : ");
    printf("\033[32m%d\033[0m", instruction->adresse);
    printf(" code : ");
    printf("\033[31m%02x\033[0m", instruction->code);
    printf(" donnee : ");
    printf("\033[32m%04hx\033[0m\n", instruction->donnée);
    return;
}


void afficher_memoire(short int memoire[5000], short int SP) {
    if (SP == 0) {printf("\033[31mPile Vide SP=%d\n\033[0m", SP);}
    else printf("\033[31mPile SP=%d\n\033[0m", SP);
    for (int i=0; i< SP; i++) {
        printf("adr : %d -> %d\n", i, memoire[i]);
    }
    printf("adr : %d -> %d\n", 10, memoire[10]);
    //printf("adr : %d -> %d\n", 101, memoire[101]);
    return;
}


int extraire_instructions(char* nom, Instruction* l_instructions[500]) {
    FILE* fichier = fopen(nom, "r");
    if (!fichier) {printf("\033[31mImpossible d'ouvrir le fichier '%s'.\033[0m\n", nom); return 0;}
    int code;
    int donnee;
    int i=0;
    Instruction* courant;
    while (fscanf(fichier, "%x %x\n", &code, &donnee) != EOF) {
        courant = creation_instruction(i, code, (short int)donnee);
        l_instructions[i] = courant;
        i++;
    }
    return 1;
}




int execution_instruction(short int* p_PC , short int* p_SP, Instruction* l_instructions[500], short int memoire[5000]) {
    if (!(*p_PC >= 0 && *p_PC < 500)) {printf("\033[31mErreur le pointeur d'instruction PC n'est pas valide.\033[0m\n"); return -1;}
    int code = l_instructions[*p_PC]->code;
    int donnee = l_instructions[*p_PC]->donnée;
    //afficher_PC(*p_PC, l_instructions);
    (*p_PC) ++; //on incremente directement PC de maniere a ce qu'il indique toujours la prochaine instruction à executer.
    if (code == 0){ //pop x
        
        if (donnee < 5000 && donnee >=0 && (*p_SP)-1 < 5000 && (*p_SP)-1 >= 0){
            --(*p_SP); 
            memoire[donnee] = memoire[(*p_SP)];
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 1){ //ipop
        if ((*p_SP)-2 >= 0 && (*p_SP)-1 < 5000 && memoire[(*p_SP)-1] >= 0 && memoire[(*p_SP)-1] < 5000) {
            memoire[memoire[(*p_SP)-1]] = memoire[(*p_SP)-2];
            (*p_SP)-=2;
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 2){ //push x
        if ((*p_SP)+1 < 5000 && (*p_SP)+1 >=0 && donnee < 5000 && donnee >=0) {
            memoire[(*p_SP)++] = memoire[donnee];
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 3){ //ipush
        if ((*p_SP)-1 >= 0 && (*p_SP)-1 < 5000 && memoire[(*p_SP)-1] >= 0 && memoire[(*p_SP)-1] < 5000) {
            memoire[(*p_SP)-1] = memoire[memoire[(*p_SP)-1]];
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 4){ //push #i
        if ((*p_SP) >= 0 && (*p_SP) < 5000) {
            memoire[(*p_SP)++] = donnee;
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 5){ //jmp adr
        (*p_PC) += donnee;
        return 0;
    }

    else if (code == 6){ //jnz adr
        if ((*p_SP)-1 >= 0 && (*p_SP)-1 < 5000) {
            if (memoire[--(*p_SP)] != 0) {(*p_PC) += donnee;}
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 7){ //call adr
        if ((*p_SP)>=0 && (*p_SP) < 5000) {
            memoire[(*p_SP)++] = (*p_PC);
            (*p_PC) += donnee;
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 8){ //ret
        if ((*p_SP)-1 >= 0 && (*p_SP)-1 < 5000) {
            (*p_PC) = memoire[--(*p_SP)];
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 9){ //read x
        if (donnee >= 0 && donnee < 5000) {
            printf("Veuillez entrer une valeur : ");
            int a;
            scanf("%d", &a);
            memoire[donnee] = (short int) a;
            if (!(a <= 32767 && a>= -32868)) {printf("\033[35mWarning le code instruction : %d necessite un argument de type short int compris entre -32768 et 32767.\033[0m\n", code);}
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 10){ //write x
        if (donnee >= 0 && donnee < 5000) {
            printf("\033[35mAffichage programme -> %d\033[0m\n", memoire[donnee]);
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 11){ //op i
        if ((*p_SP)-2 >= 0 && (*p_SP)-1 < 5000) {
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
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    
    else if (code == 12){ //rnd x
        if ((*p_SP)+1 >=0 && (*p_SP)+1 < 5000) {
            srand(time(NULL));

            short int nombre = (short)(rand() % 65536 - 32768);
            nombre = nombre % donnee;
            if (nombre*donnee < 0) nombre *= -1;
            memoire[(*p_SP)++] = nombre;
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }
    

    else if (code == 13){ //dup
        if ((*p_SP)-1 >= 0 && (*p_SP) < 5000) {
            memoire[(*p_SP)] = memoire[(*p_SP)-1];
            (*p_SP)++;
            return 0;}
        else printf("\033[31mErreur vous essayez d'acceder a un zone memoire de l'espace de travail qui n'existe pas.\033[0m\n");
    }

    else if (code == 99){ //halt
        return 1;
    }
    return -1;
}