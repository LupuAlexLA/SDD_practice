#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

typedef struct SuperCar SuperCar;
typedef struct Node Node;

#define LINESIZE 128

struct SuperCar {
    int id;
    char* brand;
    float engineSize;
    int price;
};

struct Node {
    SuperCar supercar;
    Node* next;
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

void insertAtBeginning(Node** head, SuperCar supercar) {
    Node* newNode = malloc(sizeof(Node));

    newNode->supercar = supercar;
    newNode->next = *head;

    *head = newNode;
}

void insertAtEnd(Node** head, SuperCar supercar) {
    if (*head) {
        Node* aux = *head;

        while (aux->next)
            aux = aux->next;

        Node* newNode = malloc(sizeof(Node));

        newNode->supercar = supercar;
        newNode->next = NULL;

        aux->next = newNode;
    }
    else
        insertAtBeginning(head, supercar);
}

void parseListAndPrint(Node* head) {
    if (head) {
        while (head) {
            printSuperCarToConsole(head->supercar);
            head = head->next;
        }
    }
    else {
        printf("List is empty");
    }
}

SuperCar deleteFromBeginning(Node** head) {
    if (*head) {
        SuperCar deletedfromBeginning;
        Node* oldHead = *head;

        *head = (*head)->next;
        deletedfromBeginning = oldHead->supercar;
        free(oldHead);

        return deletedfromBeginning;
    }
    else {
        SuperCar error = { .id = 0, .brand = "???", .engineSize = 0.0f, .price = 0 };
        return error;
    }
}

SuperCar deleteFromEnd(Node** head) {
    if (*head) {
        // avem al doilea element?
        if ((*head)->next) {
            Node* aux = *head;
            // pozitionare pe penultimul elem
            while (aux->next->next)
                aux = aux->next;

            SuperCar deletedFromEnd = aux->next->supercar;
            free(aux->next);
            aux->next = NULL;
            return deletedFromEnd;
        }
            // daca nu il stergi (o lista cu un elem)
        else {
            SuperCar deletedFromEnd = (*head)->supercar;
            free(*head);
            *head = NULL;
            return deletedFromEnd;
        }
    }
    else {
        SuperCar error = { .id = 0, .brand = "???", .engineSize = 0.0f, .price = 0 };
        return error;
    }
}

SuperCar deleteWithCondition(Node** head, int id) {
    if (*head) {
        // daca e primul
        if ((*head)->supercar.id == id)
            return deleteFromBeginning(head);
        // daca lista are 2 elem
        if ((*head)->next) {
            Node* aux = *head;

            while (aux->next && (aux->next->supercar.id != id))
                aux = aux->next;

            if (!aux->next) {
                printf("Car not found!\n");
                SuperCar empty = { .id = 0, .brand = "???\n", .engineSize = 0.0f, .price = 0 };
                return empty;
            }

            SuperCar vehicle = aux->next->supercar; // save sneaker
            Node* deletedNode = aux->next; // save node for later freeing
            aux->next = aux->next->next; // update link
            free(deletedNode); // free memory of deleted node

            return vehicle;
        }
            // lista contine un singur elem
        else {
            if ((*head)->supercar.id == id) {
                SuperCar supercar = (*head)->supercar;

                free(*head);
                *head = NULL;

                return supercar;
            }
            else {
                SuperCar empty = { .id = 0, .brand = "???\n", .engineSize = 0.0f, .price = 0 };
                return empty;
            }
        }
    }
    else {
        SuperCar empty = { .id = 0, .brand = "???\n", .engineSize = 0.0f, .price = 0 };
        return empty;
    }
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
    Node* head = NULL;
    for (int i = 0; i < superCarsNumber; i++)
        insertAtEnd(&head, supercars[i]);
    parseListAndPrint(head);
    printf("\n--------------------\n");

    printf("\n----------\n\nBugatti Chiron is available in the parking lot at the entrance!\n");
    insertAtBeginning(&head, initSuperCar(0, "Bugatti Chiron\n", 8.0f, 3000000));
    printf("\n----------\n\nPagani Huayra is available in the parking lot at the exit!\n");
    insertAtEnd(&head, initSuperCar(9, "Pagani Huayra\n", 6.0, 3300000));

    printf("\n----------\n\nAuction house contains:\n");
    parseListAndPrint(head);
    printf("\n--------------------\n");

    printf("\n----------\n\nThe next supercar was sold from the entrance!\n");
    SuperCar deletedFromBeginning = deleteFromBeginning(&head);
    printSuperCarToConsole(deletedFromBeginning);

    printf("\n----------\n\nThe next supercar was sold from the exit!\n");
    SuperCar deletedFromEnd = deleteFromEnd(&head);
    printSuperCarToConsole(deletedFromEnd);

    printf("\n----------\n\nAuction house contains:\n");
    parseListAndPrint(head);
    printf("\n--------------------\n");

    printf("\n----------\n\nWhat car do you want to buy?\n");
    int carWishId;
    scanf("%d", &carWishId);
    printf("\n----------\n\n\nCongratulations! You just bought a supercar:\n");
    SuperCar deletedSuperCarWithCondition = deleteWithCondition(&head, carWishId);
    printSuperCarToConsole(deletedSuperCarWithCondition);
    printf("\n--------------------\n");

    printf("\n----------\n\nThe auction house contains:\n");
    parseListAndPrint(head);
    printf("\n--------------------\n");
}