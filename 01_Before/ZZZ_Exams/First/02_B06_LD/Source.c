#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct CursaAeriana CursaAeriana;
typedef struct Node Node;
typedef struct LD LD;

struct CursaAeriana {
    unsigned int idCursa;
    char* destinatie;
    char* dataPlecare;
    unsigned short int nrIntarziere;
    unsigned char nrPasageriInregistrati;
};

struct Node {
    CursaAeriana CursaAeriana;
    Node* prev;
    Node* next;
};

struct LD {
    Node* head;
    Node* tail;
};

void printCursa(CursaAeriana cursaAeriana) {
    printf("\n-----\nId: %d\nDestinatie: %s\nData: %s\nNumar minute intarziere: %d\nNumar pasageri inregistrati: %d\n-----\n",
           cursaAeriana.idCursa, cursaAeriana.destinatie, cursaAeriana.dataPlecare, cursaAeriana.nrIntarziere, cursaAeriana.nrPasageriInregistrati);
}

CursaAeriana initCursa(unsigned int id, const char* dest, const char* data, unsigned short int nrInt, unsigned char nrPasag) {
    CursaAeriana cursaAeriana;
    cursaAeriana.idCursa = id;
    cursaAeriana.destinatie = malloc(strlen(dest) + 1);
    strcpy(cursaAeriana.destinatie, dest);
    cursaAeriana.dataPlecare = malloc(strlen(data) + 1);
    strcpy(cursaAeriana.dataPlecare, data);
    cursaAeriana.nrIntarziere = nrInt;
    cursaAeriana.nrPasageriInregistrati = nrPasag;
    return cursaAeriana;
}

CursaAeriana readCursaFromFile(FILE* f) {
    CursaAeriana cursaAeriana;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    cursaAeriana.idCursa = (int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    cursaAeriana.destinatie = malloc(strlen(buffer) + 1);
    strcpy(cursaAeriana.destinatie, buffer);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    cursaAeriana.dataPlecare = malloc(strlen(buffer) + 1);
    strcpy(cursaAeriana.dataPlecare, buffer);

    fgets(buffer, LINESIZE, f);
    cursaAeriana.nrIntarziere = (unsigned short int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    cursaAeriana.nrPasageriInregistrati = (char)strtol(buffer, NULL, 10);

    return cursaAeriana;
}

void readCurseFromFile(const char* fileName, CursaAeriana** curse, int* nrCurse) {
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    (*nrCurse) = (int)strtol(buffer, NULL, 10);

    (*curse) = malloc((*nrCurse) * sizeof(CursaAeriana));
    for(int i = 0; i < (*nrCurse); i++) {
        (*curse)[i] = readCursaFromFile(f);
    }
    fclose(f);
}

void insertAtB(LD* ld, CursaAeriana cursaAeriana) {
    Node* newNode = malloc(sizeof(Node));
    newNode->CursaAeriana = cursaAeriana;
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

void insertAtE(LD* ld, CursaAeriana cursaAeriana) {
    Node* newNode = malloc(sizeof(Node));
    newNode->CursaAeriana = cursaAeriana;
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

void parseListAndPrintBToE(LD ld) {
    if(ld.head) {
        while(ld.head) {
            printCursa(ld.head->CursaAeriana);
            ld.head = ld.head->next;
        }
    }
    else {
        printf("\n\tEmpty list!\n");
    }
}

void parseListAndPrintEToB(LD ld) {
    if(ld.tail) {
        while(ld.tail) {
            printCursa(ld.tail->CursaAeriana);
            ld.tail = ld.tail->prev;
        }
    }
    else {
        printf("\n\tEmpty list!\n");
    }
}

unsigned short int calcNrTotalMinuteIntarziere(LD* ld, const char* dest) {
    Node* currentNode = (*ld).head;
    unsigned short int total = 0;
    while(currentNode != NULL) {
        if(strcmp(currentNode->CursaAeriana.destinatie, dest) == 0) {
            total += currentNode->CursaAeriana.nrIntarziere;
        }
        currentNode = currentNode->next;
    }
    return total;
}

float calcMedNrPasageri(LD* ld, const char* dest) {
    Node* currentNode = (*ld).head;
    int sum = 0;
    int count = 0;
    while(currentNode != NULL) {
        if(strcmp(currentNode->CursaAeriana.destinatie, dest) == 0) {
            sum += currentNode->CursaAeriana.nrPasageriInregistrati;
            count++;
        }
        currentNode = currentNode->next;
    }
    if(count > 0) {
        return sum / count;
    }
    else {
        return 0;
    }
}

void stergeNoduriCuData(LD* ld, const char* data) {
    Node* current = (*ld).head;
    while(current != NULL) {
        Node *next = current->next;
        if(strcmp(current->CursaAeriana.dataPlecare, data) == 0) {
            if (current->prev == NULL) {
                (*ld).head = current->next; //next->prev = NULL;
            } else {
                current->prev->next = next;
            }
            if (next == NULL) {
                (*ld).tail = current->prev;
            } else {
                next->prev = current->prev;
            }
            free(current);
        }
        // nu stiu sa fac conditia de next
        current = next;
    }
}

int main() {
    CursaAeriana* curse;
    int nrCurse;

    readCurseFromFile("curse.txt", &curse, &nrCurse);

    printf("\n----------Curse din fisier----------\n");
    for(int i = 0; i < nrCurse; i++) {
        printCursa(curse[i]);
    }

    // 17 min

    LD ld;
    ld.head = NULL;
    ld.tail = NULL;

    for(int i = 0; i < nrCurse; i++) {
        insertAtE(&ld, curse[i]);
    }

    printf("\n----------Curse din lista dubla S-D----------\n");
    parseListAndPrintBToE(ld);

    printf("\n----------Curse din lista dubla D-S----------\n");
    parseListAndPrintEToB(ld);

    // 24 min

    unsigned short int nrTotMin = calcNrTotalMinuteIntarziere(&ld, "USA");
    printf("\n\tNr total minute pentru USA: %d\n", nrTotMin);

    // 29 min

    float medieNrPasageriPerDest = calcMedNrPasageri(&ld, "USA");
    printf("\n\tMedie nr pasageri pentru USA: %.2f\n", medieNrPasageriPerDest);

    // 34 min

    printf("\n----------Se vor sterge Cursele din lista dubla cu data 9 06 2024----------\n");
    stergeNoduriCuData(&ld, "9 06 2024");

    printf("\n----------Curse din lista dubla S-D----------\n");
    parseListAndPrintBToE(ld);

    printf("\n----------Curse din lista dubla D-S----------\n");
    parseListAndPrintEToB(ld);

    // 45 min
}