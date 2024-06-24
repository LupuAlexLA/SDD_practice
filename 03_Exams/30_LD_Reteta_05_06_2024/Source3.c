#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>

#define LINESIZE 128

typedef struct Reteta Reteta;
typedef struct Node Node;
typedef struct LD LD;

struct Reteta {
    unsigned int numarReteta;
    char* numePacient;
    char* numeMedic;
    char* statutSpecial;
    unsigned char nrMedicamente;
    float procentCompensare;
};

struct Node {
    Reteta reteta;
    Node* next;
    Node* prev;
};

struct LD {
    Node* head;
    Node* tail;
};

Reteta initReteta(unsigned int numarReteta, char* numePacient, char* numeMedic, char* statutSpecial, unsigned char nrMedicamente, float procentCompensare) {
    Reteta reteta;
    reteta.numarReteta = numarReteta;
    reteta.numePacient = malloc(strlen(numePacient) + 1);
    strcpy(reteta.numePacient, numePacient);
    reteta.numeMedic = malloc(strlen(numeMedic) + 1);
    strcpy(reteta.numeMedic, numeMedic);
    reteta.statutSpecial = malloc(strlen(statutSpecial) + 1);
    strcpy(reteta.statutSpecial, statutSpecial);
    reteta.nrMedicamente = nrMedicamente;
    reteta.procentCompensare = procentCompensare;
    return reteta;
}

void afiseazaReteta(Reteta reteta) {
    printf("\n-----\nNumar reteta: %d\nNume pacient: %s\nNume medic: %s\nStatut special: %s\nNr medicamente: %d\nProcent compensare: %.2f\n-----\n",
           reteta.numarReteta, reteta.numePacient, reteta.numeMedic, reteta.statutSpecial, reteta.nrMedicamente, reteta.procentCompensare);
}

Reteta citesteReteta(FILE* f) {
    Reteta reteta;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    reteta.numarReteta = (int)strtol(buffer, NULL, 10);
    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    reteta.numePacient = malloc(strlen(buffer) + 1);
    strcpy(reteta.numePacient, buffer);
    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    reteta.numeMedic = malloc(strlen(buffer) + 1);
    strcpy(reteta.numeMedic, buffer);
    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    reteta.statutSpecial = malloc(strlen(buffer) + 1);
    strcpy(reteta.statutSpecial, buffer);
    fgets(buffer, LINESIZE, f);
    reteta.nrMedicamente = (unsigned char)strtol(buffer, NULL, 10);
    fgets(buffer, LINESIZE, f);
    reteta.procentCompensare = strtof(buffer, NULL);

    return reteta;
}

void citesteRetete(char* numeFisier, Reteta** retete, int* nrRetete) {
    FILE* f = fopen(numeFisier, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    (*nrRetete) = (int)strtol(buffer, NULL, 10);

    (*retete) = malloc((*nrRetete) * sizeof(Reteta));
    for (int i = 0; i < (*nrRetete); i++) {
        (*retete)[i] = citesteReteta(f);
    }

    fclose(f);
}

void insereazaPePozitie(LD* ld, Reteta reteta, int pozitie) {
    Node* newNode = malloc(sizeof(Reteta));
    newNode->reteta = reteta;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (ld->head == NULL) {
        ld->head = newNode;
        ld->tail = newNode;
        return;
    }

    if (pozitie == 0) {
        newNode->next = ld->head;
        ld->head->prev = newNode;
        ld->head = newNode;
        return;
    }

    Node* current = ld->head;
    int index = 0;

    while (current != NULL && index < pozitie - 1) {
        current = current->next;
        index++;
    }

    if (current == NULL) { // cazul in care am pozitii disponibile 0-4 dar eu zic ca inserez pe pozitia 6
        ld->tail->next = newNode;
        newNode->prev = ld->tail;
        ld->tail = newNode;
    }
    else {
        newNode->next = current->next;
        newNode->prev = current;
        if (current->next != NULL) {
            current->next->prev = newNode;
        }
        current->next = newNode;
        if (newNode->next == NULL) {
            ld->tail = newNode;
        }
    }
}

void afiseazaReteteDinListaSD(LD ld) {
    if (ld.head) {
        while (ld.head) {
            afiseazaReteta(ld.head->reteta);
            ld.head = ld.head->next;
        }
    }
    else {
        printf("\n\tLista goala\n");
    }
}

int main() {
    Reteta* retete;
    int nrRetete;

    citesteRetete("retete.txt", &retete, &nrRetete);
    printf("\n----------Retete din fisier----------\n");
    for (int i = 0; i < nrRetete; i++) {
        afiseazaReteta(retete[i]);
    }

    LD ld;
    // am uitat sa le pun
    ld.head = NULL;
    ld.tail = NULL;

    printf("\n----------Inserare retete in LD specificand pozitia----------\n");
    for (int i = 0; i < nrRetete; i++) {
        insereazaPePozitie(&ld, retete[i], i);
    }

    printf("\n----------Retete din LD----------\n");
    afiseazaReteteDinListaSD(ld);

    printf("\n----------Inserare reteta noua in LD specificand pozitia (3)----------\n");
    Reteta newReteta1 = initReteta(8, "Mihai Bucur", "Paul Lazar", "asigurat", 3, 20);
    insereazaPePozitie(&ld, newReteta1, 3);

    printf("\n----------Retete din LD----------\n");
    afiseazaReteteDinListaSD(ld);

    printf("\n----------Inserare reteta noua in LD specificand pozitia (0)----------\n");
    Reteta newReteta2 = initReteta(9, "Catalina Ionescu", "Gabriel Ene", "neasigurat", 2, 25);
    insereazaPePozitie(&ld, newReteta2, 0);

    printf("\n----------Retete din LD----------\n");
    afiseazaReteteDinListaSD(ld);

    printf("\n----------Inserare reteta noua in LD specificand pozitia (20)----------\n");
    Reteta newReteta3 = initReteta(10, "Horia Badea", "Ramon Nastase", "asigurat", 5, 35);
    insereazaPePozitie(&ld, newReteta3, 20);

    printf("\n----------Retete din LD----------\n");
    afiseazaReteteDinListaSD(ld);

    return 0;
}