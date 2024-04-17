#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LINESIZE 128

typedef struct Vehicle Vehicle;
typedef struct Node Node;

struct Vehicle {
    int id;
    char *brand;
    float engineSize;
    int price;
};

struct Node {
    Vehicle vehicle;
    Node *next;
};

void printVehicleToConsole(Vehicle vehicle) {
    printf("\n----------\nId: %d\nBrand: %sEngine: %.2f\nPrice: %d\n----------\n",
           vehicle.id, vehicle.brand, vehicle.engineSize, vehicle.price);
}

Vehicle initVehicle(int id, char* brand, float engineSize, int price) {
    Vehicle vehicle = { .id = id, .brand = brand, .engineSize= engineSize, .price = price };

    vehicle.brand = malloc(strlen(brand) + 1);
    strcpy(vehicle.brand, brand);

    return vehicle;
}

Vehicle deepCopy(Vehicle vehicle) {
    Vehicle copy;

    copy.id = vehicle.id;
    copy.brand = (char *) malloc(strlen(vehicle.brand) + 1);
    strcpy(copy.brand, vehicle.brand);
    copy.engineSize = vehicle.engineSize;
    copy.price = vehicle.price;

    return copy;
}

Vehicle readVehicleFromFile(FILE *f) {
    Vehicle vehicle;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    vehicle.id = (int) strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    vehicle.brand = (char *) malloc(strlen(buffer) + 1);
    strcpy(vehicle.brand, buffer);

    fgets(buffer, LINESIZE, f);
    vehicle.engineSize = strtof(buffer, NULL);

    fgets(buffer, LINESIZE, f);
    vehicle.price = (int) strtol(buffer, NULL, 10);

    return vehicle;
}

void readVehiclesFromFile(const char *fileName, Vehicle **auctionHouse, int *auctionHouseSize) {
    char buffer[LINESIZE];

    FILE *f = fopen(fileName, "r");

    fgets(buffer, LINESIZE, f);
    *auctionHouseSize = (int) strtol(buffer, NULL, 10);

    *auctionHouse = malloc((*auctionHouseSize) * sizeof(Vehicle));

    for (int i = 0; i < *auctionHouseSize; i++)
        (*auctionHouse)[i] = readVehicleFromFile(f);

    fclose(f);
}

void insertAtBeginning(Node **head, Vehicle vehicle) {
    Node *newNode = malloc(sizeof(Vehicle));

    newNode->vehicle = vehicle;
    newNode->next = *head;

    *head = newNode;
}

void insertAtEnd(Node **head, Vehicle vehicle) {
    if (*head) {
        Node* aux = *head;

        while(aux->next)
            aux = aux->next;

        Node* newNode = malloc(sizeof(Vehicle));

        newNode->vehicle = vehicle;
        newNode->next = NULL;

        aux->next = newNode;
    }
    else
        insertAtBeginning(head, vehicle);
}

void parseListAndPrint(Node* head) {
    if(head) {
        while(head) {
            printVehicleToConsole(head->vehicle);
            head = head->next;
        }
    }
    else
        printf("The auction house has no cars!");
}

Vehicle deleteFromBeginning(Node** head) {
    if(*head) {
        Vehicle deletedVehicle;
        Node* oldHead = *head;

        *head = (*head)->next;
        deletedVehicle = oldHead->vehicle;
        free(oldHead);

        return deletedVehicle;
    }
    else {
        Vehicle errorVehicle = { .id = 0, .brand = "???", .engineSize = 0.0f, .price = 0 };
        return errorVehicle;
    }
}

Vehicle deleteFromEnd(Node** head) {
    if(*head) {
        if((*head)->next) {
            Node* aux = *head;

            while(aux->next->next)
                aux = aux->next;

            Vehicle vehicle = aux->next->vehicle;
            free(aux->next);
            aux->next = NULL;
            return vehicle;
        }
        else {
            Vehicle vehicle = (*head)->vehicle;
            free(*head);
            *head = NULL;
            return vehicle;
        }
    }
    else {
        Vehicle errorVehicle = { .id = 0, .brand = "???", .engineSize = 0.0f, .price = 0 };
        return errorVehicle;
    }
}

