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
    char *name;
    float size;
    int price;
};

struct Node {
    Sneaker sneaker;
    Node *next;
};

struct NodeDLL {
    Sneaker sneaker;
    NodeDLL *prev;
    NodeDLL *next;
};

struct DoublyLinkedList {
    NodeDLL *head;
    NodeDLL *tail;
};

void printSneakerToConsole(Sneaker sneaker) {
    printf("Id: %d\nName: %sSize: %.2f\nPrice: %d\n\n",
           sneaker.id, sneaker.name, sneaker.size, sneaker.price);
}

Sneaker initSneaker(int id, char *name, float size, int price) {

    Sneaker newSneaker = {.id = id, .name = name, .size = size, .price = price};

    newSneaker.name = malloc((strlen(name) + 1));
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

void insertAtBeginningDLL(DoublyLinkedList *dll, Sneaker sneaker) {
    // Aloci memorie pentru un nou NodeDLL.
    NodeDLL *newNode = malloc(sizeof(NodeDLL));
    // Inițializezi nodul cu sneakerul primit ca parametru.
    newNode->sneaker = sneaker;

    // Setezi next la nodul care este în prezent capul listei (dll->head), deoarece noul nod va prelua poziția de cap al listei.
    newNode->next = (*dll).head;
    // Setezi prev la NULL deoarece va fi primul nod în listă, deci nu are un nod anterior.
    newNode->prev = NULL;

    // Verifici dacă lista este goală verificând (*dll).head.
    if ((*dll).head) {
        // Dacă lista nu este goală ((*dll).head != NULL)
        // Setezi prev al nodului care era anterior capul listei să indice spre noul nod, creând astfel legătura înapoi spre noul cap al listei.
        (*dll).head->prev = newNode;
    } else {
        // Dacă lista este goală:
        // Setezi tail al listei să indice spre noul nod, deoarece noul nod va fi singurul nod în listă, deci este și capul și coada listei.
        (*dll).tail = newNode;
    }

    // Setezi head al listei să indice spre noul nod, făcându-l astfel noul cap al listei.
    (*dll).head = newNode;
}

void insertAtEndDLL(DoublyLinkedList *dll, Sneaker sneaker) {
    // Aloci memorie pentru un nou NodeDLL.
    NodeDLL *newNode = malloc(sizeof(NodeDLL));
    // Inițializezi nodul cu sneakerul primit ca parametru.
    newNode->sneaker = sneaker;

    // Setezi prev la nodul care este în prezent coada listei (dll->tail), deoarece noul nod va fi adăugat după actuala coadă.
    newNode->prev = (*dll).tail;
    // Setezi next la NULL pentru că acesta va fi ultimul nod din listă.
    newNode->next = NULL;

    // Verifici dacă lista este goală verificând (*dll).tail.
    if ((*dll).tail) {
        // Dacă lista nu este goală ((*dll).tail != NULL):
        // Setezi next al nodului care era anterior coada listei să indice spre noul nod, extinzând lista către noul său sfârșit.
        (*dll).tail->next = newNode;
    } else {
        // Dacă lista este goală:
        // Setezi head al listei să indice spre noul nod, deoarece noul nod va fi singurul nod din listă, deci este și capul și coada listei.
        (*dll).head = newNode;
    }

    // Setezi tail al listei să indice spre noul nod, făcându-l astfel noua coadă a listei.
    (*dll).tail = newNode;
}

void parseListBtoE(DoublyLinkedList dll) {
    // Înainte de a începe parcurgerea, verifici dacă lista conține elemente verificând dacă head este NULL.
    if (dll.head) {
        // Începi parcurgerea listei de la primul nod (head).
        // Utilizezi un buclă while pentru a traversa lista. În fiecare iterație:
        while (dll.head) {
            // Afișezi datele sneakerului folosind funcția printSneakerToConsole.
            printSneakerToConsole(dll.head->sneaker);
            // Mutați head la următorul nod folosind dll.head = dll.head->next.
            dll.head = dll.head->next;
            // Bucla se termină când dll.head devine NULL, adică ai ajuns la sfârșitul listei.
        }
    } else
        printf("List is empty\n"); // Dacă head este NULL, afișezi că lista este goală.
}

void parseListEtoB(DoublyLinkedList dll) {
    // Verifici dacă tail este NULL, indicând că lista nu conține elemente.
    if (dll.tail) {
        // Începi parcurgerea de la ultimul nod (tail).
        // Utilizezi o buclă while pentru a traversa lista invers. În fiecare iterație:
        while (dll.tail) {
            // Afișezi datele sneakerului folosind funcția printSneakerToConsole.
            printSneakerToConsole(dll.tail->sneaker);
            // Mutați tail la nodul anterior folosind dll.tail = dll.tail->prev.
            dll.tail = dll.tail->prev;
            // Bucla se termină când dll.tail devine NULL, adică ai ajuns la începutul listei.
        }
    } else
        printf("List is empty\n"); // Dacă tail este NULL, afișezi că lista este goală.
}

Sneaker deleteFromBeginningDLL(DoublyLinkedList *dll) {
    // Începi prin a verifica dacă lista este goală, examinând dacă head al listei este NULL.
    if ((*dll).head) {
        // Salvarea referinței la nodul curent (head) într-o variabilă locală (currentNode).
        NodeDLL *currentNode = (*dll).head;
        // Salvarea datelor sneakerului din nodul curent într-o variabilă (deletedSneaker) pentru a-l putea returna după ce nodul este eliminat.
        Sneaker deletedSneaker = currentNode->sneaker;

        // Verifici dacă există un nod după nodul curent:
        if ((*dll).head->next) {
            // Dacă există, actualizezi head al listei la următorul nod.
            (*dll).head = (*dll).head->next;
            // Setezi prev al noului head la NULL, pentru a decupla legătura cu nodul eliminat.
            (*dll).head->prev = NULL;
            // Eliberezi memoria alocată pentru nodul curent.
            free(currentNode);

            return deletedSneaker;
        } else {
            // Dacă nodul curent este singurul nod în listă (adică (*dll).head->next este NULL):
            // Eliberezi memoria alocată pentru nodul curent.
            free(currentNode);
            // Setezi atât head, cât și tail la NULL, indicând că lista este acum complet goală.
            (*dll).head = NULL;
            (*dll).tail = NULL;

            // Returnezi sneakerul care a fost eliminat din lista pentru a confirma operația și pentru a permite utilizatorului să știe ce element a fost scos din listă.
            return deletedSneaker;
        }
    }
    // Dacă lista este goală, returnezi un sneaker de eroare, indicând că nu există elemente de eliminat.
    return initSneaker(0, "ERROR SNEAKER", 0.0, 0);
}

Sneaker deleteFromEndDLL(DoublyLinkedList *dll) {
    // Începi prin a verifica dacă lista este goală, examinând dacă tail al listei este NULL.
    if ((*dll).tail) {

        // Salvarea referinței la nodul curent (tail) într-o variabilă locală (currentNode).
        NodeDLL *currentNode = (*dll).tail;
        // Salvarea datelor sneakerului din nodul curent într-o variabilă (deletedSneaker) pentru a-l putea returna după ce nodul este eliminat.
        Sneaker deletedSneaker = currentNode->sneaker;

        // Verifici dacă există un nod înaintea nodului curent:
        if ((*dll).tail->prev) {
            // Dacă există, actualizezi tail al listei la nodul anterior.
            (*dll).tail = (*dll).tail->prev;
            // Setezi next al noului tail la NULL, pentru a decupla legătura cu nodul eliminat.
            (*dll).tail->next = NULL;
            // Eliberezi memoria alocată pentru nodul curent.
            free(currentNode);

            // Returnezi sneakerul care a fost eliminat din lista pentru a confirma operația și pentru a permite utilizatorului să știe ce element a fost scos din listă.
            return deletedSneaker;
        } else {
            // Dacă nodul curent este singurul nod în listă (adică (*dll).tail->prev este NULL):
            // Eliberezi memoria alocată pentru nodul curent.
            free(currentNode);
            // Setezi atât head, cât și tail la NULL, indicând că lista este acum complet goală.
            (*dll).head = NULL;
            (*dll).tail = NULL;

            return deletedSneaker;
        }
    }
    // Dacă lista este goală, returnezi un sneaker de eroare, indicând că nu există elemente de eliminat.
    return initSneaker(0, "ERROR SNEAKER", 0.0, 0);
}

int listLength(DoublyLinkedList dll) {
    // Inițiezi un contor (len) cu valoarea zero. Acest contor va fi utilizat pentru a număra nodurile din listă.
    int len = 0;
    // Utilizezi o buclă while pentru a traversa lista. Bucla rulează atâta timp cât dll.head nu este NULL, adică există noduri în listă.
    while (dll.head) {
        // În fiecare iterație a buclei
        // Incrementezi contorul len cu 1 pentru a număra nodul curent.
        len += 1;
        // Mutați head la următorul nod din listă folosind dll.head = dll.head->next. Acest pas mută referința de la un nod la următorul până când ajunge la sfârșitul listei.
        dll.head = dll.head->next;
    }
    // După ce bucla se încheie (când dll.head devine NULL), variabila len conține numărul total de noduri din listă.
    // Returnezi această valoare pentru a oferi lungimea listei.
    return len;
}

int main() {
    int sneakersNumber;
    Sneaker *sneakersArray;

    readSneakersFromFile("sneakers.txt", &sneakersArray, &sneakersNumber);

    printf("##########\nPrinting the sneakers from file!\n##########\n");
    for (int i = 0; i < sneakersNumber; i++) {
        printSneakerToConsole(sneakersArray[i]);
    }

    printf("---------------------\n");

    DoublyLinkedList dll;

    dll.head = NULL;
    dll.tail = NULL;

    for (int i = 0; i < sneakersNumber; i++)
        insertAtEndDLL(&dll, sneakersArray[i]);

    printf("##########\nPrinting the sneakers from double list!\n##########\n");
    parseListBtoE(dll);

    printf("---------------------\n");

    printf("##########\nInserting sneaker at beginning: id: 0; name: first; size: 40.5; price: 500\n##########\n");
    insertAtBeginningDLL(&dll, initSneaker(0, "first\n", 40.5f, 500));
    printf("##########\nInserting sneaker at end: id: 6; name: last; size: 40.5; price: 500\n##########\n");
    insertAtEndDLL(&dll, initSneaker(6, "last\n", 42.5f, 500));

    printf("##########\nAsc Printing the sneakers from double list after inserts!\n##########\n");
    parseListBtoE(dll);

    printf("---------------------\n");

    printf("##########\nDesc Printing the sneakers from double list after inserts!\n##########\n");
    parseListEtoB(dll);

    printf("---------------------\n");


    Sneaker deletedSneakerFromBeginning = deleteFromBeginningDLL(&dll);
    printf("##########\nSneaker sters de la inceput:\n##########\n");
    printSneakerToConsole(deletedSneakerFromBeginning);
    printf("---------------------\n");

    Sneaker deletedSneakerFromEnd = deleteFromEndDLL(&dll);
    printf("##########\nSneaker sters de la inceput:\n##########\n");
    printSneakerToConsole(deletedSneakerFromEnd);
    printf("---------------------\n");

    printf("##########\nAsc Printing the sneakers from double list after delete!\n##########\n");
    parseListBtoE(dll);

    printf("---------------------\n");

    return 0;
}
