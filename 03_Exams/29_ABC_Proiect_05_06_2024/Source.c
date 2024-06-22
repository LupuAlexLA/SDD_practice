#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Proiect Proiect;
typedef struct TreeNode TreeNode;

struct Proiect {
    unsigned int id;
    char* titlu;
    char* contractant;
    unsigned char nrZile;
    float buget;
    float stadiu;
};

struct TreeNode {
    Proiect proiect;
    TreeNode* left;
    TreeNode* right;
};

void afiseazaProiect(Proiect proiect) {
    printf("\n-----\nId: %d\nTitlu: %s\nContractant: %s\nNumar zile: %d\nBuget: %.2f\nStadiu: %.2f\n-----\n",
           proiect.id, proiect.titlu, proiect.contractant, proiect.nrZile, proiect.buget, proiect.stadiu);
}

Proiect citesteProiect(FILE* f) {
    Proiect proiect;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    proiect.id = (int)strtol(buffer, NULL, 10);
    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    proiect.titlu = malloc(strlen(buffer) + 1);
    strcpy(proiect.titlu, buffer);
    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    proiect.contractant = malloc(strlen(buffer) + 1);
    strcpy(proiect.contractant, buffer);
    fgets(buffer, LINESIZE, f);
    proiect.nrZile = (int)strtol(buffer, NULL, 10); // de vazut
    fgets(buffer, LINESIZE, f);
    proiect.buget = strtof(buffer, NULL);
    fgets(buffer, LINESIZE, f);
    proiect.stadiu = strtof(buffer, NULL);

    return proiect;
}

void citesteProiecteDinFisier(char* numeFisier, Proiect** proiecte, int* nrProiecte) {
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

void insertInTreeNode(TreeNode** root, Proiect proiect) {
    if (*root) {
        if ((*root)->proiect.id > proiect.id) {
            insertInTreeNode(&(*root)->left, proiect);
        }
        else {
            insertInTreeNode(&(*root)->right, proiect);
        }
    }
    else {
        TreeNode* newNode = malloc(sizeof(TreeNode));
        newNode->proiect = proiect;
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
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

int nrProiecteCuContractant(TreeNode* root, char* contractant) {
    if (!root) {
        return 0;
    }

    int count = 0;

    if (strcmp(root->proiect.contractant, contractant) == 0) {
        count = 1;
    }

    return count
           + nrProiecteCuContractant(root->left, contractant)
           + nrProiecteCuContractant(root->right, contractant);
}

void modificaStadiuProiect(TreeNode** root, int id, float stadiuNou) {
    if (*root) {
        if ((*root)->proiect.id == id) {
            (*root)->proiect.stadiu = stadiuNou;
            return;
        }
        else if ((*root)->proiect.id > id) {
            modificaStadiuProiect(&(*root)->left, id, stadiuNou);
        }
        else {
            modificaStadiuProiect(&(*root)->right, id, stadiuNou);
        }
    }
}

TreeNode* stergeFrunze(TreeNode* root) {
    if (root == NULL) {
        return NULL;
    }

    // Dacă este frunză, îl ștergem
    if (root->left == NULL && root->right == NULL) {
        free(root->proiect.titlu);
        free(root->proiect.contractant);
        free(root);
        return NULL;
    }

    // Recursiv pentru copii
    root->left = stergeFrunze(root->left);
    root->right = stergeFrunze(root->right);

    return root;
}

int main() {
    Proiect* proiecte;
    int nrProiecte;

    citesteProiecteDinFisier("proiecte.txt", &proiecte, &nrProiecte);

    printf("\n----------Proiecte din fisier----------\n");
    for (int i = 0; i < nrProiecte; i++) {
        afiseazaProiect(proiecte[i]);
    }

    // 1

    TreeNode* root = NULL;
    insertInTreeNode(&root, proiecte[2]);
    insertInTreeNode(&root, proiecte[1]);
    insertInTreeNode(&root, proiecte[0]);
    insertInTreeNode(&root, proiecte[3]);
    insertInTreeNode(&root, proiecte[4]);

    /*TreeNode* root = NULL;
    for (int i = 0; i < nrProiecte; i++) {
        insertInTreeNode(&root, proiecte[i]);
    }*/

    printf("\n----------Proiecte din ABC INORDINE----------\n");
    inOrdine(root);

    printf("\n----------Proiecte din ABC PREORDINE----------\n");
    preOrdine(root);

    printf("\n----------Proiecte din ABC POSTORDINE----------\n");
    postOrdine(root);

    // 2
    char* contractant = "SIF Muntenia";
    int nrProiecteContractant = nrProiecteCuContractant(root, contractant);
    printf("\n\tNumarul de proiecte cu contractantul %s : %d\n", contractant, nrProiecteContractant);

    // 3
    int idProiect = 3;
    float stadiuNou = 0.75;
    modificaStadiuProiect(&root, idProiect, stadiuNou);

    printf("\n----------Proiecte din ABC INORDINE dupa actualizare----------\n");
    inOrdine(root);

    // 4
    root = stergeFrunze(root);

    printf("\n----------Proiecte din ABC INORDINE dupa stergere frunze----------\n");
    inOrdine(root);
    printf("\n");
}