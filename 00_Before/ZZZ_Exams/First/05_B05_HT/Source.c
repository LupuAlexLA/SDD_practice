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

HT initHT(int htSize) {
    HT ht;
    ht.size = htSize;
    ht.lists = malloc(ht.size * sizeof(Node));
    for(int i = 0; i < ht.size; i++) {
        ht.lists[i] = NULL;
    }
    return ht;
}

int hashFunction(int size, char* key) {
    int sum = 0;
    for(int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % size;
}

void insertList(Node** head, Comanda comanda) {
    Node* newNode = malloc(sizeof(Comanda));
    newNode->comanda = comanda;
    newNode->next = NULL;

    if(*head) {
        Node* aux = (*head);
        int diff1 = strcmp(aux->comanda.numeClient, comanda.numeClient);
        int diff2 = strcmp(aux->comanda.dataLansareComanda, comanda.dataLansareComanda);

        while(diff1 && diff2 && aux->next) {
            aux = aux->next;
            diff1 = strcmp(aux->comanda.numeClient, comanda.numeClient);
            diff2 = strcmp(aux->comanda.dataLansareComanda, comanda.dataLansareComanda);
        }

        if(!diff1 && !diff2) {
            free(aux->comanda.dataLivrare);
            free(aux->comanda.dataLansareComanda);
            free(aux->comanda.numeClient);
            free(newNode);
            aux->comanda = comanda;
        }
        else {
            aux->next = newNode;
        }
    }
    else {
        (*head) = newNode;
    }
}

void insertHashtable(HT ht, Comanda comanda) {
    int position = hashFunction(ht.size, comanda.numeClient);
    insertList(&ht.lists[position], comanda);
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
        printf("\n\tPosition: %d", i);
        parseListAndPrint(ht.lists[i]);
    }
}

int calcNrComenzi(HT ht, char* numeClient) {
    int position = hashFunction(ht.size, numeClient);
    Node* aux = ht.lists[position];
    if(aux) {
        int count = 0;
        while(aux) {
            if(strcmp(aux->comanda.numeClient, numeClient) == 0) {
                count++;
            }
            aux = aux->next;
        }
        return count;
    }
    else {
        return 0;
    }
}

void schimbaDataLivrare(HT* ht, unsigned int idComanda, char* dataNoua) {
    for(int i = 0; i < ht->size; i++) {
        Node* currentNode = ht->lists[i];
        while(currentNode) {
            if(currentNode->comanda.idComanda == idComanda) {
                currentNode->comanda.dataLivrare = dataNoua;
            }
            currentNode = currentNode->next;
        }
    }
}

Comanda cautaComandaDupaId(HT ht, unsigned int idComanda) {
    for(int i = 0; i < ht.size; i++) {
        Node* currentNode = ht.lists[i];
        while(currentNode) {
            if(currentNode->comanda.idComanda == idComanda) {
                return currentNode->comanda;
            }
            currentNode = currentNode->next;
        }
    }
    return initComanda(0, "???", "???", "???", 0.0);
}

void freeComenzi(Comanda comanda) {
    free(comanda.dataLansareComanda);
    free(comanda.dataLivrare);
    free(comanda.numeClient);
}

void freeLists(Node* head) {
    Node* current = head;
    while(current) {
        Node* next = current->next;
        freeComenzi(current->comanda);
        free(current);
        current = next;
    }
}

void freeHashTable(HT* ht) {
    for(int i = 0; i < ht->size; i++) {
        freeLists(ht->lists[i]);
    }
    free(ht->lists);
}

int main() {
    Comanda* comenzi;
    int nrComenzi;

    readComenziFromFile("comenzi.txt", &comenzi, &nrComenzi);

    printf("\n----------Comenzi din fisier----------\n");
    for(int i = 0; i < nrComenzi; i++) {
        printComanda(comenzi[i]);
    }

    HT ht = initHT(nrComenzi);

    for(int i = 0; i < ht.size; i++) {
        insertHashtable(ht, initComanda(comenzi[i].idComanda,
                                                  comenzi[i].dataLansareComanda,
                                                  comenzi[i].dataLivrare,
                                                  comenzi[i].numeClient,
                                                  comenzi[i].suma));
    }

    printf("\n----------Comenzi din hashtable----------\n");
    printHashtable(ht);

    // 21 min dar fara partea de fisiere

    int nrComenziAcelasiClient = calcNrComenzi(ht, "Alex");
    printf("\n\tNr comenzi lansate de Alex: %d", nrComenziAcelasiClient);

    // inca 10 min

    unsigned int idComanda = 3;
    char* dataNoua = "13 07 2024";
    printf("\n\tSchimbare data livrare (13 07 2024) a comenzii cu id-ul 3\n");
    schimbaDataLivrare(&ht, idComanda, dataNoua);

    printf("\n\tComanda gasita dupa id-ul 3\n");
    printComanda(cautaComandaDupaId(ht, idComanda));

    // inca 14 min
    printf("\n----------Incep dezalocarile----------\n");

    for(int i = 0; i < nrComenzi; i++) {
        freeComenzi(comenzi[i]);
    }
    free(comenzi);

    freeHashTable(&ht);

    printf("\n----------Am terminat dezalocarile----------\n");


    printf("\n----------Comenzi din fisier----------\n");
    for(int i = 0; i < nrComenzi; i++) {
        printComanda(comenzi[i]);
    }

    printf("\n----------Comenzi din hashtable----------\n");
    printHashtable(ht);

    // inca 10 min

    return 0;
}