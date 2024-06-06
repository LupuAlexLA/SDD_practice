#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Comanda Comanda;
typedef struct Node Node;
typedef struct HT HT;

struct Comanda {
    unsigned int idComanda;
    char* dataLansareComanda;
    char* dataLivrare;
    char* numeClient;
    float suma;
};

struct Node {
    Comanda comanda;
    Node* next;
};

struct HT {
    int size;
    Node** lists;
};

Comanda initComanda(unsigned int idComanda, char* dataLansareComanda, char* dataLivrare, char* numeClient, float suma) {
    Comanda comanda;
    comanda.idComanda = idComanda;
    comanda.dataLansareComanda = malloc(strlen(dataLansareComanda) + 1);
    strcpy(comanda.dataLansareComanda, dataLansareComanda);
    comanda.dataLivrare = malloc(strlen(dataLivrare) + 1);
    strcpy(comanda.dataLivrare, dataLivrare);
    comanda.numeClient = malloc(strlen(numeClient) + 1);
    strcpy(comanda.numeClient, numeClient);
    comanda.suma = suma;
    return comanda;
}

void printComanda(Comanda comanda) {
    printf("\n-----\nId: %d\nData Lansare: %s\nData Livrare: %s\nNume Client: %s\nSuma: %.2f\n-----\n",
           comanda.idComanda, comanda.dataLansareComanda, comanda.dataLivrare, comanda.numeClient, comanda.suma);
}

Comanda readComanda(FILE* f) {
    Comanda comanda;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    comanda.idComanda = (int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    comanda.dataLansareComanda = malloc(strlen(buffer) + 1);
    strcpy(comanda.dataLansareComanda, buffer);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    comanda.dataLivrare = malloc(strlen(buffer) + 1);
    strcpy(comanda.dataLivrare, buffer);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    comanda.numeClient = malloc(strlen(buffer) + 1);
    strcpy(comanda.numeClient, buffer);

    fgets(buffer, LINESIZE, f);
    comanda.suma = strtof(buffer, NULL);

    return comanda;
}

void readComenziFromFile(const char* fileName, Comanda** comenzi, int* nrComenzi) {
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    (*nrComenzi) = (int)strtol(buffer, NULL, 10);

    (*comenzi) = malloc((*nrComenzi) * sizeof(Comanda));
    for(int i = 0; i < (*nrComenzi); i++) {
        (*comenzi)[i] = readComanda(f);
    }
    fclose(f);
}

int hashFunction(int htSize, char* key) {
    int sum = 0;
    for(int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % htSize;
}

HT initHT(int size) {
    HT ht;
    ht.size = size;
    ht.lists = malloc(size * sizeof(Node));
    for(int i = 0; i < ht.size; i++) {
        ht.lists[i] = NULL;
    }
    return ht;
}

void parseListAndPrint(Node* head) {
    if(head) {
        while(head) {
            printComanda(head->comanda);
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

void insertInList(Node** head, Comanda comanda) {
    Node* newNode = malloc(sizeof(Node));
    newNode->comanda = comanda;
    newNode->next = NULL;

    if(*head) {
        Node* aux = (*head);
        int diff = strcmp(comanda.dataLansareComanda, aux->comanda.dataLansareComanda);

        while(diff && aux->next) {
            aux = aux->next;
            diff = strcmp(comanda.dataLansareComanda, aux->comanda.dataLansareComanda);
        }

        if(!diff) {
            free(aux);
            free(newNode);
            aux->comanda = comanda;
        }
        else {
            aux->next = newNode;
        }
    }
    else {
        *head = newNode;
    }
}

void insertInHashtable(HT* ht, Comanda comanda) {
    int position = hashFunction(ht->size, comanda.numeClient);
    insertInList(&ht->lists[position], comanda);
}

void schimbaDataLivrare(HT* ht, unsigned int idComanda, const char* dataNoua) {
    for(int i = 0; i < ht->size; i++) {
        Node* current = ht->lists[i];
        while(current) {
            if(current->comanda.idComanda == idComanda) {
                free(current->comanda.dataLivrare);
                current->comanda.dataLivrare = malloc(strlen(dataNoua) + 1);
                strcpy(current->comanda.dataLivrare, dataNoua);
                return;
            }
            current = current->next;
        }
    }
}

Comanda searchHashTable(HT ht, unsigned int idComanda) {
    for(int i = 0; i < ht.size; i++) {
        Node* aux = ht.lists[i];
        while(aux) {
            if(aux->comanda.idComanda == idComanda) {
                return initComanda(aux->comanda.idComanda,
                                   aux->comanda.dataLivrare,
                                   aux->comanda.dataLansareComanda,
                                   aux->comanda.numeClient,
                                   aux->comanda.suma);
            }
            aux = aux->next;
        }
    }
    return initComanda(0, "???", "???", "???", 0.0);
}

int main() {
    Comanda* comenzi;
    int nrComenzi;

    readComenziFromFile("comenzi.txt", &comenzi, &nrComenzi);

    printf("\n----------Comenzi din fisier----------\n");
    for(int i = 0; i < nrComenzi; i++) {
        printComanda(comenzi[i]);
    }

    // 12 min

    HT ht = initHT(nrComenzi);

    for(int i = 0; i < nrComenzi; i++) {
        insertInHashtable(&ht, initComanda(comenzi[i].idComanda,
                                           comenzi[i].dataLansareComanda,
                                           comenzi[i].dataLivrare,
                                           comenzi[i].numeClient,
                                           comenzi[i].suma));
    }

    printHashtable(ht);

    // 34 min

    schimbaDataLivrare(&ht, 3, "13 06 2024");

    printHashtable(ht);

    unsigned int comandaId = 3;
    printf("\n----------Cautare comanda in ht (id=3)----------\n");
    printComanda(searchHashTable(ht, comandaId));

    // 56 min (cu chatu)

    return 0;
}