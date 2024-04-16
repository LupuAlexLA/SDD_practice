#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LINESIZE 128

typedef struct Sneaker Sneaker;
typedef struct Node Node;
typedef struct HashTable HT;

struct Sneaker {
    int id;
    char *name;
    float size;
    int price;
};

struct Node {
    Sneaker sneaker;
    Node *next;
};

struct HashTable {
    int size;
    Node **lists; // vector de liste
};

void printSneakerToConsole(Sneaker sneaker) {
    printf("Id: %d\nName: %s\nSize: %.2f\nPrice: %d\n\n",
           sneaker.id, sneaker.name, sneaker.size, sneaker.price);
}

Sneaker initSneaker(int id, char *name, float size, int price) {
    Sneaker sneaker = {.id = id, .name = name, .size = size, .price = price};
    sneaker.name = malloc(strlen(name) + 1);
    strcpy(sneaker.name, name);
    return sneaker;
}

Sneaker readSneakerFromFile(FILE *f) {
    char buffer[LINESIZE];
    Sneaker sneaker;

    fgets(buffer, LINESIZE, f);
    sneaker.id = (int) strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = '\0';
    sneaker.name = (char *) malloc(strlen(buffer) + 1);
    strcpy(sneaker.name, buffer);

    fgets(buffer, LINESIZE, f);
    sneaker.size = strtof(buffer, NULL);

    fgets(buffer, LINESIZE, f);
    sneaker.price = (int) strtol(buffer, NULL, 10);

    return sneaker;
}

void readSneakersFromFile(const char *fileName, Sneaker **sneakersArray, int *sneakersNumber) {
    FILE *f = fopen(fileName, "r");
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    *sneakersNumber = (int) strtol(buffer, NULL, 10);

    *sneakersArray = malloc(*sneakersNumber * sizeof(Sneaker));

    for (int i = 0; i < *sneakersNumber; i++)
        (*sneakersArray)[i] = readSneakerFromFile(f);

    fclose(f);

    // in caz ca nu ai dimensiunea specificata la inceputul fisierului
    /*int readSneakers = 0;
    *sneakersArray = malloc(readSneakers * sizeof(Sneaker));
    while(!feof(f))
    {
        Sneaker sneaker = readSneakerFromFile(f);
        readSneakers += 1;
        *sneakersArray = realloc(*sneakersArray, readSneakers * sizeof(Sneaker));
        (*sneakersArray)[readSneakers - 1] = sneaker;
    }
    *sneakersNumber = readSneakers;*/
}

int hashFunction(int htSize, char *key) // key = description
{
    int sum = 0;
    for (int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }

    return sum % htSize;
}

HT initHashTable(int size) {
    HT ht;
    ht.size = size;
    ht.lists = malloc(size * sizeof(Node *)); // vector de liste
    for (int i = 0; i < ht.size; i++)
        ht.lists[i] = NULL;

    return ht;
}

void insertList(Node **head, Sneaker sneaker) // la final
{
    Node *newNode = malloc(sizeof(Node));
    newNode->sneaker = sneaker;
    newNode->next = NULL;

    if (*head) {
        Node *aux = *head;
        // la fiecare nod parcurs verific daca e identic cu cel pe care vreau sa l introduc
        int different = strcmp(aux->sneaker.name, sneaker.name);

        while (different && aux->next) {
            aux = aux->next;
            different = strcmp(aux->sneaker.name, sneaker.name);
        }

        if (!different) {
            // pe nodul in care s-a oprit while-ul
            free(aux->sneaker.name);
            free(newNode);
            aux->sneaker = sneaker;
        } else {
            aux->next = newNode;
        }
    } else {
        *head = newNode;
    }
}

void insertHashTable(HT ht, Sneaker sneaker) {
    int position = hashFunction(ht.size, sneaker.name);

    if (ht.lists[position]) // facut doar pt a detecta coliziuni
    {
        //coliziune detectata
        insertList(&ht.lists[position], sneaker);
    } else {
        // empty bucket - introducem elem in lista
        insertList(&ht.lists[position], sneaker);
    }
}

