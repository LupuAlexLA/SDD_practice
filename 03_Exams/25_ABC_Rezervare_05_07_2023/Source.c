#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h> // la includerea lui FILE
#include<string.h>
#include<stdlib.h> // nu citeste corect din fisier fara 
//#include<malloc.h>

#define LINESIZE 128

typedef struct Rezervare Rezervare;
typedef struct TreeNode TreeNode;

struct Rezervare {
	unsigned int numarRezervare;
	char* denumireHotel;
	unsigned char nrCamereRezervate;
	char* numeClient;
	char* dataRezervare;
};

struct TreeNode {
	Rezervare rezervare;
	TreeNode* left;
	TreeNode* right;
};

Rezervare initRezervare(unsigned int numarRezervare, char* denumireHotel, unsigned char nrCamereRezervate, char* numeClient, char* dataRezervare) {
	Rezervare rezervare;

	rezervare.numarRezervare = numarRezervare;
	rezervare.denumireHotel = malloc(strlen(denumireHotel) + 1);
	strcpy(rezervare.denumireHotel, denumireHotel);
	rezervare.nrCamereRezervate = nrCamereRezervate;
	rezervare.numeClient = malloc(strlen(numeClient) + 1);
	strcpy(rezervare.numeClient, numeClient);
	rezervare.dataRezervare = malloc(strlen(dataRezervare) + 1);
	strcpy(rezervare.dataRezervare, dataRezervare);

	return rezervare;
}

void afisareRezervare(Rezervare rezervare) {
	printf("\n-----\nNr rezervare: %d\nHotel: %s\nNr camere rezervare: %d\nClient: %s\nData: %s\n-----\n",
		rezervare.numarRezervare, rezervare.denumireHotel, rezervare.nrCamereRezervate, rezervare.numeClient, rezervare.dataRezervare);
}

Rezervare citesteRezervare(FILE* f) {
	Rezervare rezervare;
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	rezervare.numarRezervare = (unsigned int)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	rezervare.denumireHotel = malloc(strlen(buffer) + 1);
	strcpy(rezervare.denumireHotel, buffer);

	fgets(buffer, LINESIZE, f);
	rezervare.nrCamereRezervate = (unsigned char)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	rezervare.numeClient = malloc(strlen(buffer) + 1);
	strcpy(rezervare.numeClient, buffer);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = 0;
	rezervare.dataRezervare = malloc(strlen(buffer) + 1);
	strcpy(rezervare.dataRezervare, buffer);

	return rezervare;
}

void citesteRezervari(char* numeFisier, Rezervare** rezervari, int* nrRezervari) {
	FILE* f = fopen(numeFisier, "r");
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	(*nrRezervari) = (int)strtol(buffer, NULL, 10);

	(*rezervari) = malloc((*nrRezervari) * sizeof(Rezervare));
	for (int i = 0; i < (*nrRezervari); i++) {
		(*rezervari)[i] = citesteRezervare(f);
	}
	
	fclose(f);
}

void insertInTreeNode(TreeNode** root, Rezervare rezervare) {
	if (*root) {
		if ((*root)->rezervare.numarRezervare > rezervare.numarRezervare) {
			insertInTreeNode(&(*root)->left, rezervare);
		}
		else {
			insertInTreeNode(&(*root)->right, rezervare);
		}
	}
	else {
		TreeNode* newNode = malloc(sizeof(TreeNode));
		newNode->rezervare = rezervare;
		newNode->left = NULL;
		newNode->right = NULL;
		*root = newNode;
	}
}

void inOrdine(TreeNode* root) {
	if (root) {
		inOrdine(root->left);
		afisareRezervare(root->rezervare);
		inOrdine(root->right);
	}
}

void preOrdine(TreeNode* root) {
	if (root) {
		afisareRezervare(root->rezervare);
		preOrdine(root->left);
		preOrdine(root->right);
	}
}