Vehicle deleteWithCondition(Node **head, int id) {
    if (*head) {
        if ((*head)->vehicle.id == id)
            return deleteFromBeginning(head);

        if ((*head)->next) {
            Node *aux = *head;

            while (aux->next && (aux->next->vehicle.id != id))
                aux = aux->next;

            if (!aux->next) {
                printf("Car not found!\n");
                Vehicle empty = {.id = 0, .brand = "???\n", .engineSize = 0.0f, .price = 0};
                return empty;
            }

            Vehicle vehicle = aux->next->vehicle; // save sneaker
            Node *deletedNode = aux->next; // save node for later freeing
            aux->next = aux->next->next; // update link
            free(deletedNode); // free memory of deleted node

            return vehicle;
        } else {
            if ((*head)->vehicle.id == id) {
                Vehicle vehicle = (*head)->vehicle;

                free(*head);
                *head = NULL;

                return vehicle;
            } else {
                Vehicle empty = {.id = 0, .brand = "???\n", .engineSize = 0.0f, .price = 0};
                return empty;
            }
        }
    } else {
        Vehicle empty = {.id = 0, .brand = "???\n", .engineSize = 0.0f, .price = 0};
        return empty;
    }
}

int main() {
    int auctionHouseSize;
    Vehicle *auctionHouse;

    printf("\n----------\n\nAuction house contains:\n");
    readVehiclesFromFile("vehicles.txt", &auctionHouse, &auctionHouseSize);
    for (int i = 0; i < auctionHouseSize; i++)
        printVehicleToConsole(auctionHouse[i]);
    printf("\n--------------------\n");

    printf("\n----------\n\nPopulate the list of vehicles from the auction house file!\n");
    Node *head = NULL;
    for (int i = 0; i < auctionHouseSize; i++)
        insertAtEnd(&head, auctionHouse[i]);
    parseListAndPrint(head);
    printf("\n--------------------\n");

    printf("\n----------\n\nBugatti Chiron is available in the parking lot at the entrance!\n");
    insertAtBeginning(&head, initVehicle(0, "Bugatti Chiron\n", 8.0f, 3000000));
    printf("\n----------\n\nPagani Huayra is available in the parking lot at the exit!\n");
    insertAtEnd(&head, initVehicle(9, "Pagani Huayra\n", 6.0, 3300000));

    printf("\n----------\n\nThe auction house contains:\n");
    parseListAndPrint(head);
    printf("\n--------------------\n");

    printf("\n----------\n\nThe car from the entrance was sold!\n");
    printf("\n----------\n\nThe next car was sold:\n");
    Vehicle deletedFromBeginnig = deleteFromBeginning(&head);
    printVehicleToConsole(deletedFromBeginnig);

    printf("\n----------\n\nThe car from the exit was sold!\n");
    printf("\n----------\n\nThe next car was sold:\n");
    Vehicle deletedFromEnd = deleteFromEnd(&head);
    printVehicleToConsole(deletedFromEnd);

    printf("\n----------\n\nThe auction house contains:\n");
    parseListAndPrint(head);
    printf("\n--------------------\n");

    printf("\n----------\n\nWhat car do you want to buy?\n");
    int carWishId;
    scanf("%d", &carWishId);
    printf("\n----------\n\n\nCongratulations! You just bought a supercar:\n");
    Vehicle deletedVehicleWithCondition = deleteWithCondition(&head, carWishId);
    printVehicleToConsole(deletedVehicleWithCondition);
    printf("\n--------------------\n");

    printf("\n----------\n\nThe auction house contains:\n");
    parseListAndPrint(head);
    printf("\n--------------------\n");
}