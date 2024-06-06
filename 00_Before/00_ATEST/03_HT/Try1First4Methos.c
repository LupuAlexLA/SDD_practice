#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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

void printSneakerToConsole(Sneaker sneaker) {}

Sneaker initSneaker(int id, char* name, float size, int price) {}

Sneaker readSneakerFromFile(FILE* f) {}

void readSneakersFromFile(const char* fileName, Sneaker** sneakers, int* sneakersNo) {}

int hashFunction(int htSize, char* key) {
    int sum = 0;
    for(int i = 0; i < strlen(key); i++)
        sum += key[i];
    return sum % htSize;
}

HT initHashTable(int size) {
    HT ht;
    ht.size = size;
    ht.lists = malloc(size * sizeof(Node*)); // am uitat sa pun linia asta
    for(int i = 0; i < ht.size; i++)
        ht.lists[i] = NULL;
    return ht;
}

void insertList(Node** head, Sneaker sneaker) {
    Node* newNode = malloc(sizeof(Node));
    newNode->sneaker = sneaker;
    newNode->next = NULL;

    if(*head != NULL) {
        Node* aux = *head;
        int diff = strcmp(aux->sneaker.name, sneaker.name);

        while(diff != 0 && aux->next != NULL) {
            aux = aux->next;
            diff = strcmp(aux->sneaker.name, sneaker.name);
        }

        if(diff == 0) {
            free(aux->sneaker.name);
            free(newNode);
            aux->sneaker = sneaker;
            // aici am crezut ca mai urmeaza ceva
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
        printf("\n\tPosition %d\n", i);
        parseListAndPrint(ht.lists[i]);
    }
}

Sneaker searchHashTable(HT ht, char* key) {
    int position = hashFunction(ht.size, key);

    Node* aux = ht.lists[position];

    while(aux != NULL && strcmp(aux->sneaker.name, key) != 0)
        aux = aux->next;

    if(aux != NULL) {
        printf("\n\tFound at position %d!\n", position);
        return initSneaker(aux->sneaker.id, aux->sneaker.name, aux->sneaker.size, aux->sneaker.price);
    }
    else {
        Sneaker error = initSneaker(0, "???", 0.0f, 0);
        return error;
    }
}

int main() {

}