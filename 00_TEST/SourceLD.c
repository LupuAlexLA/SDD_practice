//#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

typedef struct SuperCar SuperCar;
typedef struct NodeDLL NodeDLL;
typedef struct DoublyLinkedList DoublyLinkedList;

#define LINESIZE 128

struct SuperCar {
    int id;
    char* brand;
    float engineSize;
    int price;
};

struct NodeDLL {
    SuperCar supercar;
    NodeDLL* prev;
    NodeDLL* next;
};

struct DoublyLinkedList {
    NodeDLL* head;
    NodeDLL* tail;
};

void printSuperCarToConsole(SuperCar supercar) {
    printf("\n----------\nId: %d\nBrand: %sEngine: %.2f\nPrice: %d\n----------\n",
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

void insertAtBeginningDLL(DoublyLinkedList* dll, SuperCar supercar) {
    NodeDLL* newNode = malloc(sizeof(NodeDLL));
    newNode->supercar = supercar;
    newNode->next = (*dll).head;
    newNode->prev = NULL;

    if ((*dll).head) {
        (*dll).head->prev = newNode;
    } else {
        (*dll).tail = newNode;
    }
    (*dll).head = newNode;
}

void insertAtEndDLL(DoublyLinkedList* dll, SuperCar supercar) {
    NodeDLL* newNode = malloc(sizeof(NodeDLL));
    newNode->supercar = supercar;
    newNode->prev = (*dll).tail;
    newNode->next = NULL;

    if ((*dll).tail) {
        (*dll).tail->next = newNode;
    } else {
        (*dll).head = newNode;
    }
    (*dll).tail = newNode;
}

void parseListBtoE(DoublyLinkedList dll) {
    if (dll.head) {
        while (dll.head) {
            printSuperCarToConsole(dll.head->supercar);
            dll.head = dll.head->next;
        }
    } else {
        printf("List is empty!");
    }
}

void parseListEtoB(DoublyLinkedList dll) {
    if (dll.tail) {
        while (dll.tail) {
            printSuperCarToConsole(dll.tail->supercar);
            dll.tail = dll.tail->prev;
        }
    }
    else {
        printf("List is empty!");
    }
}

SuperCar deleteFromBeginningDLL(DoublyLinkedList* dll) {
    if ((*dll).head) {
        NodeDLL* currentNode = (*dll).head;
        SuperCar deletedSupercar = currentNode->supercar;

        if ((*dll).head->next) {
            (*dll).head = (*dll).head->next;
            (*dll).head->prev = NULL;
            free(currentNode);
        } else {
            free(currentNode);
            (*dll).head = NULL;
            (*dll).tail = NULL;
        }
        return deletedSupercar;
    } else {
        return initSuperCar(0, "ERROR SUPERCAR", 0.0, 0);
    }
}

SuperCar deleteFromEndDLL(DoublyLinkedList* dll) {
    if ((*dll).tail) {
        NodeDLL* currentNode = (*dll).tail;
        SuperCar deletedSupercar = currentNode->supercar;

        if ((*dll).tail->prev) {
            (*dll).tail = (*dll).tail->prev;
            (*dll).tail->next = NULL;
            free(currentNode);
        }
        else {
            free(currentNode);
            (*dll).tail = NULL;
            (*dll).head = NULL;
        }
        return deletedSupercar;
    }
    else {
        return initSuperCar(0, "ERROR SUPERCAR", 0.0, 0);
    }
}

int listLength(DoublyLinkedList dll) {
    int len = 0;
    while (dll.head) {
        len += 1;
        dll.head = dll.head->next;
    }
    return len;
}

int main() {
    SuperCar* supercars;
    int superCarsNumber;

    printf("\n----------\n\nAuction house contains:\n");
    readSuperCarsFromFile("supercars.txt", &supercars, &superCarsNumber);
    for (int i = 0; i < superCarsNumber; i++)
        printSuperCarToConsole(supercars[i]);
    printf("\n--------------------\n");

    printf("\n----------\n\nPopulate the list of supercars from the auction house file!\n");
    DoublyLinkedList dll;
    dll.head = NULL;
    dll.tail = NULL;

    for (int i = 0; i < superCarsNumber; i++)
        insertAtEndDLL(&dll, supercars[i]);
    parseListBtoE(dll);
    printf("\n--------------------\n");

    printf("\n----------\n\nBugatti Chiron is available in the parking lot at the entrance!\n");
    insertAtBeginningDLL(&dll, initSuperCar(0, "Bugatti Chiron\n", 8.0f, 3000000));
    printf("\n----------\n\nPagani Huayra is available in the parking lot at the exit!\n");
    insertAtEndDLL(&dll, initSuperCar(9, "Pagani Huayra\n", 6.0, 3300000));

    printf("\n----------\n\nAuction house contains:\n");
    parseListBtoE(dll);
    printf("\n--------------------\n");

    printf("\n----------\n\nThe next supercar was sold from the entrance!\n");
    SuperCar deletedFromBeginning = deleteFromBeginningDLL(&dll);
    printSuperCarToConsole(deletedFromBeginning);

    printf("\n----------\n\nThe next supercar was sold from the exit!\n");
    SuperCar deletedFromEnd = deleteFromEndDLL(&dll);
    printSuperCarToConsole(deletedFromEnd);

    printf("\n----------\n\nAuction house contains:\n");
    parseListBtoE(dll);
    printf("\n--------------------\n");
}