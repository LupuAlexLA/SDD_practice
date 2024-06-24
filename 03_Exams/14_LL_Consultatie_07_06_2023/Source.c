#define _CRT_SECURE_NO_WARNINGS

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>

#define LINESIZE 128

typedef struct Consultatie Consultatie;
typedef struct Node Node;
typedef struct List List;

struct Consultatie {
	char* dataConsultatie;
	char* numeMedic;
	char* specialitate;
	char* diagnostic;
	int pret;
};

struct Node {
	Consultatie consultatie;
	Node* next;
};

struct List {
	char* specialitate;
	Node* head;
	List* nextList;
};

void afiseazaConsultatie(Consultatie consultatie) {
	printf("\n-----\nData: %s\nMedic: %s\nSpecialitate: %s\nDiagnostic: %s\nPret: %d\n-----\n",
		consultatie.dataConsultatie, consultatie.numeMedic, consultatie.specialitate, consultatie.diagnostic, consultatie.pret);
}

void afiseazaLista(List* head) {
	List* listTemp = head;
	while (listTemp) {
		Node* nodeTemp = listTemp->head;
		printf("\n\tSpecialitate: %s", listTemp->specialitate);
		while (nodeTemp) {
			afiseazaConsultatie(nodeTemp->consultatie);
			nodeTemp = nodeTemp->next;
		}
		listTemp = listTemp->nextList;
	}
}

Consultatie citesteConsultatie(FILE* f) {
	Consultatie consultatie;
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	consultatie.dataConsultatie = malloc(strlen(buffer) + 1);
	strcpy(consultatie.dataConsultatie, buffer);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	consultatie.numeMedic = malloc(strlen(buffer) + 1);
	strcpy(consultatie.numeMedic, buffer);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	consultatie.specialitate = malloc(strlen(buffer) + 1);
	strcpy(consultatie.specialitate, buffer);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	consultatie.diagnostic = malloc(strlen(buffer) + 1);
	strcpy(consultatie.diagnostic, buffer);

	fgets(buffer, LINESIZE, f);
	consultatie.pret = (int)strtol(buffer, NULL, 10);

	return consultatie;
}

void citesteConsultatii(char* numeFisier, Consultatie** consultatii, int* nrConsultatii) {
	FILE* f = fopen(numeFisier, "r");
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	(*nrConsultatii) = (int)strtol(buffer, NULL, 10);

	(*consultatii) = malloc((*nrConsultatii) * sizeof(Consultatie));
	for (int i = 0; i < (*nrConsultatii); i++) {
		(*consultatii)[i] = citesteConsultatie(f);
	}

	fclose(f);
}

List* cautaListaDupaSpecialitate(List* head, char* specialitate) {
	List* listTemp = head;
	while (listTemp) {
		if (strcmp(listTemp->specialitate, specialitate) == 0) {
			return listTemp;
		}
		listTemp = listTemp->nextList;
	}
	return NULL;
}

Node* creazaNod(Consultatie consultatie) {
	Node* newNode = malloc(sizeof(Node));
	newNode->consultatie = consultatie;
	newNode->next = NULL;
	return newNode;
}

List* creazaLista(char* specialitate) {
	List* newList = malloc(sizeof(List));
	newList->specialitate = malloc(strlen(specialitate) + 1);
	strcpy(newList->specialitate, specialitate);
	newList->head = NULL;
	newList->nextList = NULL;
	return newList;
}

void insereazaNod(List* lista, Consultatie consultatie) {
	Node* newNode = creazaNod(consultatie);
	if (lista->head) {
		Node* aux = lista->head;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = newNode;
	}
	else {
		lista->head = newNode;
	}
}

void insereazaInLista(List** head, List* lista) {
	if (*head == NULL) {
		*head = lista;
	}
	else {
		List* aux = *head;
		while (aux->nextList) {
			aux = aux->nextList;
		}
		aux->nextList = lista;
	}
}

