#define _CRT_SECURE_NO_WARNINGS

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define LINESIZE 128

typedef struct Masina Masina;
typedef struct Node Node;
typedef struct LD LD;
typedef struct Aparitie Aparitie;

struct Masina {
	unsigned int id;
	char* marca;
	char* model;
	float motor;
	int pret;
	unsigned char cai;
};

struct Node {
	Masina masina;
	Node* prev;
	Node* next;
};

struct LD {
	Node* head;
	Node* tail;
};

struct Aparitie {
	char* marca;
	int nrAparitii;
};

void afiseazaMasina(Masina masina) {
	printf("\n-----\nId: %d\nMarca: %s\nModel: %s\nMotor: %.2f\nPret: %d\nCai: %d\n-----\n",
		masina.id, masina.marca, masina.model, masina.motor, masina.pret, masina.cai);
}

Masina citesteMasinaDinFisier(FILE* f) {
	Masina masina;
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	masina.id = (unsigned int)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	masina.marca = malloc(strlen(buffer) + 1);
	strcpy(masina.marca, buffer);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	masina.model = malloc(strlen(buffer) + 1);
	strcpy(masina.model, buffer);

	fgets(buffer, LINESIZE, f);
	masina.motor = strtof(buffer, NULL);

	fgets(buffer, LINESIZE, f);
	masina.pret = (int)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	masina.cai = (unsigned char)strtol(buffer, NULL, 10);

	return masina;
}

void citesteMasiniDinFisier(char* numeFisier, Masina** masini, int* nrMasini) {
	FILE* f = fopen(numeFisier, "r");
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	(*nrMasini) = (int)strtol(buffer, NULL, 10);

	(*masini) = malloc((*nrMasini) * sizeof(Masina));
	for (int i = 0; i < (*nrMasini); i++) {
		(*masini)[i] = citesteMasinaDinFisier(f);
	}

	fclose(f);
}

void inserareInListaDublaLaInceput(LD* ld, Masina masina) {
	Node* newNode = malloc(sizeof(Node));
	newNode->masina = masina;
	newNode->prev = NULL;
	newNode->next = NULL;
	if (ld->head) {
		newNode->next = ld->head;
		ld->head->prev = newNode;
	}
	else {
		ld->tail = newNode;
	}
	ld->head = newNode;
}

void inserareInListaDublaLaSfarsit(LD* ld, Masina masina) {
	Node* newNode = malloc(sizeof(Node));
	newNode->masina = masina;
	newNode->prev = NULL;
	newNode->next = NULL;
	if (ld->tail) {
		newNode->prev = ld->tail;
		ld->tail->next = newNode;
	}
	else {
		ld->head = newNode;
	}
	ld->tail = newNode;
}

void inserareInListaDublaPePozitie(LD* ld, Masina masina, int pozitie) {
	Node* newNode = malloc(sizeof(Node));
	newNode->masina = masina;
	newNode->prev = NULL; 
	newNode->next = NULL;

	if (ld->head == NULL) {
		ld->head = newNode;
		ld->tail = newNode;
		return;
	}

	if (pozitie == 0) {
		newNode->next = ld->head;
		ld->head->prev = newNode;
		ld->head = newNode;
		return;
	}

	Node* current = ld->head;
	int index = 0;

	while (current != NULL && index < (pozitie - 1)) {
		current = current->next;
		index++;
	}

	if (current) { // NU AM AJUNS LA FINAL (INSEREZ IN INTERIOR)
		newNode->next = current->next;
		newNode->prev = current;
		if (current->next) {
			current->next->prev = newNode;
		}
		current->next = newNode;
		if (newNode->next == NULL) {
			ld->tail = newNode;
		}
	}
	else { // AM AJUNS LA FINAL
		ld->tail->next = newNode;
		newNode->prev = ld->tail;
		ld->tail = newNode;
	}
}

void afisareListaDublaStangaDreapta(LD ld) {
	if (ld.head) {
		while (ld.head) {
			afiseazaMasina(ld.head->masina);
			ld.head = ld.head->next;
		}
	}
	else {
		printf("\n\tLista goala!\n");
	}
}

void afisareListaDublaDreaptaStanga(LD ld) {
	if (ld.tail) {
		while (ld.tail) {
			afiseazaMasina(ld.tail->masina);
			ld.tail = ld.tail->prev;
		}
	}
	else {
		printf("\n\tLista goala!\n");
	}
}

int nrMasiniPestePrag(LD ld, unsigned char pragCai) {
	if (ld.head) {
		int count = 0;
		while (ld.head) {
			if (ld.head->masina.cai > pragCai) {
				count++;
			}
			ld.head = ld.head->next;
		}
		return count;
	}
	else {
		return 0;
	}
}

void schimbaMotor(LD* ld, char* model, float motor) {
	if (ld->head) {
		Node* aux = ld->head;
		while (aux) {
			if (strcmp(aux->masina.model, model) == 0) {
				aux->masina.motor = motor;
				break;
			}
			aux = aux->next;
		}
	}
	else {
		printf("\n\tLista goala!\n");
	}
}

