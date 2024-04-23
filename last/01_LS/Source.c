#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LINESIZE 128

typedef struct Supercar Supercar;
typedef struct Node Node;

struct Supercar {
    int id;
    char* brand;
    float engineSize;
    int price;
};

struct Node {
    Supercar supercar;
    Node* next;
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

void insertAtB(Node** head, Supercar supercar) {
    Node* newNode = malloc(sizeof(Supercar));
    newNode->supercar = supercar;
    newNode->next = *head;
    *head = newNode;
}

void insertAtE(Node** head, Supercar supercar) {
    if(*head) {
        Node* aux = *head;
        while(aux->next) {
            aux = aux->next;
        }
        Node* newNode = malloc(sizeof(Node));
        newNode->supercar = supercar;
        newNode->next = NULL;
        aux->next = newNode;
    }
    else {
        insertAtB(head, supercar);
    }
}

void parseListAndPrint(Node* head) {
    if(head) {
        while(head) {
            printSupercarToConsole(head->supercar);
            head = head->next;
        }
    }
    else {
        printf("\nEmpty list!\n");
    }
}

Supercar deleteFromB(Node** head) {
    if(*head) {
        Node* oldHead = *head;
        Supercar deleted = oldHead->supercar;
        (*head) = (*head)->next;
        free(oldHead);
        return deleted;
    }
    else {
        return initSupercar(0, "???", 0.0f, 0);
    }
}

Supercar deleteFromE(Node** head) {
    if(*head) {
        if((*head)->next) {
            Node* aux = (*head);
            while(aux->next->next)
                aux = aux->next;
            Supercar deleted = aux->next->supercar;
            free(aux->next);
            aux->next = NULL;
            return deleted;
        }
        else {
            Supercar deleted = (*head)->supercar;
            free(*head);
            *head = NULL;
            return deleted;
        }
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

    Node* head = NULL;
    for(int i = 0; i < carsNumber; i++) {
        insertAtE(&head, carsArray[i]);
    }

    printf("\n\tLS:\n");
    parseListAndPrint(head);
    printf("---------------------------------------");

    deleteFromB(&head);
    deleteFromE(&head);

    printf("\n\tLS:\n");
    parseListAndPrint(head);
    printf("---------------------------------------");
}