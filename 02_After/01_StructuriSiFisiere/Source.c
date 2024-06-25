#define _CRT_SECURE_NO_WARNINGS

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define LINESIZE 128

typedef struct Masina Masina;

struct Masina {
	unsigned int id;
	char* marca;
	char* model;
	float motor;
	int pret;
	unsigned char cai;
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

int main() {
	Masina* masini;
	int nrMasini;

	citesteMasiniDinFisier("masini.txt", &masini, &nrMasini);
	printf("\n----------Masini din fisier----------\n");
	for (int i = 0; i < nrMasini; i++) {
		afiseazaMasina(masini[i]);
	}

}
