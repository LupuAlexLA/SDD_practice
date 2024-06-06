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

// Funcția primește doi parametri: un pointer către pointerul care indică spre capul 
// listei (head) și structura Sneaker care va fi inserată în listă.
void insertAtBeginning(Node **head, Sneaker sneaker) {
    // Inițial, funcția alocă memorie pentru un nou nod de tip Node. Acest nou nod va
    // stoca datele sneakerului și va fi plasat la începutul listei. 
    Node *newNode = malloc(sizeof(Node));

    // Sneakerul furnizat ca argument este atribuit direct membrului sneaker al structurii Node. 
    // Acest pas copiază toate câmpurile din structura Sneaker în nodul nou creat.
    newNode->sneaker = sneaker;
    // Pointerul next al noului nod este setat să indice spre nodul care era anterior capul listei, 
    // adică spre nodul curent indicat de *head. Aceasta este cheia care face ca nodul nou să fie plasat la începutul listei.
    newNode->next = *head;

    //Pentru a finaliza inserția, capul listei (pointerul *head) este actualizat pentru a indica spre noul nod. 
    // Acest pas este crucial pentru că modifică lista astfel încât noul nod să devină primul nod al listei. 
    // Prin actualizarea capului listei, asigurăm că lista înlănțuită rămâne coerentă și că accesul la lista 
    // începe cu noul nod inserat.
    *head = newNode;
}

