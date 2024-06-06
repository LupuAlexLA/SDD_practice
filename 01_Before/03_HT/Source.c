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

// utilizată pentru a determina la ce poziție în tabelul de dispersie (hash table) 
// ar trebui să fie plasat un element bazat pe o cheie specifică
int hashFunction(int htSize, char *key) // key = description
{
    // Începi cu o variabilă sum inițializată la zero. Aceasta va fi folosită pentru 
    // a acumula suma valorilor ASCII ale caracterelor din cheie.
    int sum = 0;
    // Utilizezi o buclă for pentru a itera prin fiecare caracter al cheii.
    for (int i = 0; i < strlen(key); i++) {
        // În cadrul buclei, adaugi valoarea ASCII a fiecărui caracter la suma totală.
        // key[i] accesează fiecare caracter din string-ul cheie, 
        // iar sum += key[i] adaugă valoarea ASCII a caracterului la suma totală.
        sum += key[i];
    }
    // După ce ai calculat suma totală a valorilor ASCII ale caracterelor din cheie, 
    // calculezi valoarea hash. Aceasta se face prin aplicarea operatorului modulo (%)
    // asupra sumei, cu dimensiunea tabelului (htSize) ca divizor.
    // Operația sum % htSize returnează restul împărțirii sumei la dimensiunea 
    // tabelului. Acesta este un mod comun de a reduce o valoare mare la una dintre 
    // pozițiile valide din tabelul de dispersie, asigurând că valoarea hash este 
    // întotdeauna între 0 și htSize-1.
    return sum % htSize;
}

// Această funcție configurează o nouă tabelă de dispersie cu o dimensiune specificată
HT initHashTable(int size) {
    // Creezi o variabilă ht de tip HT (structura HashTable definită în cod).
    HT ht;
    // Setezi câmpul size al structurii ht la valoarea primită ca argument. 
    // Acest câmp reprezintă numărul de „găleți” sau „sloturi” din tabelă.
    ht.size = size;
    // Aloci memorie pentru vectorul lists, care va conține pointeri către 
    // începuturile listelor înlănțuite. Fiecare element al acestui vector 
    // reprezintă capul unei liste, utilizate pentru a gestiona coliziunile care apar
    // atunci când două sau mai multe elemente sunt mapate la aceeași poziție în tabelă.
    // Utilizezi malloc pentru a aloca memorie suficientă pentru size pointeri 
    // de tip Node*. Acest pas este crucial pentru asigurarea că tabelul poate 
    // stoca efectiv listele de date.
    ht.lists = malloc(size * sizeof(Node *)); // vector de liste
    // Iterezi prin vectorul de liste nou-alocat, setând fiecare pointer la NULL. 
    // Aceasta asigură că toate „gălețile” din tabel sunt inițial goale, pregătite 
    // pentru a adăuga date.
    for (int i = 0; i < ht.size; i++)
        ht.lists[i] = NULL;
    // După ce toți pașii de inițializare sunt compleți, returnezi structura ht. 
    // Acum, ht este configurată corespunzător și gata să fie folosită pentru 
    // inserții, căutări, etc.
    return ht;
}
// Funcția insertList este folosită pentru a insera un element (sneaker) într-o listă 
// înlănțuită, gestionând coliziunile într-o tabelă de dispersie. Aceasta verifică 
// dacă sneakerul există deja în listă și, dacă da, îl actualizează; altfel, 
// adaugă sneakerul la sfârșitul listei.
// Această funcție inserează un sneaker într-o listă specificată prin pointerul la capul 
// listei (head) și sneakerul de inserat.
void insertList(Node **head, Sneaker sneaker) // la final
{
    // Aloci memorie pentru un nou nod folosind malloc.
    Node *newNode = malloc(sizeof(Node));
    // Inițializezi nodul cu sneakerul dat ca argument.
    newNode->sneaker = sneaker;
    // Setezi câmpul next al nodului la NULL, pregătindu-l să fie ultimul nod din listă.
    newNode->next = NULL;

    // Verifici dacă lista nu este goală (*head nu este NULL).
    if (*head) {
        // Dacă lista conține elemente, parcurgi lista pentru a verifica dacă sneakerul există deja.
        Node *aux = *head;
        // la fiecare nod parcurs verific daca e identic cu cel pe care vreau sa l introduc
        int different = strcmp(aux->sneaker.name, sneaker.name);

        // Utilizezi o buclă while pentru a parcurge lista până când găsești un nod 
        // cu același nume sau până când ajungi la sfârșitul listei.
        while (different && aux->next) {
            aux = aux->next;
            // different este folosit pentru a determina dacă numele sneakerului curent 
            // diferă de cel pe care vrei să-l inserezi.
            different = strcmp(aux->sneaker.name, sneaker.name);
        }

        // Dacă găsești un nod cu același nume (different == 0), 
        // actualizezi datele acestui nod cu noile valori ale sneakerului.
        if (!different) {
            // pe nodul in care s-a oprit while-ul
            // Eliberezi memoria pentru numele vechi și pentru nodul nou creat, 
            // care nu mai este necesar.
            free(aux->sneaker.name);
            free(newNode);
            aux->sneaker = sneaker;
        } else {
            // Dacă ajungi la sfârșitul listei și nu găsești un sneaker cu același nume, 
            // adaugi nodul nou la sfârșitul listei.
            aux->next = newNode;
        }
    } else {
        *head = newNode;
    }
}

