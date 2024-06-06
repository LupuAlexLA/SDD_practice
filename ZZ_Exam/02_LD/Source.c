#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Supercar Supercar;
typedef struct Node Node;
typedef struct LD LD;

struct Supercar {
    int id;
    char* name;
    float engineSize;
    int price;
};

struct Node {
    Supercar supercar;
    Node* next;
    Node* prev;
};

struct LD {
    Node* head;
    Node* tail;
};

Supercar initSupercar(int id, const char* name, float engineSize, int price) {
    Supercar supercar;
    supercar.id = id;
    supercar.name = malloc(strlen(name) + 1);
    strcpy(supercar.name, name);
    supercar.engineSize = engineSize;
    supercar.price = price;
    return supercar;
}

void printSupercar(Supercar supercar) {
    printf("\n-----\nId: %d\nName: %s\nEngine: %.2f\nPrice: %d\n-----\n",
           supercar.id, supercar.name, supercar.engineSize, supercar.price);
}

Supercar readSupercar(FILE* f) {
    Supercar supercar;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    supercar.id = (int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    supercar.name = malloc(strlen(buffer) + 1);
    strcpy(supercar.name, buffer);

    fgets(buffer, LINESIZE, f);
    supercar.engineSize = strtof(buffer, NULL);

    fgets(buffer, LINESIZE, f);
    supercar.price = (int)strtol(buffer, NULL, 10);

    return supercar;
}

void readSupercars(const char* fileName, Supercar** supercarsArrays, int* supercarsNo) {
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    (*supercarsNo) = (int)strtol(buffer, NULL, 10);

    (*supercarsArrays) = malloc((*supercarsNo) * sizeof(Supercar));
    for(int i = 0; i < (*supercarsNo); i++) {
        (*supercarsArrays)[i] = readSupercar(f);
    }
    fclose(f);
}

void insertAtB(LD* ld, Supercar supercar) {
// NU TRB CURRENT, DIRECT !
// + mare grija ca trb sa fol ld cu *
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

void parseListBToE(LD ld) {
    if(ld.head) {
        while(ld.head) {
            printSupercar(ld.head->supercar);
            ld.head = ld.head->next;
        }
    }
    else {
        printf("\n\tEmpty list!\n");
    }
}

Supercar deleteFromB(LD* ld) {
    if((*ld).head) {
        // iar gresesc aici
        Node* currentNode = (*ld).head;
        Supercar deletedSupercar = currentNode->supercar;

        if((*ld).head->next) {
            (*ld).head = (*ld).head->next;
            (*ld).head->prev = NULL;
            free(currentNode);
        }
        else {
            free(currentNode);
            (*ld).head = NULL;
            (*ld).tail = NULL;
        }
        return deletedSupercar;
    }
    else {
        return initSupercar(0, "???", 0.0, 0);
    }
}

Supercar deleteFromE(LD* ld) {
    if((*ld).tail) {
        Node* currentNode = (*ld).tail;
        Supercar deletedSupercar = currentNode->supercar;

        if((*ld).tail->prev) {
            (*ld).tail = (*ld).tail->prev;
            (*ld).tail->next = NULL;
            free(currentNode);
        }
        else {
            free(currentNode);
            (*ld).head = NULL;
            (*ld).tail = NULL;
        }
        return deletedSupercar;
    }
    else {
        return initSupercar(0, "???", 0.0, 0);
    }
}

int main() {
    Supercar* supercarsArrays;
    int supercarsNo;

    readSupercars("supercars.txt", &supercarsArrays, &supercarsNo);

    printf("\n-------------Supercars from file-------------\n");
    for(int i = 0; i < supercarsNo; i++) {
        printSupercar(supercarsArrays[i]);
    }

    LD ld;
    ld.head = NULL;
    ld.tail = NULL;

    for(int i = 0; i < supercarsNo; i++) {
        insertAtE(&ld, supercarsArrays[i]); // atentie ca am pus &ld.head
    }

    printf("\n-------------Supercars from double list-------------\n");
    parseListBToE(ld); // am dat ca param ld.head

    printf("\n-------------Inserting 2 supercars in double list-------------\n");
    insertAtB(&ld, initSupercar(0, "Default", 0.0, 0));
    insertAtE(&ld, initSupercar(9, "Default", 9.9, 9));

    printf("\n-------------Supercars from double list-------------\n");
    parseListBToE(ld); // am dat ca param ld.head

    printf("\n-------------Removing 2 supercars in double list-------------\n");
    printSupercar(deleteFromB(&ld));
    printSupercar(deleteFromE(&ld));

    printf("\n-------------Supercars from double list-------------\n");
    parseListBToE(ld); // am dat ca param ld.head
}