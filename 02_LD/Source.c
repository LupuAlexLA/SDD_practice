#include<stdio.h>
#include<string.h>
//#include<malloc.h>
#include<stdlib.h>
#define LINESIZE 128
typedef struct Sneaker Sneaker;
typedef struct Node Node;
typedef struct NodeDLL NodeDLL;
typedef struct DoublyLinkedList DoublyLinkedList;

struct Sneaker {
    int id;
    char* name;
    float size;
    int price;
};

struct Node {
    Sneaker sneaker;
    Node* next;
};

struct NodeDLL {
    Sneaker sneaker;
    NodeDLL* prev;
    NodeDLL* next;
};

struct DoublyLinkedList {
    NodeDLL* head;
    NodeDLL* tail;
};

void printSneakerToConsole(Sneaker sneaker) {
    printf("Id: %d\nName: %sSize: %.2f\nPrice: %d\n\n",
           sneaker.id, sneaker.name, sneaker.size, sneaker.price);
}

Sneaker initSneaker(int id, char* name, float size, int price) {

    Sneaker newSneaker = { .id = id, .name = name, .size = size, .price = price };

    newSneaker.name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newSneaker.name, name);

    return newSneaker;
}

Sneaker readSneakerFromFile(FILE* f) {
    char buffer[LINESIZE];
    Sneaker sneaker;

    fgets(buffer, LINESIZE, f);
    sneaker.id = (int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    sneaker.name = (char*)malloc(strlen(buffer) + 1);
    strcpy(sneaker.name, buffer);

    fgets(buffer, LINESIZE, f);
    sneaker.size = strtof(buffer, NULL);

    fgets(buffer, LINESIZE, f);
    sneaker.price = (int)strtol(buffer, NULL, 10);

    return sneaker;
}

void readSneakersFromFile(const char* fileName, Sneaker** sneakersArray, int* sneakersNumber) {
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];
    Sneaker sneaker;

    fgets(buffer, LINESIZE, f);
    *sneakersNumber = (int)strtol(buffer, NULL, 10);

    *sneakersArray = malloc((*sneakersNumber) * sizeof(Sneaker));

    for (int i = 0; i < *sneakersNumber; i++)
        (*sneakersArray)[i] = readSneakerFromFile(f);

    fclose(f);
}

void insertAtBeginningDLL(DoublyLinkedList* dll, Sneaker sneaker) {
    NodeDLL* newNode = malloc(sizeof(NodeDLL));
    newNode->sneaker = sneaker;

    newNode->next = (*dll).head;
    newNode->prev = NULL;

    if ((*dll).head) {
        (*dll).head->prev = newNode;
    }
    else {
        (*dll).tail = newNode;
    }

    (*dll).head = newNode;
}

void insertAtEndDLL(DoublyLinkedList* dll, Sneaker sneaker) {
    NodeDLL* newNode = malloc(sizeof(NodeDLL));
    newNode->sneaker = sneaker;

    newNode->prev = (*dll).tail;
    newNode->next = NULL;

    if ((*dll).tail) {
        (*dll).tail->next = newNode;
    }
    else {
        (*dll).head = newNode;
    }

    (*dll).tail = newNode;
}

void parseListBtoE(DoublyLinkedList dll) {
    if (dll.head) {
        while (dll.head) {
            printSneakerToConsole(dll.head->sneaker);
            dll.head = dll.head->next;
        }
    }
    else
        printf("List is empty\n");
}

void parseListEtoB(DoublyLinkedList dll) {
    if (dll.tail) {
        while (dll.tail) {
            printSneakerToConsole(dll.tail->sneaker);
            dll.tail = dll.tail->prev;
        }
    }
    else
        printf("List is empty\n");
}

Sneaker deleteFromBeginningDLL(DoublyLinkedList* dll) {

    if ((*dll).head) {

        NodeDLL* currentNode = (*dll).head;
        Sneaker deletedSneaker = currentNode->sneaker;

        if ((*dll).head->next) {
            (*dll).head = (*dll).head->next;
            (*dll).head->prev = NULL;
            free(currentNode);

            return deletedSneaker;
        }
        else {
            free(currentNode);
            (*dll).head = NULL;
            (*dll).tail = NULL;

            return deletedSneaker;
        }
    }

    return initSneaker(0, "ERROR SNEAKER", 0.0, 0);
}

Sneaker deleteFromEndDLL(DoublyLinkedList* dll) {

    if ((*dll).tail) {

        NodeDLL* currentNode = (*dll).tail;
        Sneaker deletedSneaker = currentNode->sneaker;

        if ((*dll).tail->prev) {
            (*dll).tail = (*dll).tail->prev;
            (*dll).tail->next = NULL;
            free(currentNode);

            return deletedSneaker;
        }
        else {
            free(currentNode);
            (*dll).head = NULL;
            (*dll).tail = NULL;

            return deletedSneaker;
        }
    }

    return initSneaker(0, "ERROR SNEAKER", 0.0, 0);
}

/*int listLength(DoublyLinkedList dll) {
    int len = 0;
    while (dll.head) {
        len += 1;
        dll.head = dll.head->next;
    }
}*/

int main() {

    int sneakersNumber;
    Sneaker* sneakersArray;

    readSneakersFromFile("sneakers.txt", &sneakersArray, &sneakersNumber);

    /*for (int i = 0; i < tasksNumber; i++) {
        printTaskToConsole(tasksArray[i]);
    }*/

    DoublyLinkedList dll;

    dll.head = NULL;
    dll.tail = NULL;

    insertAtBeginningDLL(&dll, sneakersArray[0]);
    insertAtBeginningDLL(&dll, sneakersArray[1]);
    insertAtBeginningDLL(&dll, sneakersArray[1]);
    insertAtBeginningDLL(&dll, sneakersArray[2]);
    insertAtBeginningDLL(&dll, sneakersArray[3]);
    insertAtBeginningDLL(&dll, sneakersArray[4]);
    insertAtEndDLL(&dll, sneakersArray[0]);

    parseListBtoE(dll);

    printf("\n\n----\n\n");

    parseListBtoE(dll);

    printf("\n\n----\n\n");

    parseListEtoB(dll);

    Sneaker ds = deleteFromBeginningDLL(&dll);
    printSneakerToConsole(ds);

    ds = deleteFromEndDLL(&dll);

    printSneakerToConsole(ds);

    return 0;
}