// Funcția insertHashTable este esențială pentru a adăuga un nou element în 
// tabela de dispersie, utilizând funcția de hash pentru a determina locația 
// corectă în tabelă și gestionând coliziunile prin liste înlănțuite. 
// Această funcție inserează un sneaker în tabela de dispersie (hash table). 
// Primește ca argumente structura de tip HT (tabela de dispersie) 
// și Sneaker (elementul de inserat).
void insertHashTable(HT ht, Sneaker sneaker) {
    // Folosește funcția hashFunction pentru a determina indexul corespunzător 
    // în vectorul de liste al tabelei de dispersie, bazându-se pe numele sneakerului. 
    // Acest index este locul în care sneakerul ar trebui să fie stocat în tabelă.
    int position = hashFunction(ht.size, sneaker.name);

    // Verifică dacă există deja un nod la poziția calculată în tabelă (ht.lists[position]). 
    // Prezența unui nod indică o posibilă coliziune, adică două sau mai multe elemente 
    // au fost asignate aceleiași poziții.
    if (ht.lists[position]) // facut doar pt a detecta coliziuni
    {
        //coliziune detectata
        // Indiferent dacă poziția este deja ocupată sau nu, apelează funcția insertList 
        // pentru a adăuga sneakerul în lista aferentă poziției determinate.
        // Dacă poziția este ocupată, acest pas gestionează coliziunea prin adăugarea 
        // sneakerului în lista existentă.
        // Dacă poziția este liberă, inițializează lista la acea poziție cu noul sneaker.
        // Funcția insertList este inteligentă și poate actualiza un sneaker existent 
        // sau poate adăuga unul nou la sfârșitul listei, cum am discutat în detaliul 
        // funcției anterioare.
        insertList(&ht.lists[position], sneaker);
    } else {
        // empty bucket - introducem elem in lista
        insertList(&ht.lists[position], sneaker);
    }
}

// Funcția parseListAndPrint este utilizată pentru a parcurge și afișa conținutul 
// unei liste înlănțuite, tipic utilizată în contextul gestiunii coliziunilor 
// într-un tabel de dispersie. Aceasta afișează toate sneakerii dintr-o listă 
// specifică, începând de la capul listei (head).
// Această funcție acceptă un singur parametru, head, care este un pointer 
// către primul nod al listei înlănțuite.
void parseListAndPrint(Node *head) {
    // Înainte de a începe parcurgerea, funcția verifică dacă lista conține 
    // elemente verificând dacă head nu este NULL.
    if (head) {
        // Dacă lista conține elemente, funcția intră într-o buclă while care 
        // continuă până când nu mai există noduri (când head devine NULL).
        while (head) {
            //printf("\t");
            // Sneakerul din nodul curent este afișat utilizând funcția printSneakerToConsole.
            printSneakerToConsole(head->sneaker);
            // Pointerul head este mutat la următorul nod din listă (head->next). 
            head = head->next;
        }
    } else {
        // Dacă head este NULL, înseamnă că lista este goală, 
        // și funcția afișează un mesaj corespunzător.
        printf("\tEmpty bucket\n");
    }
}

