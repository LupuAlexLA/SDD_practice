#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Garantie Garantie;
typedef struct Node Node;
typedef struct HT HT;

struct Garantie {
    char* denumireProdus;
    unsigned int nrLuniGarantie;
    unsigned char esteElectronica;
    char* service;
};

struct Node {
    Garantie garantie;
    Node* next;
};

struct HT {
    int size;
    Node** lists;
};

Garantie initGarantie(const char* denProd, unsigned int nrLuniG, unsigned char esteE, const char* service) {
    Garantie garantie;
    garantie.denumireProdus = malloc(strlen(denProd) + 1);
    strcpy(garantie.denumireProdus, denProd);
    garantie.nrLuniGarantie = nrLuniG;
    garantie.esteElectronica = esteE;
    garantie.service = malloc(strlen(service) + 1);
    strcpy(garantie.service, service);
    return garantie;
}

void printGarantie(Garantie garantie) {
    printf("\n-----\nDenumire produs: %s\nNr luni garantie: %d\nElectronica: %d\nService: %s\n-----\n",
           garantie.denumireProdus, garantie.nrLuniGarantie, garantie.esteElectronica, garantie.service);
}

Garantie readGarantieFromFile(FILE* f) {
    Garantie garantie;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    garantie.denumireProdus = malloc(strlen(buffer) + 1);
    strcpy(garantie.denumireProdus, buffer);

    fgets(buffer, LINESIZE, f);
    garantie.nrLuniGarantie = (int)strtol(buffer, NULL, 0);

    fgets(buffer, LINESIZE, f);
    garantie.esteElectronica = (char)strtol(buffer, NULL, 0);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    garantie.service = malloc(strlen(buffer) + 1);
    strcpy(garantie.service, buffer);

    return garantie;
}

void readGarantiiFromFile(const char* fileName, Garantie** garantii, int* nrGarantii) {
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    (*nrGarantii) = (int)strtol(buffer, NULL, 10);

    (*garantii) = malloc((*nrGarantii) * sizeof(Garantie));
    for(int i = 0; i < (*nrGarantii); i++) {
        (*garantii)[i] = readGarantieFromFile(f);
    }
    fclose(f);
}

HT initHT(int htSize) {
    HT ht; // uitat
    ht.size = htSize;
    ht.lists = malloc(ht.size * sizeof(Node));
    for(int i = 0; i < ht.size; i++) {
        ht.lists[i] = NULL;
    }
    return ht; // uitat; pusesem void la return type
}

int hashFunction(int htSize, char* key) {
    int sum = 0;
    for(int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % htSize;
}

void parseListAndPrint(Node* head) {
    if(head) {
        while(head != NULL) {
            printGarantie(head->garantie);
            head = head->next;
        }
    }
    else {
        printf("\n\tEmpty bucket!\n");
    }
}

void printHashtable(HT ht) {
    for(int i = 0; i < ht.size; i++) {
        printf("\n\tPosition: %d\n", i);
        parseListAndPrint(ht.lists[i]);
    }
}

void insertList(Node** head, Garantie garantie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->garantie = garantie;
    newNode->next = NULL;

    if(*head) {
        Node* aux = (*head);
        int diff = strcmp(aux->garantie.denumireProdus, garantie.denumireProdus);

        while(diff && aux->next) {
            aux = aux->next;
            diff = strcmp(aux->garantie.denumireProdus, garantie.denumireProdus);
        }

        if(!diff) {
            free(aux->garantie.service);
            free(newNode);
            aux->garantie = garantie;
        }
        else {
            aux->next = newNode;
        }
    }
    else {
        *head = newNode;
    }
}

void insertInHashtable(HT ht, Garantie garantie) {
    int position = hashFunction(ht.size, garantie.service);
    insertList(&ht.lists[position], garantie);
}

int calcGarantiiEfectuate(HT ht, char* service) {
    int count = 0;
    int position = hashFunction(ht.size, service);
    Node* current = ht.lists[position];
    while(current != NULL) {
        if(strcmp(current->garantie.service, service) == 0) {
            count++;
        }
        current = current->next;
    }
    return count;
}

void extindeGarantie(HT* ht, unsigned int nrLuniExtensie) {
    for(int i = 0; i < ht->size; i++) {
        Node* currentNode = ht->lists[i];
        while(currentNode != NULL) {
            if(currentNode->garantie.esteElectronica != 0) {
                currentNode->garantie.nrLuniGarantie += nrLuniExtensie;
            }
            currentNode = currentNode->next;
        }
    }
}

int main() {
    Garantie* garantii;
    int nrGarantii;

    readGarantiiFromFile("garantii.txt", &garantii, &nrGarantii);

    printf("\n----------Garantii din fisier----------\n");
    for(int i = 0; i < nrGarantii; i++) {
        printGarantie(garantii[i]);
    }

    // 15 min (fisieru ia mult sa l scrii)

    HT ht = initHT(nrGarantii);

    for(int i = 0; i < nrGarantii; i++) {
        insertInHashtable(ht, initGarantie(garantii[i].denumireProdus, garantii[i].nrLuniGarantie,
                                           garantii[i].esteElectronica, garantii[i].service));
    }

    printf("\n----------Garantii din hashTable----------\n");
    printHashtable(ht);

    // 44 min (groaznic)

    int nrGarantiiEfectuate = calcGarantiiEfectuate(ht, "Reprezentanta Fiat");
    printf("\nGarantii efectuate in service fiat: %d\n", nrGarantiiEfectuate);

    // 15 min (cu chat)

    extindeGarantie(&ht, 12);

    printf("\n----------Garantii din hashTable----------\n");
    printHashtable(ht);

    // 5 min (cu chat)

    return 0;
}