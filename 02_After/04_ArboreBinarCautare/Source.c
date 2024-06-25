#define _CRT_SECURE_NO_WARNINGS

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Masina Masina;
typedef struct TreeNode TreeNode;

struct Masina {
	unsigned int id;
	char* marca;
	char* model;
	float motor;
	int pret;
	unsigned char cai;
};

struct TreeNode {
    Masina masina;
    TreeNode* left;
    TreeNode* right;
};

Masina initMasina(unsigned int id, char* marca, char* model, float motor, int pret, unsigned char cai)
{
    Masina masina;

    masina.id = id;
    masina.marca = malloc(strlen(marca) + 1);
    strcpy(masina.marca, marca);
    masina.model = malloc(strlen(model) + 1);
    strcpy(masina.model, model);
    masina.motor = motor;
    masina.pret = pret;
    masina.cai = cai;

    return masina;
}

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

void inserareInArbore(TreeNode** root, Masina masina) {
    if(*root) {
        if((*root)->masina.id > masina.id) {
            inserareInArbore(&(*root)->left, masina);
        }
        else {
            inserareInArbore(&(*root)->right, masina);
        }
    }
    else {
        TreeNode* newNode = malloc(sizeof(TreeNode));
        newNode->masina = masina;
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
    }
}

int cautaMasiniAcelasiModel(TreeNode* root, char* model) {
    if(root) {
        int count = 0;
        if(strcmp(root->masina.model, model) == 0) {
            count = 1;
        }

        return count
        + cautaMasiniAcelasiModel(root->left, model)
        + cautaMasiniAcelasiModel(root->right, model);
    }
    else {
        return 0;
    }
}

Masina cautaMasinaCuNrMaxCai(TreeNode* root) {
    if(root == NULL) {
        return initMasina(0, "???", "???", 0.0, 0, 0);
    }

    Masina maxCai = root->masina;
    Masina maxLeft = cautaMasinaCuNrMaxCai(root->left);
    Masina maxRight = cautaMasinaCuNrMaxCai(root->right);

    if(maxLeft.cai > maxCai.cai) {
        maxCai = maxLeft;
    }

    if(maxRight.cai > maxCai.cai) {
        maxCai = maxRight;
    }

    return maxCai;
}

void inOrdine(TreeNode* root) {
    if(root) {
        inOrdine(root->left);
        afiseazaMasina(root->masina);
        inOrdine(root->right);
    }
}

void preOrdine(TreeNode* root) {
    if(root) {
        afiseazaMasina(root->masina);
        preOrdine(root->left);
        preOrdine(root->right);
    }
}

void postOrdine(TreeNode* root) {
    if(root) {
        postOrdine(root->left);
        postOrdine(root->right);
        afiseazaMasina(root->masina);
    }
}

void schimbaMotor(TreeNode** root, int id, float motor) {
    if(*root) {
        if((*root)->masina.id == id) {
            (*root)->masina.motor = motor;
            return;
        }
        else if((*root)->masina.id > id) {
            schimbaMotor(&(*root)->left, id, motor);
        }
        else {
            schimbaMotor(&(*root)->right, id, motor);
        }
    }
    else {
        printf("\n\tArbore gol!\n");
    }
}

int calculeazaPretTotalAceeasiMarca(TreeNode* root, char* marca) {
    if(root) {
        int sum = 0;
        if(strcmp(root->masina.marca, marca) == 0) {
            sum = root->masina.pret;
        }

        return sum
            + calculeazaPretTotalAceeasiMarca(root->left, marca)
            + calculeazaPretTotalAceeasiMarca(root->right, marca);
    }
    else {
        return 0;
    }
}

TreeNode* stergeNoduriFrunza(TreeNode** root) {
    if(*root == NULL) {
        return NULL;
    }
    if((*root)->left == NULL && (*root)->right == NULL) {
        free((*root)->masina.marca);
        free((*root)->masina.model);
        free(*root);
        return NULL;
    }

    (*root)->left = stergeNoduriFrunza(&(*root)->left);
    (*root)->right = stergeNoduriFrunza(&(*root)->right);

    return *root;
}

int aflaNumarNoduri(TreeNode* root) {
    if(root == NULL) {
        return 0;
    }
    return 1
        + aflaNumarNoduri(root->left)
        + aflaNumarNoduri(root->right);
}

int aflaNumarNoduriFrunza(TreeNode* root) {
    if(!root) {
        return 0;
    }

    if(root->left == NULL && root->right == NULL) {
        return 1;
    }

    return aflaNumarNoduriFrunza(root->left) + aflaNumarNoduriFrunza(root->right);
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

    free(current->masina.marca);
    free(current->masina.model);
    free(current);
}

int main() {
	Masina* masini;
	int nrMasini;

	citesteMasiniDinFisier("masini.txt", &masini, &nrMasini);
	printf("\n----------Masini din fisier----------\n");
	for (int i = 0; i < nrMasini; i++) {
		afiseazaMasina(masini[i]);
	}

    TreeNode* root = NULL;

    // 1
    for(int i = 0; i < nrMasini; i++) {
        inserareInArbore(&root, masini[i]);
    }

    // 2
    char* model = "A3";
    int nrMasiniAcelasiModel = cautaMasiniAcelasiModel(root, model);
    printf("\n\tExista %d masini cu modelul %s\n", nrMasiniAcelasiModel, model);

    // 3
    Masina masinaPuternica = cautaMasinaCuNrMaxCai(root);
    printf("\n\tMasina cu cei mai multi cai este:\n");
    afiseazaMasina(masinaPuternica);

    // 4
    printf("\n----------Masini din ABC INORDINE----------\n");
    inOrdine(root);

    printf("\n----------Masini din ABC PREORDINE----------\n");
    preOrdine(root);

    printf("\n----------Masini din ABC POSTORDINE----------\n");
    postOrdine(root);

    // 5
    int id = 7;
    float motor = 3.0;
    schimbaMotor(&root, id, motor);

    printf("\n----------Masini din ABC INORDINE----------\n");
    inOrdine(root);

    // 6
    char* marca = "Audi";
    int pretTotal = calculeazaPretTotalAceeasiMarca(root, marca);
    printf("\n\tPretul total al masinilor cu marca %s este %d\n", marca, pretTotal);

    // 7
    root = stergeNoduriFrunza(&root);
    printf("\n----------Masini din ABC INORDINE dupa STERGERE FRUNZE----------\n");
    inOrdine(root);

    // 8
    int noduri = aflaNumarNoduri(root);
    printf("\n\tNoduri %d\n", noduri);

    // 9
    int noduriFrunza = aflaNumarNoduriFrunza(root);
    printf("\n\tNoduri frunza %d\n", noduriFrunza);

    // 10
    stergeNodCheieMinima(&root);
    printf("\n----------Masini din ABC INORDINE dupa STERGERE NOD CHEIE MINIMA----------\n");
    inOrdine(root);
}
