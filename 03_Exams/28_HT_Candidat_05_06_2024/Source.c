#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Candidat Candidat;
typedef struct Node Node;
typedef struct HT HT;

struct Candidat {
    int cod;
    char* nume;
    char* facultate;
    float medie;
    int salariu;
};

struct Node {
    Candidat candidat;
    Node* next;
};

struct HT {
    int size;
    Node** lists;
};

void afiseazaCandidat(Candidat candidat) {
    printf("\n-----\nCod: %d\nNume: %s\nFacultate: %s\nMedie: %.2f\nSalariu: %d\n-----\n",
           candidat.cod, candidat.nume, candidat.facultate, candidat.medie, candidat.salariu);
}

Candidat citesteCandidatDinFisier(FILE* f) {
    Candidat candidat;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    candidat.cod = (int)strtol(buffer, NULL, 10);
    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    candidat.nume = malloc(strlen(buffer) + 1);
    strcpy(candidat.nume, buffer);
    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    candidat.facultate = malloc(strlen(buffer) + 1);
    strcpy(candidat.facultate, buffer);
    fgets(buffer, LINESIZE, f);
    candidat.medie = strtof(buffer, NULL);
    fgets(buffer, LINESIZE, f);
    candidat.salariu = (int)strtol(buffer, NULL, 10);

    return candidat;
}

void citesteCandidatiDinFisier(char* numeFisier, Candidat** candidati, int* nrCandidati) {
    FILE* f = fopen(numeFisier, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    (*nrCandidati) = (int)strtol(buffer, NULL, 10);

    (*candidati) = malloc((*nrCandidati) * sizeof(Candidat));
    for (int i = 0; i < (*nrCandidati); i++) {
        (*candidati)[i] = citesteCandidatDinFisier(f);
    }

    fclose(f);
}

int hashFunction(int size, char* key) {
    int sum = 0;
    for (int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % size;
}

HT initHT(int size) {
    HT ht;
    ht.size = size;
    ht.lists = malloc(size * sizeof(Node*)); // imp *
    for (int i = 0; i < ht.size; i++) {
        ht.lists[i] = NULL;
    }
    return ht;
}

void insertInList(Node** head, Candidat candidat) {
    Node* newNode = malloc(sizeof(Node));
    newNode->candidat = candidat;
    newNode->next = NULL;

    if (*head) {
        Node* aux = *head;
        //int different = strcmp(aux->candidat.facultate, candidat.facultate);

        while (/*different &&*/ aux->next) {
            aux = aux->next;
            //different = strcmp(aux->candidat.facultate, candidat.facultate);
        }

        // nu vreau sa am cheie unica
        /*if (!different) {
            free(aux->candidat.nume);
            free(aux->candidat.facultate);
            free(newNode);
            aux->candidat = candidat;
        }
        else {*/
        aux->next = newNode;
        //}
    }
    else {
        *head = newNode;
    }
}

void insertInHT(HT* ht, Candidat candidat) {
    int position = hashFunction(ht->size, candidat.facultate);

    insertInList(&ht->lists[position], candidat);
}

void printList(Node* head) {
    if (head) {
        while (head) {
            afiseazaCandidat(head->candidat);
            head = head->next;
        }
    }
    else {
        printf("\n\tCos gol!\n");
    }
}

void printHT(HT ht) {
    for (int i = 0; i < ht.size; i++) {
        printf("\n\tPozitie: %d", i);
        printList(ht.lists[i]);
    }
}

float cautareInHTDupaMedieMaxima(HT ht, char* facultate) {
    int position = hashFunction(ht.size, facultate);

    Node* current = ht.lists[position];

    float medie = -1;

    if (ht.lists[position]) {
        while (current) {
            if (strcmp(current->candidat.facultate, facultate) == 0) {
                if (current->candidat.medie > medie) {
                    medie = current->candidat.medie;
                }
            }
            current = current->next;
        }

        return medie;
    }
    else {
        printf("\n\tNu am gasit!\n");
        return -1;
    }
}

void actualizeazaSalariuCandidat(HT* ht, int cod, int salariu)
{
    for (int i = 0; i < ht->size; i++) {
        // NU ARE SENS FOR-UL ASTA
        //for (int j = 0; j < ht->lists[i]; j++) {
        Node* aux = ht->lists[i];
        while (aux) {
            if (aux->candidat.cod == cod) {
                aux->candidat.salariu = salariu;
            }
            aux = aux->next;
        }
        //}
    }
}

void stergeCandidatCuNumele(HT* ht, char* nume) {
    for (int i = 0; i < ht->size; i++) {
        Node* current = ht->lists[i];
        Node* prev = NULL;
        while (current != NULL) {
            if (strcmp(current->candidat.nume, nume) == 0) {
                if (prev == NULL) {
                    ht->lists[i] = current->next;
                }
                else {
                    prev->next = current->next;
                }
                free(current->candidat.nume);
                free(current->candidat.facultate);
                free(current);
                return; //UITAM SA IL PUN F IMPORTANT
            }
            prev = current;
            current = current->next;
        }
    }
}

int main() {
    Candidat* candidati;
    int nrCandidati;

    citesteCandidatiDinFisier("candidati.txt", &candidati, &nrCandidati);

    printf("\n----------Candidati din fisier----------\n");
    for (int i = 0; i < nrCandidati; i++) {
        afiseazaCandidat(candidati[i]);
    }

    // 1
    HT ht = initHT(7);

    for (int i = 0; i < nrCandidati; i++) {
        insertInHT(&ht, candidati[i]);
    }

    printf("\n----------Candidati din HT----------\n");
    printHT(ht);

    // 2
    printf("\n----------Cea mai mare medie a candidatilor----------\n");
    char facultate[LINESIZE];
    printf("\nCe facultate doresti?\n");
    scanf("%s", facultate);
    float medieMaxima = cautareInHTDupaMedieMaxima(ht, facultate);
    printf("\n\tMedie gasita: %.2f", medieMaxima);

    // 3
    printf("\n----------Modificarea salariului unui candidat----------\n");
    int cod = 0, salariu = 0;
    printf("\nCe candidat doresti?\n");
    scanf("%d", &cod);
    printf("\nCe salariu doresti?\n");
    scanf("%d", &salariu);
    actualizeazaSalariuCandidat(&ht, cod, salariu);

    printf("\n----------Candidati din HT----------\n");
    printHT(ht);

    // 4
    //char nume[LINESIZE];
    // NU MERGE ASA: (NU IA SPATIUL IN CONSIDERARE)
    //printf("\nCe nume doresti?\n");
    //scanf("%s", nume);

    //printf("\nCe nume doresti?\n");
    //fgets(nume, LINESIZE, stdin);
    //nume[strcspn(nume, "\n")] = 0;
    char* nume = "Ion Barbu";
    stergeCandidatCuNumele(&ht, nume);

    printf("\n----------Candidati din HT----------\n");
    printHT(ht);
}