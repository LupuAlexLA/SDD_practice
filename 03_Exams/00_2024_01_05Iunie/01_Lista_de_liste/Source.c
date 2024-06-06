#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Programare Programare;
typedef struct Node Node;
typedef struct LL LL;

struct Programare {
    char* dataProgramare;
    char* numeClient;
    float durataInOre;
    char* serviciiEfectuate;
    int costProgramare;
};

struct Node {
    Programare programare;
    Node* next;
};

struct LL {
    Node* info;
    LL* next;
};

void printProgramare(Programare programare) {
    printf("\n-----\nData: %s\nClient: %s\nDurata: %.2f\nServicii : %s\nCost: %d\n-----\n",
           programare.dataProgramare, programare.numeClient, programare.durataInOre,
           programare.serviciiEfectuate, programare.costProgramare);
}

Programare readProgramare(FILE* f) {
    Programare programare;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    programare.dataProgramare = malloc(strlen(buffer) + 1);
    strcpy(programare.dataProgramare, buffer);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    programare.numeClient = malloc(strlen(buffer) + 1);
    strcpy(programare.numeClient, buffer);

    fgets(buffer, LINESIZE, f);
    programare.durataInOre = strtof(buffer, NULL);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    programare.serviciiEfectuate = malloc(strlen(buffer) + 1);
    strcpy(programare.serviciiEfectuate, buffer);

    fgets(buffer, LINESIZE, f);
    programare.costProgramare = (int)strtol(buffer, NULL, 10);

    return programare;
}

void readProgramari(char* fileName, Programare** programari, int* nrProgramari) {
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    (*nrProgramari) = (int)strtol(buffer, NULL, 10);

    (*programari) = malloc((*nrProgramari) * sizeof(Programare));
    for(int i = 0; i < (*nrProgramari); i++) {
        (*programari)[i] = readProgramare(f);
    }

    fclose(f);
}

int main() {
    Programare* programari;
    int nrProgramari;

    readProgramari("programari.txt", &programari, &nrProgramari);

    printf("\n----------Programari din fisier----------\n");
    for(int i = 0; i < nrProgramari; i++) {
        printProgramare(programari[i]);
    }
}