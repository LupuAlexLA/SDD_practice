#define _CRT_SECURE_NO_WARNINGS

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
//#include<malloc.h>

#define LINESIZE 128

typedef struct Masina Masina;
typedef struct Node Node;
typedef struct HT HT;

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
    Node* next;
};

struct HT {
    int size;
    Node** lists;
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

HT initHT(int size) {
    HT ht;
    ht.size = size;
    ht.lists = malloc(size * sizeof(Node*)); // !!!!!!!!!!!!!!!
    for(int i = 0; i < ht.size; i++) {
        ht.lists[i] = NULL;
    }
    return ht;
}

int hashFunction(int htSize, char* key) {
    int sum = 0;
    for(int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % htSize;
}

void inserareInLista(Node** head, Masina masina) {
    Node* newNode = malloc(sizeof(Node));
    newNode->masina = masina;
    newNode->next = NULL;
    if(*head) {
        Node* aux = *head;
        while(aux->next) {
            aux = aux->next;
        }
        aux->next = newNode;
    }
    else {
        *head = newNode;
    }
}

void inserareInListaFaraDuplicate(Node** head, Masina masina) {
    Node* newNode = malloc(sizeof(Node));
    newNode->masina = masina;
    newNode->next = NULL;
    if(*head) {
        Node* aux = *head;
        int diff = strcmp(aux->masina.model, masina.model);
        while(diff && aux->next) {
            aux = aux->next;
            diff = strcmp(aux->masina.model, masina.model);
        }

        if(diff == 0) {
            free(aux->masina.marca);
            free(aux->masina.model);
            free(newNode);
            aux->masina = masina;
        }
        else {
            aux->next = newNode;
        }
    }
    else {
        *head = newNode;
    }
}

void inserareInHTCuDuplicate(HT* ht, Masina masina) {
    int position = hashFunction(ht->size, masina.marca);
    inserareInLista(&ht->lists[position], masina);
}

void inserareInHTFaraDuplicate(HT* ht, Masina masina) {
    int position = hashFunction(ht->size, masina.marca);
    inserareInListaFaraDuplicate(&ht->lists[position], masina);
}

void afiseazaLista(Node* head) {
    if(head) {
        while(head) {
            afiseazaMasina(head->masina);
            head = head->next;
        }
    }
    else {
        printf("\n\tLista goala!\n");
    }
}

void afisareHT(HT ht) {
    for(int i = 0; i < ht.size; i++) {
        printf("\n\tPozitie: %d", i);
        afiseazaLista(ht.lists[i]);
    }
}

float cautaMotorulMax(HT ht, char* marca) {
    int position = hashFunction(ht.size, marca);

    Node* aux = ht.lists[position];
    float motorMax = 0;

    if(ht.lists[position]) {
        while(aux) {
            if(strcmp(aux->masina.marca, marca) == 0) {
                if(aux->masina.motor > motorMax) {
                    motorMax = aux->masina.motor;
                }
            }
            aux = aux->next;
        }

        return motorMax;
    }
    else {
        return 0;
    }
}

int calculeazaPretTotalMarca(HT ht, char* marca) {
    int position = hashFunction(ht.size, marca);
    Node* aux = ht.lists[position];
    int sum = 0;

    if(aux) {
        while(aux) {
            if (strcmp(aux->masina.marca, marca) == 0) {
                sum += aux->masina.pret;
                aux = aux->next;
            }
        }
        return sum;
    }
    else {
        printf("\n\tLista goala!\n");
        return 0;
    }
}

void actualizarePretMasina(HT* ht, int id, int pretNou) {
    for(int i = 0; i < ht->size; i++) {
        Node* aux = ht->lists[i];
        while(aux) {
            if(aux->masina.id == id) {
                aux->masina.pret = pretNou;
                break;
            }
            aux = aux->next;
        }
    }
}

void actualizareModelMasina(HT* ht, char* model, char* modelNou) {
    for(int i = 0; i < ht->size; i++) {
        Node* aux = ht->lists[i];
        while(aux) {
            if(strcmp(aux->masina.model, model) == 0) {
                free(aux->masina.model);
                aux->masina.model = malloc(strlen(modelNou) + 1);
                strcpy(aux->masina.model, modelNou);
            }
            aux = aux->next;
        }
    }
}

void stergeMasina(HT* ht, char* model) {
    for(int i = 0; i < ht->size; i++) {
        Node* current = ht->lists[i];
        Node* prev = NULL;
        while(current) {
            if(strcmp(current->masina.model, model) == 0) {
                if(prev == NULL) {
                    ht->lists[i] = current->next;
                }
                else {
                    prev->next = current->next;
                }
                free(current->masina.model);
                free(current->masina.marca);
                free(current);
                return;
            }
            prev = current;
            current = current->next;
        }
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

    HT ht = initHT(7);

    // 1.1
    for(int i = 0; i < nrMasini; i++) {
        inserareInHTCuDuplicate(&ht, masini[i]);
    }

    //1.2
    //for(int i = 0; i < nrMasini; i++) {
    //    inserareInHTFaraDuplicate(&ht, masini[i]);
    //}

    // 2
    printf("\n----------Masini din HT----------\n");
    afisareHT(ht);

    // 3
    char* marca = "Audi";
    float celMaiMareMotor = cautaMotorulMax(ht, marca);
    printf("\n\tPentru marca %s cel mai mare motor este %.2f\n", marca, celMaiMareMotor);

    // 4
    int pretTotalMarca = calculeazaPretTotalMarca(ht, marca);
    printf("\n\tPentru marca %s pretul total al masinilor este %d\n", marca, pretTotalMarca);

    // 5
    int id = 5;
    int pretNou = 5000;
    actualizarePretMasina(&ht, id, pretNou);

    printf("\n----------Masini din HT----------\n");
    afisareHT(ht);

    // 6
    char* model = "320";
    char* modelNou = "Seria 3";
    actualizareModelMasina(&ht, model, modelNou);

    printf("\n----------Masini din HT----------\n");
    afisareHT(ht);

    // 7
    model = "Leon";
    stergeMasina(&ht, model);

    printf("\n----------Masini din HT----------\n");
    afisareHT(ht);
}