// Această funcție primește doi parametri: un pointer către pointerul care indică spre capul listei (head) 
// și structura Sneaker care va fi adăugată la sfârșitul listei.
void insertAtEnd(Node **head, Sneaker sneaker) {
    // Înainte de a proceda cu inserția, funcția verifică dacă lista este inițial goală (adică *head este NULL). 
    if (*head) {
        // Dacă lista nu este goală, se inițializează un pointer auxiliar (aux) pentru a naviga prin listă. 
        // Acesta este setat inițial la capul listei (*head).
        Node *aux = *head;
        // Se folosește o buclă while pentru a parcurge lista până când se găsește ultimul nod, 
        // adică nodul care nu are un următor (next este NULL).
        while (aux->next)
            aux = aux->next;

        // După ce s-a ajuns la sfârșitul listei, se alocă memorie pentru un nou nod (newNode).
        Node *newNode = malloc(sizeof(Node));

        // Noul nod este inițializat cu datele Sneaker primite ca parametru, și câmpul next este setat la NULL, 
        // indicând că acesta este acum ultimul nod din listă.
        newNode->sneaker = sneaker;
        newNode->next = NULL;
        // Ultimul nod din listă, identificat anterior (aux), este legat de noul nod prin setarea pointerului 
        // next al lui aux la newNode. Acest pas finalizează procesul de adăugare a noului element la sfârșitul listei.
        aux->next = newNode;
    } else // Dacă lista este goală, adăugarea unui element la sfârșitul listei este echivalentă cu inserția la început, așa că apelează funcția insertAtBeginning.
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

// Această funcție acceptă un singur argument, head, care este un pointer către primul nod al listei înlănțuite.
void parseListAndPrint(Node *head) {
    // Înainte de a începe traversarea, funcția verifică dacă există elemente în listă verificând dacă head este NULL.
    if (head) {
        // Dacă lista conține elemente, funcția intră într-o buclă while care continuă 
        // până când nu mai există noduri (când head devine NULL).
        while (head) {
            // Funcția printSneakerToConsole este apelată pentru a afișa informațiile despre sneakerul din nodul curent. 
            // Aceasta include ID-ul, numele, mărimea și prețul sneakerului.
            printSneakerToConsole(head->sneaker);
            // Mutăm `head` la următorul nod în listă
            head = head->next;
        }
    } else // Dacă head este NULL, înseamnă că lista nu conține elemente, și funcția afișează un mesaj corespunzător.
        printf("List is empty\n");
}

// Funcția primește un singur argument, head, care este un pointer către pointerul care indică spre capul listei înlănțuite.
Sneaker deleteFromBeginning(Node **head) {
    // În primul rând, funcția verifică dacă lista este goală verificând dacă *head este NULL. 
    // Acesta este un caz special care trebuie tratat separat pentru a evita dereferențierea unui pointer NULL.
    if (*head) {
        // Dacă lista nu este goală, se procedează la eliminarea primului nod.
        Sneaker deletedSneaker;
        // Un pointer temporar (oldHead) este utilizat pentru a reține adresa nodului curent de la începutul listei. 
        // Acest pas este crucial pentru a nu pierde referința la nodul care trebuie eliminat.
        Node *oldHead = *head;

        // Pointerul head este actualizat pentru a indica spre următorul nod din listă, 
        // efectiv eliminând legătura cu nodul curent (oldHead).
        *head = (*head)->next;

        // Informațiile Sneaker din nodul eliminat sunt salvate într-o variabilă locală (deletedSneaker) 
        // înainte de a elibera memoria nodului.
        deletedSneaker = oldHead->sneaker;
        // Memoria alocată pentru oldHead este eliberată folosind free, prevenind astfel scurgerile de memorie.
        free(oldHead);

        // Funcția returnează sneakerul din nodul eliminat, permițând aplicației să proceseze 
        // sau să afișeze informațiile despre sneakerul eliminat.
        return deletedSneaker;
    } else {
        // Dacă lista este goală, funcția construiește și returnează un sneaker cu valori 
        // specifice pentru eroare, indicând că nu există elemente de eliminat.
        Sneaker errorSneaker = {.id = 0, .name = "Error Sneaker\n", .size = 0.0f, .price = 0};
        return errorSneaker;
    }
}

// Această funcție acceptă un singur argument, head, care este un pointer către pointerul 
// care indică spre capul listei înlănțuite.
Sneaker deleteFromEnd(Node **head) {
    // Funcția începe prin a verifica dacă lista este goală, adică *head este NULL.
    if (*head) {
        // Dacă lista conține un singur nod ((*head)->next este NULL), 
        // acesta este eliminat direct. Se salvează informațiile despre sneaker, se eliberează nodul, 
        // iar capul listei este setat la NULL.
        if ((*head)->next) {
            // Dacă există mai multe noduri în listă, se folosește un pointer auxiliar (aux) pentru 
            // a parcurge lista până când acesta indică penultimul nod, adică nodul a cărui proprietate 
            // next indică spre ultimul nod.
            Node *aux = *head;
            // pozitionare pe penultimul nod
            while (aux->next->next)
                aux = aux->next;
            // aux = penultimul; aux->next = ultimul
            // După identificarea penultimului nod, se salvează informațiile despre sneakerul din ultimul nod, 
            // se eliberează memoria alocată pentru ultimul nod, iar legătura next a penultimului nod este setată 
            // la NULL pentru a decupa ultimul nod din listă.
            Sneaker sneaker = aux->next->sneaker;
            free(aux->next);
            aux->next = NULL;

            // Sneakerul din nodul eliminat este returnat, permițând utilizatorului să obțină acces la 
            // informațiile despre produsul eliminat.
            return sneaker;
        } else {
            Sneaker sneaker = (*head)->sneaker;
            free(*head);
            *head = NULL;
            return sneaker;
        }
    } else {
        // Dacă lista este goală, nu există noduri de eliminat, și se returnează 
        // un sneaker cu valori specifice pentru eroare.
        Sneaker empty = {.id = 0, .name = "???\n", .size = 0.0f, .price = 0};
        return empty;
    }
}

// Funcția primește doi parametri: un pointer către capul listei (head) și un id 
// după care se va face căutarea și eliminarea nodului.
Sneaker deleteWithCondition(Node **head, int id) {
    // Funcția începe prin a verifica dacă lista este goală, adică dacă *head este NULL. 
    if (*head) {
        // Dacă sneakerul din nodul de la capul listei are ID-ul căutat, 
        // se folosește funcția deleteFromBeginning pentru a elimina și a returna sneakerul.
        if ((*head)->sneaker.id == id)
            return deleteFromBeginning(head);

        // Dacă nodul de la începutul listei nu are ID-ul căutat, se parcurge lista pentru 
        // a găsi nodul care trebuie eliminat. Se folosește un pointer auxiliar (aux) care începe de la capul listei.
        if ((*head)->next) {
            Node *aux = *head;
            // Se iterează prin listă până când aux->next este NULL sau până când se găsește nodul cu ID-ul căutat.
            while (aux->next && (aux->next->sneaker.id != id))
                aux = aux->next;

            // După terminarea iterației, se verifică dacă nodul căutat a fost găsit. Dacă aux->next este NULL, 
            // înseamnă că nodul cu ID-ul specificat nu a fost găsit, și se returnează un sneaker de eroare.
            if (!aux->next) {
                printf("Item not found!\n");
                Sneaker empty = {.id = 0, .name = "???\n", .size = 0.0f, .price = 0};
                return empty;
            }

            // Dacă nodul a fost găsit, informațiile sneakerului sunt salvate, 
            // nodul este deconectat de la listă, și memoria alocată pentru el este eliberată.
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
        // Dacă lista este goală, nu există noduri de eliminat, și se returnează 
        // un sneaker cu valori specifice pentru eroare.
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