void postOrdine(TreeNode* root) {
	if (root) {
		postOrdine(root->left);
		postOrdine(root->right);
		afisareRezervare(root->rezervare);
	}
}

Rezervare cautaRezervareaCuNrMaximCamere(TreeNode* root) {
	if (root == NULL) {
		return initRezervare(0, "???", 0, "???", "???");
	}

	Rezervare maxRezervare = root->rezervare;
	Rezervare maxLeft = cautaRezervareaCuNrMaximCamere(root->left); 
	Rezervare maxRight = cautaRezervareaCuNrMaximCamere(root->right);

	if (maxLeft.nrCamereRezervate > maxRezervare.nrCamereRezervate) {
		maxRezervare = maxLeft;
	}

	if (maxRight.nrCamereRezervate > maxRezervare.nrCamereRezervate) {
		maxRezervare = maxRight;
	}

	return maxRezervare;
}

void modificaNrCamereRezervare(TreeNode* root, unsigned int numarRezervare, unsigned char nrCamereNou) {
	if (root == NULL) {
		printf("\n\tArbore gol!\n");
		return;
	}

	if (root->rezervare.numarRezervare == numarRezervare) {
		root->rezervare.nrCamereRezervate = nrCamereNou;
		printf("\n\tRezervare modificata:\n");
		afisareRezervare(root->rezervare);
	}
	else if (root->rezervare.numarRezervare > numarRezervare) {
		modificaNrCamereRezervare(root->left, numarRezervare, nrCamereNou);
	}
	else {
		modificaNrCamereRezervare(root->right, numarRezervare, nrCamereNou);
	}
}

int numarNoduri(TreeNode* root) {
	if (root == NULL) {
		return 0;
	}
	return 1 + numarNoduri(root->left) + numarNoduri(root->right);
}

//void elibereazaRezervare(Rezervare* rezervare) {
//	free(rezervare->denumireHotel);
//	free(rezervare->numeClient);
//	free(rezervare->dataRezervare);
//}
//
//void elibereazaArbore(TreeNode* root) {
//	if (root) {
//		elibereazaArbore(root->left);
//		elibereazaArbore(root->right);
//		elibereazaRezervare(&root->rezervare);
//		free(root);
//	}
//}

int main() {
	Rezervare* rezervari;
	int nrRezervari;

	citesteRezervari("rezervari.txt", &rezervari, &nrRezervari);
	printf("\n----------Rezervari din fisier----------\n");
	for (int i = 0; i < nrRezervari; i++) {
		afisareRezervare(rezervari[i]);
	}

	// 1
	TreeNode* root = NULL; // pune null
	for (int i = 0; i < nrRezervari; i++) {
		insertInTreeNode(&root, rezervari[i]);
	}

	printf("\n----------Rezervari din ABC INORDINE----------\n");
	inOrdine(root); // aici crapa daca nu pui la definire = NULL;

	printf("\n----------Rezervari din ABC PREORDINE----------\n");
	preOrdine(root);

	printf("\n----------Rezervari din ABC POSTORDINE----------\n");
	postOrdine(root);

	// 2
	Rezervare rezervareNRMax = cautaRezervareaCuNrMaximCamere(root);
	printf("\n----------Rezervarea cu nr maxim de camere----------\n");
	afisareRezervare(rezervareNRMax);

	// 3
	unsigned int nrRezervare = 3;
	unsigned char nrCamereNou = 5;
	modificaNrCamereRezervare(root, nrRezervare, nrCamereNou);

	printf("\n----------Rezervari din ABC INORDINE----------\n");
	inOrdine(root);

	// 4
	int numarTotalNoduri = numarNoduri(root);
	printf("\n\tNumarul total de noduri din arbore: %d\n", numarTotalNoduri);

	/*for (int i = 0; i < nrRezervari; i++) {
		elibereazaRezervare(&rezervari[i]);
	}
	free(rezervari);

	elibereazaArbore(root);*/

	return 0;
}