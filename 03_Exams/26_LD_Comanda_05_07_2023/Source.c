#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Comanda Comanda;
typedef struct Node Node;
typedef struct LD LD;
typedef struct Client Client;

struct Comanda {
	unsigned int numar;
	char* dataComanda;
	char* dataLivrare;
	char* numeClient;
	float suma;
};

struct Node {
	Comanda comanda;
	Node* next;
	Node* prev;
};

struct LD {
	Node* head;
	Node* tail;
};

struct Client {
	char* numeClient;
	int numarComenzi;
};

void afiseazaComanda(Comanda comanda) {
	printf("\n-----\nNumar: %d\nData Comanda: %s\nData Livrare: %s\nClient: %s\nSuma: %.2f\n-----\n",
		comanda.numar, comanda.dataComanda, comanda.dataLivrare, comanda.numeClient, comanda.suma);
}

Comanda citesteComandaDinFisier(FILE* f) {
	Comanda comanda;
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	comanda.numar = (int)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	comanda.dataComanda = malloc(strlen(buffer) + 1);
	strcpy(comanda.dataComanda, buffer);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	comanda.dataLivrare = malloc(strlen(buffer) + 1);
	strcpy(comanda.dataLivrare, buffer);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	comanda.numeClient = malloc(strlen(buffer) + 1);
	strcpy(comanda.numeClient, buffer);

	fgets(buffer, LINESIZE, f);
	comanda.suma = strtof(buffer, NULL);

	return comanda;
}

void citesteComenziDinFisier(char* numeFisier, Comanda** comenzi, int* nrComanda) {
	FILE* f = fopen(numeFisier, "r");
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	(*nrComanda) = (int)strtol(buffer, NULL, 10);

	(*comenzi) = malloc((*nrComanda) * sizeof(Comanda));
	for (int i = 0; i < (*nrComanda); i++) {
		(*comenzi)[i] = citesteComandaDinFisier(f);
	}
	fclose(f);
}

void inserareLaInceput(LD* ld, Comanda comanda) {
	Node* newNode = malloc(sizeof(Node));
	newNode->comanda = comanda;
	newNode->prev = NULL;
	newNode->next = ld->head;
	if (ld->head) {
		ld->head->prev = newNode;
	}
	else {
		ld->tail = newNode;
	}
	ld->head = newNode;
}

void inserareLaSfarsit(LD* ld, Comanda comanda) {
	Node* newNode = malloc(sizeof(Node));
	newNode->comanda = comanda;
	newNode->next = NULL;
	newNode->prev = ld->tail;
	if (ld->tail) {
		ld->tail->next = newNode;
	}
	else {
		ld->head = newNode;
	}
	ld->tail = newNode;
}

void afisareStangaDreapta(LD ld) {
	if (ld.head) {
		while (ld.head) {
			afiseazaComanda(ld.head->comanda);
			ld.head = ld.head->next;
		}
	}
	else {
		printf("\n\tLista goala!\n");
	}
}

void afisareDreaptaStanga(LD ld) {
	if (ld.tail) {
		while (ld.tail) {
			afiseazaComanda(ld.tail->comanda);
			ld.tail = ld.tail->prev;
		}
	}
	else {
		printf("\n\tLista goala!\n");
	}
}

void modificaValoareComenziCuSumaMaxima(LD* ld, float discount) {
	if (ld->head == NULL) {
		printf("\n\tLista goala!\n");
		return;
	}

	Node* current = ld->head;
	float sumaMaxima = 0.0;

	while (current) {
		if (current->comanda.suma > sumaMaxima) {
			sumaMaxima = current->comanda.suma;
		}
		current = current->next;
	}

	current = ld->head;
	while (current) {
		if (current->comanda.suma == sumaMaxima) {
			current->comanda.suma -= (current->comanda.suma * discount / 100);
		}
		current = current->next;
	}
}

char* clientCuNrMaxComenzi(LD ld) {
	Node* current = ld.head;
	int maxComenzi = 0;
	char* clientMaxComenzi = NULL;

	Client clienti[100];
	int nrClienti = 0;

	while (current) {
		int found = 0;
		for (int i = 0; i < nrClienti; i++) {
			if (strcmp(clienti[i].numeClient, current->comanda.numeClient) == 0) {
				clienti[i].numarComenzi++;
				found = 1;
				break;
			}
		}

		if (!found) {
			clienti[nrClienti].numeClient = current->comanda.numeClient;
			clienti[nrClienti].numarComenzi = 1;
			nrClienti++;
		}

		current = current->next;
	}

	for (int i = 0; i < nrClienti; i++) {
		if (clienti[i].numarComenzi > maxComenzi) {
			maxComenzi = clienti[i].numarComenzi;
			clientMaxComenzi = clienti[i].numeClient;
		}
	}

	return clientMaxComenzi;
}

int main() {
	Comanda* comenzi;
	int nrComenzi;

	citesteComenziDinFisier("comenzi.txt", &comenzi, &nrComenzi);

	printf("\n----------Comenzi din fisier----------\n");
	for (int i = 0; i < nrComenzi; i++) {
		afiseazaComanda(comenzi[i]);
	}

	// 1
	LD ld;
	ld.head = NULL; 
	ld.tail = NULL;

	for (int i = 0; i < nrComenzi; i++) {
		inserareLaSfarsit(&ld, comenzi[i]);
	}
	printf("\n----------Comenzi din lista dubla stanga dreapta----------\n");
	afisareStangaDreapta(ld);

	printf("\n----------Comenzi din lista dubla dreapta stanga----------\n");
	afisareDreaptaStanga(ld);

	// 2

	// 3
	float discount = 10.0;
	modificaValoareComenziCuSumaMaxima(&ld, discount);

	printf("\n----------Comenzi din lista dubla stanga dreapta dupa modificare----------\n");
	afisareStangaDreapta(ld);

	printf("\n----------Comenzi din lista dubla dreapta stanga dupa modificare----------\n");
	afisareDreaptaStanga(ld);

	// 4
	char* clientMax = clientCuNrMaxComenzi(ld);
	printf("\n\tClientul cu cel mai mare numar de comenzi: %s\n", clientMax);

	// 5

	// 6
	Node* aux = ld.head;
	while ((ld.head != NULL) && (ld.tail != NULL))
	{
		aux = ld.head;
		ld.head = ld.tail->next;
		if (ld.head == NULL) {
			ld.tail = NULL;
		}
		free(aux->comanda.dataComanda);
		free(aux->comanda.dataLivrare);
		free(aux->comanda.numeClient);
		free(aux);
	}

	for (int i = 0; i < nrComenzi; i++) {
		free(comenzi[i].dataComanda);
		free(comenzi[i].dataLivrare);
		free(comenzi[i].numeClient);
	}
	free(comenzi);

	printf("\n----------Comenzi din lista dubla stanga dreapta dupa modificare----------\n");
	afisareStangaDreapta(ld);

	printf("\n----------Comenzi din lista dubla dreapta stanga dupa modificare----------\n");
	afisareDreaptaStanga(ld);
}