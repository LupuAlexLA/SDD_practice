#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LINESIZE 128

typedef struct Sneaker Sneaker;
typedef struct Node Node;

struct Sneaker
{
    char* name;
    float size;
    int price;
};

struct Node
{
    Sneaker sneaker;
    Node* next;
};
// basics
void printSneakerToConsole(Sneaker sneaker)
{
    printf("Name: %sSize: %.2f\nPrice: %d\n\n",
           sneaker.name, sneaker.size, sneaker.price);
}

Sneaker readSneakerFromFile(FILE* f)
{
    Sneaker sneaker;
    char buffer[LINESIZE];
    char* endptr = NULL;

    fgets(buffer, LINESIZE, f);
    sneaker.name = (char*)malloc(strlen(buffer) + 1);
    strcpy(sneaker.name, buffer);

    fgets(buffer, LINESIZE, f);
    sneaker.size = strtof(buffer, &endptr);

    fgets(buffer, LINESIZE, f);
    sneaker.price = (int)strtol(buffer, &endptr, 10);

    return sneaker;
}

void readSneakersFromFile(const char* fileName, Sneaker** sneakersArray, int* sneakersNumber)
{
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];
    char* endptr = NULL;

    fgets(buffer, LINESIZE, f);
    *sneakersNumber = (int)strtol(buffer, &endptr, 10);

    *sneakersArray = malloc((*sneakersNumber) * sizeof(Sneaker));

    for(int i = 0; i < *sneakersNumber; i++)
    {
        (*sneakersArray)[i] = readSneakerFromFile(f);
    }

    fclose(f);

}
// 1. inserare
void insertAtBeginning(Node** head, Sneaker sneaker)
{
    Node* newNode = malloc(sizeof(Node));

    newNode->sneaker = sneaker;
    newNode->next = *head;

    *head = newNode;
}

void insertAtEnd(Node** head, Sneaker sneaker)
{
    if (*head)
    {
        Node* aux = *head;

        while (aux->next)
        {
            aux = aux->next;
        }

        Node* newNode = malloc(sizeof(Node));

        newNode->sneaker = sneaker;
        newNode->next = NULL;

        aux->next = newNode;
    }
    else
    {
        insertAtBeginning(head, sneaker);
    }
}
// 2. afisare
void parseListAndPrint(Node* head)
{
    if (head)
    {
        while (head)
        {
            printSneakerToConsole(head->sneaker);
            head = head->next;
        }
    }
    else
    {
        printf("List is empty\n");
    }
}
// 3. deepCopy
Sneaker deepCopyTask(Sneaker sneaker)
{
    Sneaker copySneaker;

    copySneaker.name = malloc((strlen(sneaker.name) + 1));
    strcpy(copySneaker.name, sneaker.name);
    copySneaker.size = sneaker.size;
    copySneaker.price = sneaker.price;

    return copySneaker;
}
// 4. stergere
Sneaker deleteFromBeginning(Node** head)
{
    if (*head)
    {
        Sneaker deletedSneaker;
        Node* oldHead = *head;

        *head = (*head)->next;

        deletedSneaker = oldHead->sneaker;
        free(oldHead);

        return deletedSneaker;
    }
    else
    {
        Sneaker errorSneaker = { .name = "Error Sneaker\n", .size = 0.0f, .price = 0 };
        return errorSneaker;
    }
}

Sneaker deleteFromEnd(Node** head)
{
    if (*head == NULL)
    {
        Sneaker errorSneaker = { .name = "Error Sneaker\n", .size = 0.0f, .price = 0 };
        return errorSneaker;
    }

    if((*head)->next == NULL)
    {
        Sneaker deletedSneaker = (*head)->sneaker;
        free(*head);
        *head = NULL;
        return deletedSneaker;
    }

    Node* current = *head;
    while(current->next->next != NULL)
    {
        current = current->next;
    }

    Sneaker deletedSneaker = current->next->sneaker;
    free(current->next);
    current->next = NULL;

    return deletedSneaker;
}

int main()
{
    Node* head = NULL;
    Sneaker* sneakersArray;
    int sneakersNumber;

    // Citire sneakeri din fisier si inserare in lista
    readSneakersFromFile("sneakers.txt", &sneakersArray, &sneakersNumber);
    for(int i = 0; i < sneakersNumber; i++) {
        insertAtEnd(&head, sneakersArray[i]);
    }

    // Afisare lista
    printf("Lista initiala:\n");
    parseListAndPrint(head);

    // Adaugare sneaker la inceput
    Sneaker newSneakerAtBeginning = {"New Beginning Sneaker\n", 9.5, 120};
    insertAtBeginning(&head, newSneakerAtBeginning);
    printf("\nLista dupa adaugarea unui sneaker la inceput:\n");
    parseListAndPrint(head);

    // Adaugare sneaker la final
    Sneaker newSneakerAtEnd = {"New End Sneaker\n", 10.0, 150};
    insertAtEnd(&head, newSneakerAtEnd);
    printf("\nLista dupa adaugarea unui sneaker la final:\n");
    parseListAndPrint(head);

    // Stergere sneaker de la inceput
    Sneaker deletedSneakerFromBeginning = deleteFromBeginning(&head);
    printf("\nSneaker sters de la inceput:\n");
    printSneakerToConsole(deletedSneakerFromBeginning);

    // Stergere sneaker de la final
    Sneaker deletedSneakerFromEnd = deleteFromEnd(&head);
    printf("\nSneaker sters de la final:\n");
    printSneakerToConsole(deletedSneakerFromEnd);

    // Afisare lista finala
    printf("\nLista finala:\n");
    parseListAndPrint(head);

    // Eliberare memoria
    /*for (int i = 0; i < sneakersNumber; i++) {
        free(sneakersArray[i].name);
    }
    free(sneakersArray);

    // Eliberare lista
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->sneaker.name); // Presupunem ca fiecare sneaker are un nume alocat dinamic
        free(temp);
    }*/

    return 0;
}
