#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define LINESIZE 128

typedef struct Proiect Proiect;
typedef struct TreeNode TreeNode;

struct Proiect {
	unsigned int cod;
	char* titlu;
	char* beneficiar;
	unsigned int durata;
	float buget;
};

struct TreeNode {
	Proiect proiect;
	TreeNode* left;
	TreeNode* right;
};

Proiect initProiect(unsigned int cod, char* titlu, char* beneficiar, unsigned int durata, float buget) {
	Proiect proiect;

	proiect.cod = cod;
	proiect.titlu = malloc(strlen(titlu) + 1);
	strcpy(proiect.titlu, titlu);
	proiect.beneficiar = malloc(strlen(beneficiar) + 1);
	strcpy(proiect.beneficiar, beneficiar);
	proiect.durata = durata;
	proiect.buget = buget;

	return proiect;
}

void afiseazaProiect(Proiect proiect) {
	printf("\n-----\nCod: %d\nTitlu: %s\nBeneficiar: %s\nDurata: %d\nBuget: %.2f\n-----\n",
		proiect.cod, proiect.titlu, proiect.beneficiar, proiect.durata, proiect.buget);
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
	proiect.durata = (unsigned int)strtol(buffer, NULL, 10);

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

void inserareInTreeNode(TreeNode** root, Proiect proiect) {
	if (*root) {
		if ((*root)->proiect.cod > proiect.cod) {
			inserareInTreeNode(&(*root)->left, proiect);
		}
		else {
			inserareInTreeNode(&(*root)->right, proiect);
		}
	}
	else {
		TreeNode* newNode = malloc(sizeof(TreeNode));
		newNode->proiect = proiect;
		newNode->left = NULL;
		newNode->right = NULL; 
		*root = newNode; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
}

void inOrdine(TreeNode* root) {
	if (root) {
		inOrdine(root->left);
		afiseazaProiect(root->proiect);
		inOrdine(root->right);
	}
}

void preOrdine(TreeNode* root) {
	if (root) {
		afiseazaProiect(root->proiect);
		preOrdine(root->left);
		preOrdine(root->right);
	}
}

void postOrdine(TreeNode* root) {
	if (root) {
		postOrdine(root->left);
		postOrdine(root->right);
		afiseazaProiect(root->proiect);
	}
}

int proiecteBeneficiar(TreeNode* root, char* beneficiar) {
	if (!root) {
		return 0;
	}

	int count = 0;

	if (strcmp(root->proiect.beneficiar, beneficiar) == 0) {
		count = 1;
	}

	return count
		+ proiecteBeneficiar(root->left, beneficiar)
		+ proiecteBeneficiar(root->right, beneficiar);
}

void modificaBuget(TreeNode** root, unsigned int cod, float buget) {
	if ((*root)->proiect.cod == cod) {
		(*root)->proiect.buget = buget;
		return; // uitam de el !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	if ((*root)->proiect.cod > cod) {
		modificaBuget(&(*root)->left, cod, buget);
	}
	else {
		modificaBuget(&(*root)->right, cod, buget);
	}
}

Proiect cautaNod(TreeNode* root, unsigned int cod) {
	if (!root) {
		return initProiect(0, "???", "???", 0, 0.0);
	}

	if ((*root).proiect.cod == cod) {
		return root->proiect;
	}
	else if ((*root).proiect.cod > cod) {
		cautaNod((*root).left, cod);
	}
	else {
		cautaNod((*root).right, cod); 
	}
}

int noduriFrunza(TreeNode* root) {
	if (!root) {
		return 0;
	}

	if (root->left == NULL && root->right == NULL) {
		return 1;
	}

	return noduriFrunza(root->left) + noduriFrunza(root->right);
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
	TreeNode* root = NULL;
	for (int i = 0; i < nrProiecte; i++) {
		inserareInTreeNode(&root, proiecte[i]);
	}

	printf("\n----------Proiecte din ABC INORDINE----------\n");
	inOrdine(root);
	printf("\n----------Proiecte din ABC PREORDINE----------\n");
	preOrdine(root);
	printf("\n----------Proiecte din ABC POSTORDINE----------\n");
	postOrdine(root);

	// 2
	char* beneficiar = "SIF Evergent";
	int nrProiecteBeneficiar = proiecteBeneficiar(root, beneficiar);
	printf("\n\tS-au gasit %d proiecte cu beneficiarul %s", nrProiecteBeneficiar, beneficiar);

	// 3
	unsigned int cod = 3;
	float buget = 7777.77;
	modificaBuget(&root, cod, buget);
	Proiect proiectGasit = cautaNod(root, cod);
	printf("\n\tNodul cu bugetul actualizat:\n");
	afiseazaProiect(proiectGasit);

	// 4
	int nrFrunze = noduriFrunza(root);
	printf("\n\tNoduri frunza: %d", nrFrunze);
	
}