void inserarareInListaDeListe(List** listaDeListe, Consultatie consultatie) {
	List* lista = cautaListaDupaSpecialitate(*listaDeListe, consultatie.specialitate);

	if (lista == NULL) {
		lista = creazaLista(consultatie.specialitate);
		insereazaInLista(listaDeListe, lista);
	}
	insereazaNod(lista, consultatie);
}

int valoareTotalaLaSpecialitatea(List* listaDeListe, char* specialitate) {
	List* lista = cautaListaDupaSpecialitate(listaDeListe, specialitate);
	if (lista == NULL) {
		return 0;
	}

	int sumaTotala = 0;
	Node* current = lista->head;
	while (current) {
		sumaTotala += current->consultatie.pret;
		current = current->next;
	}

	return sumaTotala;
}

void modificaConsultatie(List* listaDeListe, char* dataConsultatie, int pretNou) {
	List* listTemp = listaDeListe;
	while (listTemp) {
		Node* nodeTemp = listTemp->head;
		while (nodeTemp) {
			if (strcmp(nodeTemp->consultatie.dataConsultatie, dataConsultatie) == 0) {
				nodeTemp->consultatie.pret = pretNou;
				return;
			}
			nodeTemp = nodeTemp->next;
		}
		listTemp = listTemp->nextList;
	}
}

void stergeConsultatiiCuDiagnostic(List** listaDeListe, char* diagnostic) {
	List* listTemp = *listaDeListe;
	List* prevList = NULL;

	while (listTemp) {
		Node* nodeTemp = listTemp->head;
		Node* prevNode = NULL;

		while (nodeTemp) {
			//if (nodeTemp->programare.durataProgramare < prag) {
			if (strcmp(nodeTemp->consultatie.diagnostic, diagnostic) == 0) {
				if (prevNode) {
					prevNode->next = nodeTemp->next;
				}
				else {
					listTemp->head = nodeTemp->next;
				}
				Node* tempNode = nodeTemp;
				nodeTemp = nodeTemp->next;
				free(tempNode->consultatie.dataConsultatie);
				free(tempNode->consultatie.numeMedic);
				free(tempNode->consultatie.specialitate);
				free(tempNode->consultatie.diagnostic);
				free(tempNode);
			}
			else {
				prevNode = nodeTemp;
				nodeTemp = nodeTemp->next;
			}
		}

		if (listTemp->head == NULL) {
			if (prevList) {
				prevList->nextList = listTemp->nextList;
			}
			else {
				*listaDeListe = listTemp->nextList;
			}
			List* tempList = listTemp;
			listTemp = listTemp->nextList;
			free(tempList->specialitate);
			free(tempList);
		}
		else {
			prevList = listTemp;
			listTemp = listTemp->nextList;
		}
	}
}

int main() {
	Consultatie* consultatii;
	int nrConsultatii;

	citesteConsultatii("consultatii.txt", &consultatii, &nrConsultatii);
	printf("\n----------Consultatii din fisier----------\n");
	for (int i = 0; i < nrConsultatii; i++) {
		afiseazaConsultatie(consultatii[i]);
	}

	// 1
	List* listaDeListe = NULL;

	for (int i = 0; i < nrConsultatii; i++) {
		inserarareInListaDeListe(&listaDeListe, consultatii[i]);
	}

	printf("\n----------Programari din lista de liste----------\n");
	afiseazaLista(listaDeListe);

	// 2
	char* specialitate = "Cardiologie";
	int valoareTotala = valoareTotalaLaSpecialitatea(listaDeListe, specialitate);
	printf("\n\tValoarea totala a consultatiilor la specialitatea %s este %d\n", specialitate, valoareTotala);

	// 3
	char* dataConsultatie = "02 06 2024";
	int pretNou = 333;
	modificaConsultatie(listaDeListe, dataConsultatie, pretNou);

	printf("\n----------Programari din lista de liste dupa modificare----------\n");
	afiseazaLista(listaDeListe);

	// 4
	char* diagnostic = "Infectie urinara";
	stergeConsultatiiCuDiagnostic(&listaDeListe, diagnostic);

	printf("\n----------Programari din lista de liste dupa stergere----------\n");
	afiseazaLista(listaDeListe);
}