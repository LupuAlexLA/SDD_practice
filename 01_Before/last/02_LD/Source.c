#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LINESIZE 128

typedef struct Supercar Supercar;
typedef struct Node Node;
typedef struct LD LD;

struct Supercar {
    int id;
    char* brand;
    float engineSize;
    int price;
};

struct Node {
    Supercar supercar;
    Node* prev;
    Node* next;
};

struct LD {
    Node* head;
    Node* tail;
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

void insertAtB(LD* ld, Supercar supercar) {
    Node* newNode = malloc(sizeof(Node));
    newNode->supercar = supercar;
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

void insertAtE(LD* ld, Supercar supercar) {
    Node* newNode = malloc(sizeof(Node));
    newNode->supercar = supercar;
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

void parseListBtoE(LD ld) {
    if(ld.head) {
        while(ld.head) {
            printSupercarToConsole(ld.head->supercar);
            ld.head = ld.head->next;
        }
    }
    else {
        printf("\nList is empty!\n");
    }
}

void parseListEtoB(LD ld) {
    if(ld.head) {
        while(ld.tail) {
            printSupercarToConsole(ld.tail->supercar);
            ld.tail = ld.tail->prev;
        }
    }
    else {
        printf("\nList is empty!\n");
    }
}

Supercar deleteFromB(LD* ld) {
    if((*ld).head) {
        Node* oldHead = (*ld).head;
        Supercar deleted = oldHead->supercar;
        if((*ld).head->next) {
            (*ld).head = (*ld).head->next;
            (*ld).head->prev = NULL;
            free(oldHead);
        }
        else {
            free(oldHead);
            (*ld).head = NULL;
            (*ld).tail = NULL;
        }
        return deleted;
    }
    else {
        return initSupercar(0, "???", 0.0f, 0);
    }
}

Supercar deleteFromE(LD* ld) {
    if((*ld).tail) {
        Node* oldHead = (*ld).tail;
        Supercar deleted = oldHead->supercar;
        if((*ld).tail->prev) {
            (*ld).tail = (*ld).tail->prev;
            (*ld).tail->next = NULL;
            free(oldHead);
        }
        else {
            free(oldHead);
            (*ld).head = NULL;
            (*ld).tail = NULL;
        }
        return deleted;
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
    LD ld;
    ld.head = NULL;
    ld.tail = NULL;
    for(int i = 0; i < carsNumber; i++) {
        insertAtE(&ld, carsArray[i]);
    }

    printf("\n\tLD:\n");
    parseListBtoE(ld);
    printf("---------------------------------------");

    deleteFromB(&ld);
    deleteFromE(&ld);

    printf("\n\tLD:\n");
    parseListBtoE(ld);
    printf("---------------------------------------");
}