// Funcția printHashTable este folosită pentru a afișa întregul conținut al unei 
// tabele de dispersie. Această funcție este esențială pentru vizualizarea structurii 
// și conținutului tabelului, permițându-ți să vezi cum sunt distribuite datele și 
// cum sunt gestionate coliziunile.
// Această funcție primește ca argument o structură de tip HT (HashTable), 
// care reprezintă tabelul de dispersie ce trebuie afișat.
void printHashTable(HT ht) {
    // Funcția folosește o buclă for pentru a parcurge fiecare poziție din tabelă, 
    // de la 0 până la ht.size - 1. Această abordare asigură că fiecare „bucket” 
    // sau poziție din tabel va fi procesată.
    for (int i = 0; i < ht.size; i++) {
        // Pentru fiecare iterație a buclei, se afișează indexul poziției curente 
        // în tabel. Aceasta servește ca o etichetă pentru a indica locul din tabel 
        // unde se află lista de elemente (sneakeri) gestionate la acea poziție.
        printf("Position %d\n", i);
        // Funcția parseListAndPrint este apelată cu argumentul ht.lists[i], 
        // care este capul listei de la poziția i în tabel. Aceasta funcție este 
        // responsabilă pentru afișarea tuturor sneakerilor din lista respectivă.
        parseListAndPrint(ht.lists[i]);
        // Dacă lista este goală (adică ht.lists[i] este NULL), parseListAndPrint 
        // va afișa un mesaj indicând că bucket-ul este gol. 
        // Dacă lista conține elemente, acestea vor fi afișate unul câte unul.
    }
}

// Funcția searchHashTable este destinată căutării unui sneaker în tabelul de dispersie 
// bazându-se pe numele acestuia, folosind o cheie specificată (key). 
// Această funcție caută un sneaker în tabelul de dispersie 
// folosind numele sneakerului ca cheie de căutare.
Sneaker searchHashTable(HT ht, char *key) {
    // Inițial, folosești funcția de hash hashFunction pentru a determina indexul 
    // la care ar trebui să fie stocat sau găsit sneakerul, în funcție de numele acestuia. 
    // Acesta calculează poziția bazându-se pe dimensiunea tabelului și cheia dată.
    int position = hashFunction(ht.size, key);

    // Verifici dacă la poziția calculată (position) există o listă inițializată 
    // (adică, lista nu este NULL).
    if (ht.lists[position]) // nu are sens if-ul si else-ul (e redundant)
    {
        // daca lista e populata parcurgem pana gasim elem (daca il gasim)
        // Dacă la acea poziție există o listă, începi să parcurgi lista pentru a găsi sneakerul căutat.
        Node *aux = ht.lists[position]; // primul elem din lista

        // Utilizezi o buclă while pentru a parcurge fiecare nod din listă. 
        // Continui căutarea până când fie găsești un sneaker cu numele căutat, 
        // fie ajungi la sfârșitul listei (aux devine NULL).
        while (aux && strcmp(key, aux->sneaker.name))
            aux = aux->next;

        // Dacă aux nu este NULL după terminarea buclei, înseamnă că ai găsit sneakerul căutat. 
        // Afișezi mesajul corespunzător și returnezi sneakerul găsit.
        if (aux) {
            printf("Found at position %d\n", position);
            return initSneaker(aux->sneaker.id, aux->sneaker.name, aux->sneaker.size, aux->sneaker.price);
        } 
        else // Dacă aux este NULL, înseamnă că sneakerul căutat nu există în listă, și returnezi un sneaker de eroare.
        {
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

