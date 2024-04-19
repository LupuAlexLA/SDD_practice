//#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct SuperCar SuperCar;
typedef struct Node Node;
typedef struct HashTable HT;

struct SuperCar {
    int id;
    char* brand;
    float engineSize;
    int price;
};

struct Node {
    SuperCar supercar;
    Node* next;
};

struct HashTable {
    int size;
    Node** lists;
};

void printSuperCarToConsole(SuperCar supercar) {
    printf("\n----------\nId: %d\nBrand: %s\nEngine: %.2f\nPrice: %d\n----------\n",
           supercar.id, supercar.brand, supercar.engineSize, supercar.price);
}

SuperCar initSuperCar(int id, char* brand, float engineSize, int price) {
    SuperCar supercar = { .id = id, .brand = brand, .engineSize = engineSize, .price = price };

    supercar.brand = malloc(strlen(brand) + 1);
    strcpy(supercar.brand, brand);

    return supercar;
}

SuperCar readSuperCarFromFile(FILE* f) {
    SuperCar supercar;
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

void readSuperCarsFromFile(const char* fileName, SuperCar** supercars, int* supercarsNumber) {
    char buffer[LINESIZE];
    FILE* f = fopen(fileName, "r");

    fgets(buffer, LINESIZE, f);
    *supercarsNumber = (int)strtol(buffer, NULL, 10);

    *supercars = malloc((*supercarsNumber) * sizeof(SuperCar));
    for (int i = 0; i < (*supercarsNumber); i++)
        (*supercars)[i] = readSuperCarFromFile(f);

    fclose(f);
}

int hashFunction(int htSize, char* key) {
    int sum = 0;

    for (int i = 0; i < strlen(key); i++)
        sum += key[i];

    return sum % htSize;
}

HT initHashTable(int size) {
    HT ht;
    ht.size = size;
    ht.lists = malloc(size * sizeof(Node*));

    for (int i = 0; i < ht.size; i++)
        ht.lists[i] = NULL;

    return ht;
}

void insertList(Node** head, SuperCar supercar) {
    Node* newNode = malloc(sizeof(Node));
    newNode->supercar = supercar;
    newNode->next = NULL;

    if (*head) {
        Node* aux = *head;
        int different = strcmp(aux->supercar.brand, supercar.brand);

        while (different && aux->next) {
            aux = aux->next;
            different = strcmp(aux->supercar.brand, supercar.brand);
        }

        if (!different) {
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

void insertHashTable(HT ht, SuperCar supercar) {
    int position = hashFunction(ht.size, supercar.brand);

    if (ht.lists[position])
        insertList(&ht.lists[position], supercar); // coliziune
    else
        insertList(&ht.lists[position], supercar); // empty bucket
}

void parseListAndPrint(Node* head) {
    if (head) {
        while (head) {
            printSuperCarToConsole(head->supercar);
            head = head->next;
        }
    }
    else
        printf("Empty bucket\n");
}

void printHashTable(HT ht) {
    for (int i = 0; i < ht.size; i++) {
        printf("\nPosition %d\n", i);
        parseListAndPrint(ht.lists[i]);
    }
}

SuperCar searchHashTable(HT ht, char* key) {
    int position = hashFunction(ht.size, key);

    if (ht.lists[position]) {
        Node* aux = ht.lists[position];

        while (aux && strcmp(key, aux->supercar.brand))
            aux = aux->next;

        if (aux) {
            printf("Found at position %d\n", position);
            return initSuperCar(aux->supercar.id, aux->supercar.brand, aux->supercar.engineSize, aux->supercar.price);
        }
        else {
            SuperCar error = { .id = 0, .brand = "???", .engineSize = 0.0f, .price = 0 };
            return error;
        }
    }
    else {
        SuperCar error = { .id = 0, .brand = "???", .engineSize = 0.0f, .price = 0 };
        return error;
    }
}

int main() {
    SuperCar* supercars;
    int superCarsNumber;

    printf("\n----------\n\nAuction house contains:\n");
    readSuperCarsFromFile("supercars.txt", &supercars, &superCarsNumber);
    for (int i = 0; i < superCarsNumber; i++)
        printSuperCarToConsole(supercars[i]);
    printf("\n--------------------\n");

    HT ht = initHashTable(superCarsNumber);
    printf("\n----------\n\nPopulate the hashtable of supercars from the auction house file!\n");
    for (int i = 0; i < superCarsNumber; i++)
        insertHashTable(ht, initSuperCar(supercars[i].id, supercars[i].brand,
                                         supercars[i].engineSize, supercars[i].price));
    printHashTable(ht);
    printf("---------------------\n");

    printf("\n----------\n\nSearching in hash table for 'Audi R8'!\n");
    printSuperCarToConsole(searchHashTable(ht, "Audi R8"));

    printf("\n----------\n\nInserting new supercar in hash table!\n");
    SuperCar newSupercar = initSuperCar(9, "Bugatti Chiron", 8.0f, 3000000);
    insertHashTable(ht, newSupercar);

    printf("---------------------\n");

    printf("\n----------\n\nPrinting again the supercars from hash table!\n");
    printHashTable(ht);

    return 0;
}