void aplicaSoftStage1(LD* ld, unsigned char surplusCai) {
	if (ld->head) {
		// cautam masinile cu nr minim de cai
		Node* current = ld->head;
		unsigned char caiMinim = 255; // lungime max pt unsigned char

		while (current) {
			if (current->masina.cai < caiMinim) {
				caiMinim = current->masina.cai;
			}
			current = current->next;
		}

		current = ld->head;
		while (current) {
			if (current->masina.cai == caiMinim) {
				current->masina.cai += surplusCai;
			}
			current = current->next;
		}
	}
	else {
		printf("\n\tLista goala!\n");
	}
}

char* cautaNrMaxMasini(LD ld) {
	if (ld.head) {
		// cautam masina cu nr maxim de aparitii
		Node* current = ld.head;
		int maxAparitii = 0;
		char* marcaMaxAparitii = NULL;

		Aparitie aparitii[100];
		int nrAparitii = 0;

		while (current) {

			int found = 0;
			for (int i = 0; i < nrAparitii; i++) {
				if (strcmp(aparitii[i].marca, current->masina.marca) == 0) {
					aparitii[i].nrAparitii++;
					found = 1;
					break;
				}
			}

			if (!found) {
				aparitii[nrAparitii].marca = current->masina.marca;
				aparitii[nrAparitii].nrAparitii = 1;
				nrAparitii++;
			}

			current = current->next;
		}
		
		for (int i = 0; i < nrAparitii; i++) {
			if (aparitii[i].nrAparitii > maxAparitii) {
				maxAparitii = aparitii[i].nrAparitii;
				marcaMaxAparitii = aparitii[i].marca;
			}
		}

		return marcaMaxAparitii;
	}
	else {
		printf("\n\tLista goala!\n");
		return 0;
	}
}

void stergeNoduriCuPrag(LD* ld, unsigned char prag) {
	if (ld->head) {
		Node* current = ld->head;

		while (current) {
			Node* next = current->next;
			if (current->masina.cai < prag) {

				if (current->prev == NULL) {
					ld->head = next;
				}
				else {
					current->prev->next = next;
				}

				if (current->next == NULL) {
					ld->tail = current->prev;
				}
				else {
					next->prev = current->prev;
				}

				free(current->masina.marca);
				free(current->masina.model);
				free(current);
			}

			current = next;
		}
	}
	else {
		printf("\n\tLista goala!\n");
	}
}

int main() {
	Masina* masini;
	int nrMasini;

	citesteMasiniDinFisier("masini.txt", &masini, &nrMasini);
	printf("\n----------Masini din fisier----------\n");
	for (int i = 0; i < nrMasini; i++) {
		afiseazaMasina(masini[i]);
	}

	LD ld;
	ld.head = NULL;
	ld.tail = NULL;

	// 1.1
	/*for (int i = 0; i < nrMasini; i++) {
		inserareInListaDublaLaSfarsit(&ld, masini[i]);
	}*/

	// 1.2
	inserareInListaDublaPePozitie(&ld, masini[0], 0); 
	inserareInListaDublaPePozitie(&ld, masini[1], 1);
	inserareInListaDublaPePozitie(&ld, masini[2], 2);
	inserareInListaDublaPePozitie(&ld, masini[3], 3);
	inserareInListaDublaPePozitie(&ld, masini[4], 4);
	inserareInListaDublaPePozitie(&ld, masini[5], 5);
	inserareInListaDublaPePozitie(&ld, masini[6], 6);

	printf("\n----------Masini din lista dubla stanga dreapta----------\n");
	afisareListaDublaStangaDreapta(ld);

	printf("\n----------Masini din lista dubla dreapta stanga----------\n");
	afisareListaDublaDreaptaStanga(ld);

	// 2
	unsigned char pragCai = 200;
	int nrMasiniCuCaiPestePrag = nrMasiniPestePrag(ld, pragCai);
	printf("\n\tAm gasit %d masini care au peste %d cai\n", nrMasiniCuCaiPestePrag, pragCai); 

	// - pana aici 35 min

	// 3.1
	char* model = "Leon";
	float motorSchimbat = 3.0;
	schimbaMotor(&ld, model, motorSchimbat);

	printf("\n----------Masini din lista dubla stanga dreapta DUPA MODIFICARE MOTOR----------\n");
	afisareListaDublaStangaDreapta(ld);

	printf("\n----------Masini din lista dubla dreapta stanga DUPA MODIFICARE MOTOR----------\n");
	afisareListaDublaDreaptaStanga(ld);

	// 3.2
	unsigned char softCai = 35;
	aplicaSoftStage1(&ld, softCai);

	printf("\n----------Masini din lista dubla stanga dreapta DUPA STAGE----------\n");
	afisareListaDublaStangaDreapta(ld);

	printf("\n----------Masini din lista dubla dreapta stanga DUPA STAGE----------\n");
	afisareListaDublaDreaptaStanga(ld);

	// - pana aici 55 min

	// 4
	char* marca = cautaNrMaxMasini(ld);
	printf("\n\tMasina %s este cea cu nr maxim de aparitii!\n", marca);

	// - pana aici 68 min

	// 5
	unsigned char prag = 200;
	stergeNoduriCuPrag(&ld, prag);

	printf("\n----------Masini din lista dubla stanga dreapta DUPA STERGERI----------\n");
	afisareListaDublaStangaDreapta(ld);

	printf("\n----------Masini din lista dubla dreapta stanga DUPA STERGERI----------\n");
	afisareListaDublaDreaptaStanga(ld);

	// 75 min
}
