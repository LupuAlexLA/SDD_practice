#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define LINESIZE 128

typedef struct Proiect Proiect;
typedef struct Node Node;
typedef struct HT HT;

struct Proiect {
	unsigned int cod;
	char* titlu;
	char* beneficiar;
	unsigned char nrExecutanti;
	float buget;
};

struct Node {
	Proiect proiect;
	Node* next;
};

struct HT {
	int size;
	Node** lists;
};

Proiect initProiect(unsigned int cod, char* titlu, char* beneficiar, unsigned char nrExecutanti, float buget) {
	Proiect proiect;

	proiect.cod = cod;
	proiect.titlu = malloc(strlen(titlu) + 1);
	strcpy(proiect.titlu, titlu);   
	proiect.beneficiar = malloc(strlen(beneficiar) + 1);
	strcpy(proiect.beneficiar, beneficiar);
	proiect.nrExecutanti = nrExecutanti;
	proiect.buget = buget;

	return proiect;
}

void afiseazaProiect(Proiect proiect) {
	printf("\n-----\nCod: %d\nTitlu: %s\nBeneficiar: %s\nNr executanti: %d\nBuget: %.2f\n-----\n",
		proiect.cod, proiect.titlu, proiect.beneficiar, proiect.nrExecutanti, proiect.buget);
}

void afiseazaProiectSimplu(Proiect proiect) {
	printf("\n-----\nCod: %d\nBeneficiar: %s\n-----\n",
		proiect.cod, proiect.beneficiar);
}

Proiect citesteProiect(FILE* f) {
	Proiect proiect;
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	proiect.cod = (int)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	proiect.titlu = malloc(strlen(buffer) + 1);
	strcpy(proiect.titlu, buffer);

	fgets(buffer, LINESIZE, f); 
	buffer[strcspn(buffer, "\n")] = 0; 
	proiect.beneficiar = malloc(strlen(buffer) + 1);
	strcpy(proiect.beneficiar, buffer);

	fgets(buffer, LINESIZE, f);
	proiect.nrExecutanti = (unsigned char)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	proiect.buget = strtof(buffer, NULL);

	return proiect;
}

void citesteProiecte(char* numeFisier, Proiect** proiecte, int* nrProiecte) {
	FILE* f = fopen(numeFisier, "r");
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	(*nrProiecte) = (int)strtol(buffer, NULL, 10);

	(*proiecte) = malloc((*nrProiecte) * sizeof(Proiect));
	for (int i = 0; i < (*nrProiecte); i++) {
		(*proiecte)[i] = citesteProiect(f);
	}
	
	fclose(f);
}

HT initHT(int size) {
	HT ht;

	ht.size = size;
	ht.lists = malloc(ht.size * sizeof(Node*)); // am uitat * la sizeof
	for (int i = 0; i < ht.size; i++) {
		ht.lists[i] = NULL;
	}

	return ht;
}

int hashFunction(int size, char* key) {
	int sum = 0;
	for (int i = 0; i < strlen(key); i++) {
		sum += key[i];
	}
	return sum % size;
}

void inserareInLista(Node** head, Proiect proiect) {
	Node* newNode = malloc(sizeof(Node));
	newNode->proiect = proiect;
	newNode->next = NULL;

	if (*head) {
		Node* aux = *head;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = newNode;
	}
	else {
		*head = newNode;
	}
}

void inserareInHT(HT* ht, Proiect proiect) {
	int position = hashFunction(ht->size, proiect.beneficiar); // f imp da ca param si ht.size

	inserareInLista(&ht->lists[position], proiect);
}

void afiseazaLista(Node* head) {
	if (head) {
		while (head) {
			afiseazaProiectSimplu(head->proiect);
			head = head->next;
		}
	}
	else {
		printf("\n\tLista goala!\n");
	}
}

void afiseazaHT(HT ht) {
	for (int i = 0; i < ht.size; i++) {
		printf("\n\tPozitie: %d", i);
		afiseazaLista(ht.lists[i]);
	}
}

float bugetTotalBeneficiar(HT ht, char* beneficiar) {
	int position = hashFunction(ht.size, beneficiar);

	if (ht.lists[position]) {
		float bugetTotal = 0;
		Node* aux = ht.lists[position];
		while (aux) {
			if (strcmp(aux->proiect.beneficiar, beneficiar) == 0) {
				bugetTotal += aux->proiect.buget;
			}
			aux = aux->next;
		}
	}
	else {
		return 0;
	}
}

void modificareBeneficiar(HT* ht, char* beneficiar, char* beneficiarNou) {
	int position = hashFunction((*ht).size, beneficiar);

	if ((*ht).lists[position]) {
		Node* aux = (*ht).lists[position];
		while (aux) {
			if (strcmp(aux->proiect.beneficiar, beneficiar) == 0) {
				free(aux->proiect.beneficiar);
				aux->proiect.beneficiar = malloc(strlen(beneficiarNou) + 1);
				strcpy(aux->proiect.beneficiar, beneficiarNou);
			}
			aux = aux->next;
		}
	}
}

int main() {
	Proiect* proiecte;
	int nrProiecte;

	citesteProiecte("proiecte.txt", &proiecte, &nrProiecte);
	printf("\n----------Proiecte din fisier----------\n");
	for (int i = 0; i < nrProiecte; i++) {
		afiseazaProiect(proiecte[i]);
	}

	// 1
	//HT ht = initHT(ht);
	HT ht = initHT(7);

	for (int i = 0; i < nrProiecte; i++) {
		inserareInHT(&ht, proiecte[i]);
	}

	// 2
	printf("\n----------Proiecte din HT----------\n");
	afiseazaHT(ht);

	// 3
	char* beneficiar = "ASE";
	float bugetTotal = bugetTotalBeneficiar(ht, beneficiar);
	printf("\n\tBuget total pentru beneficiarul %s : %.2f\n", beneficiar, bugetTotal);

	// 4
	char* beneficiarNou = "UNIV ASE";
	modificareBeneficiar(&ht, beneficiar, beneficiarNou);

	printf("\n----------Proiecte din HT----------\n"); 
	afiseazaHT(ht); 

}