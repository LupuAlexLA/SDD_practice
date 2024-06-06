#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LINESIZE 128

typedef struct Supercar Supercar;
typedef struct Node Node;
typedef struct HT HT;

struct Supercar {
    int id;
    char* brand;
    float engineSize;
    int price;
};

struct Node {
    Supercar supercar;
    Node* next;
};

struct HT {
    int size;
    Node** lists;
};

void printSupercarToConsole(Supercar supercar) {
    printf("\n----------\nId: %d\nBrand: %s\nEngine: %.2f\nPrice: %d\n----------\n",
           supercar.id, supercar.brand, supercar.engineSize, supercar.price);
}

Supercar initSupercar(int id, char* brand, float engineSize, int price) {
    Supercar supercar = { .id = id, .brand = brand, .engineSize = engineSize, .price = price };

    supercar.brand = malloc(strlen(brand) + 1);
    strcpy(supercar.brand, brand);

    return supercar;
}

Supercar readSupercarFromFile(FILE* f) {
    Supercar supercar;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    supercar.id = (int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = '\0';
    supercar.brand = (char*)malloc(strlen(buffer) + 1);
    strcpy(supercar.brand, buffer);

    fgets(buffer, LINESIZE, f);
    supercar.engineSize = strtof(buffer, NULL);

    fgets(buffer, LINESIZE, f);
    supercar.price = (int)strtol(buffer, NULL, 10);

    return supercar;
}

void readSupercarsFromFile(const char* fileName, Supercar** carsArray, int* carsNumber) {
    char buffer[LINESIZE];
    FILE* f = fopen(fileName, "r");

    fgets(buffer, LINESIZE, f);
    *carsNumber = (int)strtol(buffer, NULL, 10);

    *carsArray = malloc((*carsNumber) * sizeof(Supercar));
    for (int i = 0; i < (*carsNumber); i++)
        (*carsArray)[i] = readSupercarFromFile(f);

    fclose(f);
}

int hashFunction(int htSize, char* key) {
    int sum = 0;
    for(int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % htSize;
}

HT initHashTable(int size) {
    HT ht;
    ht.size = size;
    ht.lists = malloc(size * sizeof(Node**));
    for(int i = 0; i < ht.size; i++) {
        ht.lists[i] = NULL;
    }
    return ht;
}

void insertList(Node** head, Supercar supercar) {
    Node* newNode = malloc(sizeof(Node));
    newNode->supercar = supercar;
    newNode->next = NULL;
    if(*head) {
        Node* aux = *head;
        int different = strcmp(supercar.brand, aux->supercar.brand);
        while(different && aux->next) {
            aux = aux->next;
            different = strcmp(supercar.brand, aux->supercar.brand);
        }
        if(!different) {
            free(aux->supercar.brand);
            free(newNode);
            aux->supercar = supercar;
        }
        else {
            aux->next = newNode;
        }
    }
    else {
        *head = newNode;
    }
}

void insertInHashTable(HT ht, Supercar supercar) {
    int position = hashFunction(ht.size, supercar.brand);
    insertList(&ht.lists[position], supercar);
}

void parseListAndPrint(Node* head) {
    if(head) {
        while(head) {
            printSupercarToConsole(head->supercar);
            head = head->next;
        }
    }
    else {
        printf("\nEmpty bucket\n");
    }
}

void printHT(HT ht) {
    for(int i = 0; i < ht.size; i++) {
        printf("\nPosition %d\n", i);
        parseListAndPrint(ht.lists[i]);
    }
}

Supercar searchHashTable(HT ht, char* key) {
    int position = hashFunction(ht.size, key);

    if(ht.lists[position]) {
        Node* aux = ht.lists[position];
        while(aux && strcmp(key, aux->supercar.brand)) {
            aux = aux->next;
        }

        if(aux) {
            printf("\nFound at position %d\n", position);
            return initSupercar(aux->supercar.id, aux->supercar.brand, aux->supercar.engineSize, aux->supercar.price);
        }
        else {
            printf("\nItem not found!\n");
            return initSupercar(0, "???", 0.0f, 0);
        }
    }
    else {
        return initSupercar(0, "???", 0.0f, 0);
    }
}

int main() {
    Supercar* carsArray;
    int carsNumber;

    printf("\n\tFILE:\n");
    readSupercarsFromFile("supercars.txt", &carsArray, &carsNumber);
    for(int i = 0; i < carsNumber; i++)
        printSupercarToConsole(carsArray[i]);

    printf("---------------------------------------");

    HT ht = initHashTable(carsNumber);
    for(int i = 0; i < ht.size; i++)
        insertInHashTable(ht, initSupercar(carsArray[i].id, carsArray[i].brand, carsArray->engineSize, carsArray[i].price));

    printf("\n\tHT:\n");
    printHT(ht);

    printf("---------------------------------------");
    printSupercarToConsole(searchHashTable(ht, "Audi R8"));
    printf("---------------------------------------");

    insertInHashTable(ht, initSupercar(9, "mine", 6.3, 10000));
    printf("\n\tHT:\n");
    printHT(ht);
}