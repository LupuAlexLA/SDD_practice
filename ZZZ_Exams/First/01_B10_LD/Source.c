#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Localitate Localitate;
typedef struct Node Node;
typedef struct LD LD;

struct Localitate {
    char* denumire;
    char* judet;
    unsigned int populatie;
    char* tipLocalitate;
    float bugetAlocat;
};

struct Node {
    Localitate localitate;
    Node* prev;
    Node* next;
};

struct LD {
    Node* head;
    Node* tail;
};

void printLocalitate(Localitate localitate) {
    printf("\n-----\nDenumire: %s\nJudet: %s\nPopulatie: %d\nTip Localitate: %s\nBuget alocat: %.3f\n-----\n",
           localitate.denumire, localitate.judet, localitate.populatie, localitate.tipLocalitate, localitate.bugetAlocat);
}

Localitate readLocalitateFromFile(FILE* f) {
    Localitate localitate;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    localitate.denumire = malloc(strlen(buffer) + 1);
    strcpy(localitate.denumire, buffer);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    localitate.judet = malloc(strlen(buffer) + 1);
    strcpy(localitate.judet, buffer);

    fgets(buffer, LINESIZE, f);
    localitate.populatie = (int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    localitate.tipLocalitate = malloc(strlen(buffer) + 1);
    strcpy(localitate.tipLocalitate, buffer);

    fgets(buffer, LINESIZE, f);
    localitate.bugetAlocat = strtof(buffer, NULL);

    return localitate;
}

void readLocalitatiFromFile(const char* fileName, Localitate** localitati, int* nrLocalitati) {
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    (*nrLocalitati) = (int)strtol(buffer, NULL, 10);

    (*localitati) = malloc((*nrLocalitati) * sizeof(Localitate));
    for(int i = 0; i < (*nrLocalitati); i++) {
        (*localitati)[i] = readLocalitateFromFile(f);
    }
    fclose(f);
}

void insertAtB(LD* ld, Localitate localitate) {
    Node* newNode = malloc(sizeof(Node));
    newNode->localitate = localitate;
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

void insertAtE(LD* ld, Localitate localitate) {
    Node* newNode = malloc(sizeof(Node));
    newNode->localitate = localitate;
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

void parseListBToE(LD ld) {
    if(ld.head) {
        while(ld.head) {
            printLocalitate(ld.head->localitate);
            ld.head = ld.head->next;
        }
    }
    else {
        printf("\n\tEmpty list!\n");
    }
}

void parseListEToB(LD ld) {
    if(ld.tail) {
        while(ld.tail) {
            printLocalitate(ld.tail->localitate);
            ld.tail = ld.tail->prev;
        }
    }
    else {
        printf("\n\tEmpty list!\n");
    }
}

unsigned int populatieTotala(LD ld, const char* tipLocal) {
    Node* currentNode = ld.head;
    unsigned int populatie = 0;
    while(currentNode) {
        if(strcmp(currentNode->localitate.tipLocalitate, tipLocal) == 0) {
            populatie += currentNode->localitate.populatie;
        }
        currentNode = currentNode->next;
    }
    return populatie;
}

void actualizeazaPopulatieJudet(LD* ld, const char* judet, int procent) {
    Node* currentNode = (*ld).head;
    while(currentNode) {
        if(strcmp(currentNode->localitate.judet, judet) == 0) {
            currentNode->localitate.populatie += currentNode->localitate.populatie * procent/100;
        }
        currentNode = currentNode->next;
    }
}

float calcMediePopulatie(LD ld) {
    int sum = 0;
    int count = 0;
    Node* current = ld.head;
    while(current != NULL) {
        sum += current->localitate.populatie;
        count++;
        current = current->next;
    }
    if(count) {
        return (float)sum/count;
    }
    else {
        return 0;
    }
}

void eliminaNoduriSubMedie(LD* ld, float medie) {
    Node* current = (*ld).head;

    while(current != NULL) {
        Node* next = current->next;
        if(current->localitate.populatie < medie) {
            if(current->prev == NULL) {
                (*ld).head = current->next;
            }
            else {
                current->prev->next = current->next;
            }
            if(current->next != NULL) {
                current->next->prev = current->prev;
            }
            else {
                (*ld).tail = current->prev;
            }
            free(current);
        }
        current = next;
    }
}

int main() {
    Localitate* localitati;
    int nrLocalitati;

    readLocalitatiFromFile("localitati.txt", &localitati, &nrLocalitati);

    printf("\n----------Localitati din fisier----------\n");
    for(int i = 0; i < nrLocalitati; i++) {
        printLocalitate(localitati[i]);
    }

    LD ld;
    ld.head = NULL;
    ld.tail = NULL;

    for(int i = 0; i < nrLocalitati; i++) {
        insertAtE(&ld, localitati[i]);
    }

    printf("\n----------Localitati din lista dubla----------\n");
    parseListBToE(ld);

    // pana aici 22 de min
    unsigned int populatie = populatieTotala(ld, "comuna");
    printf("\n----------Populatia totala pt comuna: %d----------\n", populatie);
    // + inca 15 min pt asta (6 min eu - gresit, restu chat - corect)

    actualizeazaPopulatieJudet(&ld, "Vrancea", 10);

    printf("\n----------Localitati din lista dubla S-D----------\n");
    parseListBToE(ld);

    printf("\n----------Localitati din lista dubla D-S----------\n");
    parseListEToB(ld);
    // + inca 15 min pt asta

    float mediePopulatie = calcMediePopulatie(ld);
    printf("\n----------Medie: %.2f\n", mediePopulatie);
    eliminaNoduriSubMedie(&ld, mediePopulatie);
    printf("\n----------Localitati cu populatia sub medie sterse----------\n");

    printf("\n----------Localitati din lista dubla S-D----------\n");
    parseListBToE(ld);

    printf("\n----------Localitati din lista dubla D-S----------\n");
    parseListEToB(ld);

    // + inca 30 de min pt asta

    return 0;
}