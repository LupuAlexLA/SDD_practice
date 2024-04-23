#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LINESIZE 128

typedef struct Sneaker Sneaker;
typedef struct Node Node;
typedef struct HT HT;

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

struct HT {
    int size;
    Node** lists;
};

void printSneakerToConsole(Sneaker sneaker) {
    printf("\n-----\nId: %d\nName: %sSize: %.2f\nPrice: %d",
           sneaker.id, sneaker.name, sneaker.size, sneaker.price);
}

Sneaker initSneaker(int id, char* name, float size, int price) {
    Sneaker sneaker = { .id = id, .name = name, .size = size, .price = price };
    sneaker.name = malloc(strlen(name) + 1);
    strcpy(sneaker.name, name);
    return sneaker;
}

Sneaker readSneakerFromFile(FILE* f) {
    Sneaker sneaker;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    sneaker.id = (int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    sneaker.name = malloc(strlen(buffer) + 1);
    strcpy(sneaker.name, buffer);

    fgets(buffer, LINESIZE, f);
    sneaker.size = strtof(buffer, NULL);

    fgets(buffer, LINESIZE, f);
    sneaker.price = (int) strtol(buffer, NULL, 10);

    return sneaker;
}

void readSneakersFromFile(const char* fileName, Sneaker** sneakers, int* sneakersNo) {
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    *sneakersNo = (int)strtol(buffer, NULL, 10);

    *sneakers = malloc((*sneakersNo) * sizeof(Sneaker));
    for(int i = 0; i < (*sneakersNo); i++)
        (*sneakers)[i] = readSneakerFromFile(f);

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
    ht.lists = malloc( size * sizeof(Node*));
    for(int i = 0; i < ht.size; i++)
        ht.lists[i] = NULL;
    return ht;
}

void insertList(Node** head, Sneaker sneaker) {
    Node* newNode = malloc(sizeof(Node));
    newNode->sneaker = sneaker;
    newNode->next = NULL;

    if(*head) {
        Node* aux = *head;
        int diff = strcmp(aux->sneaker.name, sneaker.name);

        while(diff && aux->next) {
            aux = aux->next;
            diff = strcmp(aux->sneaker.name, sneaker.name);
        }

        if(!diff) {
            free(aux->sneaker.name);
            free(newNode);
            aux->sneaker = sneaker;
        }
        else {
            aux->next = newNode;
        }
    }
    else {
        *head = newNode;
    }
}

void insertHashTable(HT ht, Sneaker sneaker) {
    int position = hashFunction(ht.size, sneaker.name);
    insertList(&ht.lists[position], sneaker);
}

void parseListAndPrint(Node* head) {
    if(head != NULL) {
        while(head != NULL) {
            printSneakerToConsole(head->sneaker);
            head = head->next;
        }
    }
    else {
        printf("\n\tEmpty bucket!\n");
    }
}

void printHashTable(HT ht) {
    for(int i = 0; i < ht.size; i++) {
        printf("\n\tPosition: %d\n", i);
        parseListAndPrint(ht.lists[i]);
    }
}

Sneaker searchHashTable(HT ht, char* key) {
    int position = hashFunction(ht.size, key);

    if(ht.lists[position] != NULL) {
        Node* aux = ht.lists[position];

        while(aux != NULL && strcmp(aux->sneaker.name, key) != 0)
            aux = aux->next;

        if(aux != NULL) {
            printf("\n\tFound at position: %d\n", position);
            return initSneaker(aux->sneaker.id, aux->sneaker.name, aux->sneaker.size, aux->sneaker.price);
        }
        else {
            Sneaker error = initSneaker(0, "???", 0.0f, 0);
            return error;
        }
    }
    else {
        Sneaker error = initSneaker(0, "???", 0.0f, 0);
        return error;
    }
}

int main() {
    Sneaker* sneakers;
    int sneakersNo;

    readSneakersFromFile("sneakers.txt", &sneakers, &sneakersNo);
    for(int i = 0; i < sneakersNo; i++)
        printSneakerToConsole(sneakers[i]);

    HT ht = initHashTable(sneakersNo);
    for(int i = 0 ; i < sneakersNo; i++)
        insertHashTable(ht, initSneaker(sneakers[i].id, sneakers[i].name, sneakers[i].size, sneakers[i].price));
    printf("##########\nPrinting the sneakers from ther hash table!\n##########\n");
    printHashTable(ht);
    printf("----------\n");
    printSneakerToConsole(searchHashTable(ht, "Nike Runner"));
}