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

int hashFunction(int htSize, char* key) {}

HT initHashTable(int size) {}

void insertList(Node** head, Sneaker sneaker) {}

void insertHashTable(HT ht, Sneaker sneaker) {}

void parseListAndPrint(Node** head) {}

void printHashTable(HT ht) {}

Sneaker searchHashTable(HT ht, char* key) {}

int main() {

}