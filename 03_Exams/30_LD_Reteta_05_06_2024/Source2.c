#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LINESIZE 128

typedef struct Reteta {
    unsigned int numarReteta;
    char* numePacient;
    char* numeMedic;
    char* statutSpecial;
    unsigned char nrMedicamente;
    float procentCompensare;
} Reteta;

typedef struct Node {
    Reteta reteta;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} LD;

Reteta initReteta(unsigned int nr, char* pacient, char* medic, char* status, unsigned char nrMed, float comp) {
    Reteta reteta;

    reteta.numarReteta = nr;
    reteta.numePacient = malloc(strlen(pacient) + 1);
    strcpy(reteta.numePacient, pacient);
    reteta.numeMedic = malloc(strlen(medic) + 1);
    strcpy(reteta.numeMedic, medic);
    reteta.statutSpecial = malloc(strlen(status) + 1);
    strcpy(reteta.statutSpecial, status);
    reteta.nrMedicamente = nrMed;
    reteta.procentCompensare = comp;

    return reteta;
}

void printReteta(Reteta reteta) {
    printf("\n-----\nNr reteta: %d\nPacient: %s\nMedic: %s\nStatus: %s\nNr Medicamente: %d\nCompensare: %.2f\n-----\n",
           reteta.numarReteta, reteta.numePacient, reteta.numeMedic,
           reteta.statutSpecial, reteta.nrMedicamente, reteta.procentCompensare);
}

Reteta readReteta(FILE* f) {
    Reteta reteta;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    reteta.numarReteta = (unsigned int)strtol(buffer, NULL, 10);

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

void readRetete(const char* fileName, Reteta** retete, int* nrRetete) {
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    (*nrRetete) = (int)strtol(buffer, NULL, 10);

    (*retete) = malloc((*nrRetete) * sizeof(Reteta));
    for(int i = 0; i < (*nrRetete); i++) {
        (*retete)[i] = readReteta(f);
    }

    fclose(f);
}

void insertAtPosition(LD* ld, int position, Reteta reteta) {
    Node* newNode = malloc(sizeof(Node));
    newNode->reteta = reteta;
    newNode->next = NULL;
    newNode->prev = NULL;

    if(ld->head == NULL) {
        ld->head = newNode;
        ld->tail = newNode;
        return;
    }

    if(position == 0) {
        newNode->next = ld->head;
        ld->head->prev = newNode;
        ld->head = newNode;
        return;
    }

    Node* current = ld->head;
    int index = 0;

    while(current != NULL && index < position - 1) {
        current = current->next;
        index++;
    }

    if(current == NULL) {
        ld->tail->next = newNode;
        newNode->prev = ld->tail;
        ld->tail = newNode;
    } else {
        newNode->next = current->next;
        newNode->prev = current;
        if(current->next != NULL) {
            current->next->prev = newNode;
        }
        current->next = newNode;
        if(newNode->next == NULL) {
            ld->tail = newNode;
        }
    }
}

void printList(LD ld) {
    Node* current = ld.head;
    while(current != NULL) {
        printReteta(current->reteta);
        current = current->next;
    }
}

void freeReteta(Reteta reteta) {
    free(reteta.numePacient);
    free(reteta.numeMedic);
    free(reteta.statutSpecial);
}

void freeList(LD* ld) {
    Node* current = ld->head;
    while(current != NULL) {
        Node* next = current->next;
        freeReteta(current->reteta);
        free(current);
        current = next;
    }
    ld->head = NULL;
    ld->tail = NULL;
}

int main() {
    Reteta* retete;
    int nrRetete;

    readRetete("retete.txt", &retete, &nrRetete);

    printf("\n----------Retete din fisier----------\n");
    for(int i = 0; i < nrRetete; i++) {
        printReteta(retete[i]);
    }

    LD ld;
    ld.head = NULL;
    ld.tail = NULL;

    for(int i = 0; i < nrRetete; i++) {
        insertAtPosition(&ld, i, retete[i]);
    }

    printf("\n----------Retete din lista dubla----------\n");
    printList(ld);

    printf("\n----------Adaugare reteta 8 in lista dubla pe pozitia 3----------\n");
    insertAtPosition(&ld, 3, initReteta(8, "Mihai Bobonete", "Dr. Ion Albescu", "asigurat", '3', 13));

    printf("\n----------Retete din lista dubla----------\n");
    printList(ld);

    freeList(&ld);
    for(int i = 0; i < nrRetete; i++) {
        freeReteta(retete[i]);
    }
    free(retete);

    return 0;
}