void parseListAndPrint(Node *head) {
    if (head) {
        while (head) {
            //printf("\t");
            printSneakerToConsole(head->sneaker);
            head = head->next;
        }
    } else {
        printf("\tEmpty bucket\n");
    }
}

void printHashTable(HT ht) {
    for (int i = 0; i < ht.size; i++) {
        printf("Position %d\n", i);
        parseListAndPrint(ht.lists[i]);
    }
}

Sneaker searchHashTable(HT ht, char *key) {
    int position = hashFunction(ht.size, key);

    if (ht.lists[position]) // nu are sens if-ul si else-ul (e redundant)
    {
        // daca lista e populata parcurgem pana gasim elem (daca il gasim)
        Node *aux = ht.lists[position]; // primul elem din lista

        while (aux && strcmp(key, aux->sneaker.name))
            aux = aux->next;

        if (aux) {
            printf("Found at position %d\n", position);
            return initSneaker(aux->sneaker.id, aux->sneaker.name, aux->sneaker.size, aux->sneaker.price);
        } else {
            Sneaker error = {.id = 0, .name = "???", .size = 0.0f, .price = 0};
            return error;
        }
    } else {
        Sneaker error = {.id = 0, .name = "???", .size = 0.0f, .price = 0};
        return error;
    }
}

int main() {

    int sneakerNumber;
    Sneaker *sneakersArray;

    readSneakersFromFile("sneakers.txt", &sneakersArray, &sneakerNumber);

    printf("##########\nPrinting the sneakers from file!\n##########\n");
    for (int i = 0; i < sneakerNumber; i++)
        printSneakerToConsole(sneakersArray[i]);

    printf("---------------------\n");

    HT ht = initHashTable(sneakerNumber);

    for (int i = 0; i < sneakerNumber; i++)
        insertHashTable(ht, initSneaker(sneakersArray[i].id, sneakersArray[i].name, sneakersArray[i].size,
                                        sneakersArray[i].price));

    printf("##########\nPrinting the sneakers from hash table!\n##########\n");
    printHashTable(ht);

    printf("---------------------\n");

    // incalca DRY
    /*//insertHashTable(ht, tasksArray[0]); //-> shallow copy
    insertHashTable(ht, initSneaker(8, "Converse", 42.50f, 550));
    insertHashTable(ht, initSneaker(sneakersArray[0].id, sneakersArray[0].name, sneakersArray[0].size, sneakersArray[0].price));
    insertHashTable(ht, initSneaker(sneakersArray[1].id, sneakersArray[1].name, sneakersArray[1].size, sneakersArray[1].price));
    insertHashTable(ht, initSneaker(sneakersArray[2].id, sneakersArray[2].name, sneakersArray[2].size, sneakersArray[2].price));
    insertHashTable(ht, initSneaker(sneakersArray[3].id, sneakersArray[3].name, sneakersArray[3].size, sneakersArray[3].price));
    insertHashTable(ht, initSneaker(sneakersArray[4].id, sneakersArray[4].name, sneakersArray[4].size, sneakersArray[4].price));
    insertHashTable(ht, initSneaker(sneakersArray[5].id, sneakersArray[5].name, sneakersArray[5].size, sneakersArray[5].price));
    insertHashTable(ht, initSneaker(sneakersArray[6].id, sneakersArray[6].name, sneakersArray[6].size, sneakersArray[6].price));
    insertHashTable(ht, initSneaker(sneakersArray[7].id, sneakersArray[7].name, sneakersArray[7].size, sneakersArray[7].price));*/

    printf("##########\nSearching in hash table for 'Nike Runner'!\n##########\n");
    printSneakerToConsole(searchHashTable(ht, "Nike Runner"));

    printf("##########\nInserting new sneaker in hash table!\n##########\n");
    Sneaker newSneaker = initSneaker(9, "diablo", 42.5f, 1500);
    insertHashTable(ht, newSneaker);

    printf("---------------------\n");

    printf("##########\nPrinting again the sneakers from hash table!\n##########\n");
    printHashTable(ht);

    return 0;
}

