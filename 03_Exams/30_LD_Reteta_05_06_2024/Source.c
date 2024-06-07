#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

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

void readRetete(char* fileName, Reteta** retete, int* nrRetete) {
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

void insertAtB(LD* ld, Reteta reteta) {
    Node* newNode = malloc(sizeof(Node));
    newNode->reteta = reteta;
    newNode->prev = NULL;
    newNode->next = (*ld).head;
    if((*ld).head) {
        (*ld).head->prev = newNode;
    }
    else {
        (*ld).tail = newNode;
    }
    (*ld).head = newNode;
}

void insertAtE(LD* ld, Reteta reteta) {
    Node* newNode = malloc(sizeof(Node));
    newNode->reteta = reteta;
    newNode->next = NULL;
    newNode->prev = (*ld).tail;
    if((*ld).tail) {
        (*ld).tail->next = newNode;
    }
    else {
        (*ld).head = newNode;
    }
    (*ld).tail = newNode;
}

void insertAtPosition(LD* ld, int position, Reteta reteta) {
    Node* newNode = malloc(sizeof(Node));
    newNode->reteta = reteta;
    newNode->next = NULL;
    newNode->prev = NULL;

    if(position == 0) {
        // Inserare la începutul listei
        insertAtB(ld, reteta);
        free(newNode);
        return;
    }

    Node* current = (*ld).head;
    int pos = 0;
    while(current && pos < position - 1) {
        current = current->next;
        pos++;
    }

    if(current == NULL) {
        // Dacă poziția este mai mare decât lungimea listei, inserez la sfârșit
        insertAtE(ld, reteta);
        free(newNode);
        return;
    }

    newNode->next = current->next;
    newNode->prev = current;

    if(current->next) {
        current->next->prev = newNode;
    } else {
        (*ld).tail = newNode; // Dacă inserez la sfârșit, actualizez tail-ul
    }

    current->next = newNode;
}

void parseListAndPrint(LD ld) {
    Node* current = ld.head;
    while(current != NULL) {
        printReteta(current->reteta);
        current = current->next;
    }
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
    parseListAndPrint(ld);

    printf("\n----------Adaugare reteta 8 in lista dubla pe pozitia 3----------\n");
    insertAtPosition(&ld, 3, initReteta(8, "Mihai Bobonete", "Dr. Ion Albescu", "asigurat", '3', 13));

    printf("\n----------Retete din lista dubla----------\n");
    parseListAndPrint(ld);

    return 0;
}