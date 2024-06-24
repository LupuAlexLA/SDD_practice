#define _CRT_SECURE_NO_WARNINGS

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Programare Programare;
typedef struct Node Node;
typedef struct List List;

struct Programare {
	char* dataProgramare;
	char* numeClient;
	float durataProgramare;
	char* serviciiEfectuate;
	int costProgramare;
};

struct Node {
	Programare programare;
	Node* next;
};

struct List {
	char* dataProgramare;
	Node* head;
	List* nextList;
};

void afiseazaProgramare(Programare programare) {
	printf("\n-----\nData: %s\nClient: %s\nDurata: %.2f\nServicii: %s\nCost: %d\n-----\n",
		programare.dataProgramare, programare.numeClient, programare.durataProgramare, programare.serviciiEfectuate, programare.costProgramare);
}

void afiseazaLista(List* head) {
	List* listTemp = head;
	while (listTemp) {
		Node* nodeTemp = listTemp->head;
		printf("\n\tData: %s", listTemp->dataProgramare);
		while (nodeTemp) {
			afiseazaProgramare(nodeTemp->programare);
			nodeTemp = nodeTemp->next;
		}
		listTemp = listTemp->nextList;
	}
}

Programare citesteProgramare(FILE* f) {
	Programare programare;
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	programare.dataProgramare = malloc(strlen(buffer) + 1);
	strcpy(programare.dataProgramare, buffer);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	programare.numeClient = malloc(strlen(buffer) + 1);
	strcpy(programare.numeClient, buffer);

	fgets(buffer, LINESIZE, f);
	programare.durataProgramare = strtof(buffer, NULL);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	programare.serviciiEfectuate = malloc(strlen(buffer) + 1);
	strcpy(programare.serviciiEfectuate, buffer);

	fgets(buffer, LINESIZE, f);
	programare.costProgramare = (int)strtol(buffer, NULL, 10);

	return programare;
}

void citesteProgramari(char* numeFisier, Programare** programari, int* nrProgramari) {
	FILE* f = fopen(numeFisier, "r");
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	(*nrProgramari) = (int)strtol(buffer, NULL, 10);

	(*programari) = malloc((*nrProgramari) * sizeof(Programare));
	for (int i = 0; i < (*nrProgramari); i++) {
		(*programari)[i] = citesteProgramare(f);
	}

	fclose(f);
}

List* cautaListaDupaData(List* head, char* dataProgramare) {
	List* listTemp = head;
	while (listTemp) {
		if (strcmp(listTemp->dataProgramare, dataProgramare) == 0) {
			return listTemp;
		}
		listTemp = listTemp->nextList;
	}
	return NULL;
}

Node* creazaNod(Programare programare) {
	Node* newNode = malloc(sizeof(Node));
	newNode->programare = programare;
	newNode->next = NULL;
	return newNode;
}

List* creazaLista(char* dataProgramare) {
	List* newList = malloc(sizeof(List));
	newList->dataProgramare = malloc(strlen(dataProgramare) + 1);
	strcpy(newList->dataProgramare, dataProgramare);
	newList->head = NULL;
	newList->nextList = NULL;
	return newList;
}

void insereazaNod(List* lista, Programare programare) {
	Node* newNode = creazaNod(programare);
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

void inserarareInListaDeListe(List** listaDeListe, Programare programare) {
	List* lista = cautaListaDupaData(*listaDeListe, programare.dataProgramare);

	if (lista == NULL) {
		lista = creazaLista(programare.dataProgramare);
		insereazaInLista(listaDeListe, lista); 
	}
	insereazaNod(lista, programare); 
}

int valoareTotalaLaData(List* listaDeListe, char* dataProgramare) {
	List* lista = cautaListaDupaData(listaDeListe, dataProgramare);
	if (lista == NULL) {
		return 0;
	}

	int sumaTotala = 0;
	Node* current = lista->head;
	while (current) {
		sumaTotala += current->programare.costProgramare;
		current = current->next;
	}

	return sumaTotala;
}

void modificaProgramare(List* listaDeListe, char* numeClient, int costNou) {
	List* listTemp = listaDeListe;
	while (listTemp) {
		Node* nodeTemp = listTemp->head;
		while (nodeTemp) {
			if (strcmp(nodeTemp->programare.numeClient, numeClient) == 0) {
				nodeTemp->programare.costProgramare = costNou;
				return;
			}
			nodeTemp = nodeTemp->next;
		}
		listTemp = listTemp->nextList;
	}
}

void stergeProgramariCuDurataMaiMica(List** listaDeListe, float prag) {
	List* listTemp = *listaDeListe;
	List* prevList = NULL;

	while (listTemp) {
		Node* nodeTemp = listTemp->head;
		Node* prevNode = NULL;

		while (nodeTemp) {
			if (nodeTemp->programare.durataProgramare < prag) {
				if (prevNode) {
					prevNode->next = nodeTemp->next;
				}
				else {
					listTemp->head = nodeTemp->next;
				}
				Node* tempNode = nodeTemp;
				nodeTemp = nodeTemp->next;
				free(tempNode->programare.dataProgramare);
				free(tempNode->programare.numeClient);
				free(tempNode->programare.serviciiEfectuate);
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
			free(tempList->dataProgramare);
			free(tempList);
		}
		else {
			prevList = listTemp;
			listTemp = listTemp->nextList;
		}
	}
}

int main() {
	Programare* programari;
	int nrProgramari;

	citesteProgramari("programari.txt", &programari, &nrProgramari);
	printf("\n----------Programari din fisier----------\n");
	for (int i = 0; i < nrProgramari; i++) {
		afiseazaProgramare(programari[i]);
	}

	// 1
	List* listaDeListe = NULL;

	for (int i = 0; i < nrProgramari; i++) {
		inserarareInListaDeListe(&listaDeListe, programari[i]);
	}

	printf("\n----------Programari din lista de liste----------\n");
	afiseazaLista(listaDeListe);

	// 2
	char* dataProgramre = "01 06 2024";
	int valoareTotala = valoareTotalaLaData(listaDeListe, dataProgramre);
	printf("\n\tValoarea totala a programarilor la data %s este %d\n", dataProgramre, valoareTotala);

	// 3
	char* numeClient = "Ion Popescu";
	int costNou = 500;
	modificaProgramare(listaDeListe, numeClient, costNou);

	printf("\n----------Programari din lista de liste dupa modificare----------\n");
	afiseazaLista(listaDeListe);

	// 4
	float prag = 2.0;
	stergeProgramariCuDurataMaiMica(&listaDeListe, prag);

	printf("\n----------Programari din lista de liste dupa stergere----------\n");
	afiseazaLista(listaDeListe);
}