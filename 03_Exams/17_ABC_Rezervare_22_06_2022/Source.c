#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>

#define LINESIZE 128

typedef struct Rezervare Rezervare;
typedef struct TreeNode TreeNode;

struct Rezervare {
    unsigned int idRezervare;
    char* denumireHotel;
    unsigned char nrCamereRezervate;
    char* numeClient;
    float suma;
};

struct TreeNode {
    Rezervare rezervare;
    TreeNode* left;
    TreeNode* right;
};

Rezervare initRezervare(unsigned int numarRezervare, char* denumireHotel, unsigned char nrCamereRezervate, char* numeClient, float suma) {
    Rezervare rezervare;

    rezervare.idRezervare = numarRezervare;
    rezervare.denumireHotel = malloc(strlen(denumireHotel) + 1);
    strcpy(rezervare.denumireHotel, denumireHotel);
    rezervare.nrCamereRezervate = nrCamereRezervate;
    rezervare.numeClient = malloc(strlen(numeClient) + 1);
    strcpy(rezervare.numeClient, numeClient);
    rezervare.suma = suma;

    return rezervare;
}

void afisareRezervare(Rezervare rezervare) {
    printf("\n-----\nID rezervare: %d\nHotel: %s\nNr camere rezervare: %d\nClient: %s\nSuma: %.2f\n-----\n",
           rezervare.idRezervare, rezervare.denumireHotel, rezervare.nrCamereRezervate, rezervare.numeClient, rezervare.suma);
}

void afisareRezervareSimpla(Rezervare rezervare) {
    printf("\n-----\nID rezervare: %d\nClient: %s\n-----\n",
           rezervare.idRezervare, rezervare.numeClient);
}

Rezervare citesteRezervare(FILE* f) {
    Rezervare rezervare;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    rezervare.idRezervare = (unsigned int)strtol(buffer, NULL, 10);

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
    rezervare.suma = strtof(buffer, NULL);

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

void inserareInArbore(TreeNode** root, Rezervare rezervare) {
    if (*root) {
        if ((*root)->rezervare.idRezervare > rezervare.idRezervare) {
            inserareInArbore(&(*root)->left, rezervare);
        }
        else {
            inserareInArbore(&(*root)->right, rezervare);
        }
    }
    else {
        TreeNode* newNode = malloc(sizeof(TreeNode));
        newNode->rezervare = rezervare;
        newNode->left = NULL;
        newNode->right = NULL;
        (*root) = newNode;
    }
}

void inOrdine(TreeNode* root) {
    if (root) {
        inOrdine(root->left);
        afisareRezervareSimpla(root->rezervare);
        inOrdine(root->right);
    }
}

void preOrdine(TreeNode* root) {
    if (root) {
        afisareRezervareSimpla(root->rezervare);
        preOrdine(root->left);
        preOrdine(root->right);
    }
}

void postOrdine(TreeNode* root) {
    if (root) {
        postOrdine(root->left);
        postOrdine(root->right);
        afisareRezervareSimpla(root->rezervare);
    }
}

float sumaTotala(TreeNode* root, char* client) {
    if (!root) {
        return 0;
    }
    float sum = 0;
    if (strcmp(root->rezervare.numeClient, client) == 0) {
        sum += root->rezervare.suma;
    }
    return sum
           + sumaTotala(root->left, client)
           + sumaTotala(root->right, client);
}

void stergeNodCheieMinima(TreeNode** root) {
    if (*root == NULL) {
        return;
    }

    TreeNode* parrent = NULL;
    TreeNode* current = *root;

    while (current->left) {
        parrent = current;
        current = current->left;
    }

    if (current->right != NULL) {
        //parrent->left = current->right; // nu i de ajuns
        if (parrent != NULL) {
            parrent->left = current->right;
        }
        else {
            *root = current->right;
        }
    }
    else {
        if (parrent != NULL) {
            parrent->left = NULL;
        }
        else {
            *root = NULL;
        }
    }

    free(current->rezervare.denumireHotel);
    free(current->rezervare.numeClient);
    free(current);
}

int main() {
    Rezervare* rezervari;
    int nrRezervari;

    citesteRezervari("rezervari.txt", &rezervari, &nrRezervari);
    printf("\n----------Rezervari din fisier----------\n");
    for (int i = 0; i < nrRezervari; i++) {
        afisareRezervare(rezervari[i]);
    }

    // 1
    TreeNode* root = NULL;
    inserareInArbore(&root, rezervari[2]);
    inserareInArbore(&root, rezervari[1]);
    inserareInArbore(&root, rezervari[0]);
    inserareInArbore(&root, rezervari[3]);
    inserareInArbore(&root, rezervari[4]);

    // 2
    printf("\n----------Rezervari din ABC INORDINE----------\n");
    inOrdine(root);
    printf("\n----------Rezervari din ABC PREORDINE----------\n");
    preOrdine(root);
    printf("\n----------Rezervari din ABC POSTORDINE----------\n");
    postOrdine(root);

    // 3
    char* client = "Florin Tufaru";
    float sumaTotalaClient = sumaTotala(root, client);
    printf("\n\tClientul %s a cheltuit in total %.2f\n\t", client, sumaTotalaClient);

    // 4
    stergeNodCheieMinima(&root);
    printf("\n----------Rezervari din ABC PREORDINE----------\n");
    preOrdine(root);

    stergeNodCheieMinima(&root);
    printf("\n----------Rezervari din ABC PREORDINE----------\n");
    preOrdine(root);
}