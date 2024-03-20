#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LINESIZE 128

typedef struct Car Car;

struct Car
{
    int id;
    char* brand;
    char* name;
    float hp;
    int stagesNo;
    int* stages;
};

Car initCar(int id, char* brand, char* name, float hp, int stagesNo, int* stages)
{
    Car car;

    car.id = id;
    car.brand = (char*)malloc(strlen(brand) + 1);
    strcpy(car.brand, brand);
    car.name = (char*)malloc(strlen(name) + 1);
    strcpy(car.name, name);
    car.hp = hp;
    car.stagesNo = stagesNo;
    car.stages = (int*)malloc(stagesNo * sizeof(int));
    for(int i = 0; i < stagesNo; i++)
        car.stages[i] = stages[i];

    return car;
}

void printCar(Car car)
{
    printf("-----\nCar ID: %d\nCar Brand: %sCar Name: %sCar Hp: %.2f\nCar Maps: %d\n",
           car.id, car.brand, car.name, car.hp, car.stagesNo);
    for(int i = 0; i < car.stagesNo; i++)
        printf("Car Map %d: %d\n", i + 1, car.stages[i]);
    printf("-----\n");
}

Car readCarFromFile(FILE* f)
{
    char buffer[LINESIZE];
    char* endptr = NULL;
    Car car;

    fgets(buffer, LINESIZE, f);
    car.id = (int)strtol(buffer, &endptr, 10);

    fgets(buffer, LINESIZE, f);
    car.brand = (char*)malloc(strlen(buffer) + 1);
    strcpy(car.brand, buffer);

    fgets(buffer, LINESIZE, f);
    car.name = (char*)malloc(strlen(buffer) + 1);
    strcpy(car.name, buffer);

    fgets(buffer, LINESIZE, f);
    car.hp = (float)strtof(buffer, &endptr);

    fgets(buffer, LINESIZE, f);
    car.stagesNo = (int)strtol(buffer, &endptr, 10);

    car.stages = (int*)malloc(car.stagesNo * sizeof(int));
    for(int i = 0; i < car.stagesNo; i++)
    {
        fgets(buffer, LINESIZE, f);
        car.stages[i] = (int)strtol(buffer, &endptr, 10);
    }

    return car;
}

void readCarsFromFile(char* fileName, Car** carsArray, int* carsNo)
{
    FILE* f = fopen(fileName, "r");

    char buffer[LINESIZE];
    char* endptr = NULL;

    fgets(buffer, LINESIZE, f);
    *carsNo = (int)strtol(buffer, &endptr, 10);

    *carsArray = malloc((*carsNo) * sizeof(Car));

    for(int i = 0; i < *carsNo; i++) {
        (*carsArray)[i] = readCarFromFile(f);
    }
}

int main()
{
    int carsNo;
    Car* carsArray;

    readCarsFromFile("cars.txt", &carsArray, &carsNo);

    for(int i = 0; i < carsNo; i++) {
        printCar(carsArray[i]);
    }
}