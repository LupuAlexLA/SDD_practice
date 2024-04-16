#include<stdio.h>
#include<string.h>
//#include<malloc.h>
#include<stdlib.h>

#define LINESIZE 128
typedef struct Sneaker Sneaker;
typedef struct Node Node;

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

void printSneakerToConsole(Sneaker sneaker) {
    printf("Id: %d\nName: %sSize: %.2f\nPrice: %d\n\n",
           sneaker.id, sneaker.name, sneaker.size, sneaker.price);
}

Sneaker initSneaker(int id, char *name, float size, int price) {

    Sneaker newSneaker = {.id = id, .name = name, .size = size, .price = price};

    newSneaker.name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newSneaker.name, name);

    return newSneaker;
}

Sneaker readSneakerFromFile(FILE *f) {
    char buffer[LINESIZE];
    Sneaker sneaker;

    fgets(buffer, LINESIZE, f);
    sneaker.id = (int) strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
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
    Sneaker sneaker;

    fgets(buffer, LINESIZE, f);
    *sneakersNumber = (int) strtol(buffer, NULL, 10);

    *sneakersArray = malloc((*sneakersNumber) * sizeof(Sneaker));

    for (int i = 0; i < *sneakersNumber; i++)
        (*sneakersArray)[i] = readSneakerFromFile(f);

    fclose(f);
}

void insertAtBeginning(Node **head, Sneaker sneaker) {
    Node *newNode = malloc(sizeof(Node));

    newNode->sneaker = sneaker;
    newNode->next = *head;

    *head = newNode;
}

void insertAtEnd(Node **head, Sneaker sneaker) {
    if (*head) {
        Node *aux = *head;
        while (aux->next)
            aux = aux->next;

        Node *newNode = malloc(sizeof(Node));

        newNode->sneaker = sneaker;
        newNode->next = NULL;

        aux->next = newNode;
    } else
        insertAtBeginning(head, sneaker);
}

Sneaker deepCopy(Sneaker sneaker) {
    Sneaker copy;

    copy.id = sneaker.id;
    copy.name = (char *) malloc(strlen(sneaker.name) + 1);
    strcpy(copy.name, sneaker.name);
    copy.size = sneaker.size;
    copy.price = sneaker.price;

    return copy;
}

void parseListAndPrint(Node *head) {
    if (head) {
        while (head) {
            printSneakerToConsole(head->sneaker);
            head = head->next;
        }
    } else
        printf("List is empty\n");
}

Sneaker deleteFromBeginning(Node **head) {
    if (*head) {
        Sneaker deletedSneaker;
        Node *oldHead = *head;

        *head = (*head)->next;

        deletedSneaker = oldHead->sneaker;
        free(oldHead);

        return deletedSneaker;
    } else {
        Sneaker errorSneaker = {.id = 0, .name = "Error Sneaker\n", .size = 0.0f, .price = 0};
        return errorSneaker;
    }
}

Sneaker deleteFromEnd(Node **head) {
    if (*head) {
        if ((*head)->next) {
            Node *aux = *head;
            // pozitionare pe penultimul nod
            while (aux->next->next)
                aux = aux->next;
            // aux = penultimul; aux->next = ultimul
            Sneaker sneaker = aux->next->sneaker;
            free(aux->next);
            aux->next = NULL;

            return sneaker;
        } else {
            Sneaker sneaker = (*head)->sneaker;
            free(*head);
            *head = NULL;
            return sneaker;
        }
    } else {
        Sneaker empty = {.id = 0, .name = "???\n", .size = 0.0f, .price = 0};
        return empty;
    }
}

// chat gpt
/*Sneaker deleteFromEnd(Node** head) {
    if (*head == NULL)
    {
        Sneaker errorSneaker = { .id = 0, .name = "Error Sneaker\n", .size = 0.0f, .price = 0 };
        return errorSneaker;
    }

    if ((*head)->next == NULL)
    {
        Sneaker deletedSneaker = (*head)->sneaker;
        free(*head);
        *head = NULL;
        return deletedSneaker;
    }

    Node* current = *head;
    while (current->next->next != NULL)
        current = current->next;

    Sneaker deletedSneaker = current->next->sneaker;
    free(current->next);
    current->next = NULL;

    return deletedSneaker;
}*/

Sneaker deleteWithCondition(Node **head, int id) {
    if (*head) {
        if ((*head)->sneaker.id == id)
            return deleteFromBeginning(head);

        if ((*head)->next) {
            Node *aux = *head;

            while (aux->next && (aux->next->sneaker.id != id))
                aux = aux->next;

            if (!aux->next) {
                printf("Item not found!\n");
                Sneaker empty = {.id = 0, .name = "???\n", .size = 0.0f, .price = 0};
                return empty;
            }

            Sneaker sneaker = aux->next->sneaker; // save sneaker
            Node *deletedNode = aux->next; // save node for later freeing
            aux->next = aux->next->next; // update link
            free(deletedNode); // free memory of deleted node

            return sneaker;
        } else {
            if ((*head)->sneaker.id == id) {
                Sneaker sneaker = (*head)->sneaker;

                free(*head);
                *head = NULL;

                return sneaker;
            } else {
                Sneaker empty = {.id = 0, .name = "???\n", .size = 0.0f, .price = 0};
                return empty;
            }
        }
    } else {
        Sneaker empty = {.id = 0, .name = "???\n", .size = 0.0f, .price = 0};
        return empty;
    }
}

int main() {
    int sneakersNumber;
    Sneaker *sneakersArray;

    readSneakersFromFile("sneakers.txt", &sneakersArray, &sneakersNumber);

    printf("##########\nPrinting the sneakers from file!\n##########\n");
    for (int i = 0; i < sneakersNumber; i++)
        printSneakerToConsole(sneakersArray[i]);

    printf("---------------------\n");

    Node *head = NULL;
    for (int i = 0; i < sneakersNumber; i++)
        insertAtEnd(&head, sneakersArray[i]);

    printf("##########\nPrinting the sneakers from simple list!\n##########\n");
    parseListAndPrint(head);

    printf("---------------------\n");

    /*insertAtBeginning(&head, deepCopy(sneakersArray[4]));
    insertAtEnd(&head, sneakersArray[0]);*/
    printf("##########\nInserting sneaker at beginning: id: 0; name: first; size: 40.5; price: 500\n##########\n");
    insertAtBeginning(&head, initSneaker(0, "first\n", 40.5f, 500));
    printf("##########\nInserting sneaker at end: id: 6; name: last; size: 40.5; price: 500\n##########\n");
    insertAtEnd(&head, initSneaker(6, "last\n", 42.5f, 500));

    printf("##########\nPrinting the sneakers from simple list after inserts!\n##########\n");
    parseListAndPrint(head);
    printf("---------------------\n");

    Sneaker deletedSneakerFromBeginning = deleteFromBeginning(&head);
    printf("##########\nSneaker sters de la inceput:\n##########\n");
    printSneakerToConsole(deletedSneakerFromBeginning);
    printf("---------------------\n");

    Sneaker deletedSneakerFromEnd = deleteFromEnd(&head);
    printf("##########\nSneaker sters de la final:\n##########\n");
    printSneakerToConsole(deletedSneakerFromEnd);
    printf("---------------------\n");

    printf("##########\nPrinting the sneakers from simple list after deleting!\n##########\n");
    parseListAndPrint(head);

    printf("---------------------\n");

    printf("##########\nDeleted sneaker: \n##########\n");
    Sneaker deletedSneakerWithCondition = deleteWithCondition(&head, 3);
    printSneakerToConsole(deletedSneakerWithCondition);

    printf("---------------------\n");

    printf("##########\nPrinting the sneakers from simple list after delete!\n##########\n");
    parseListAndPrint(head);

    printf("---------------------\n");

    return 0;
}