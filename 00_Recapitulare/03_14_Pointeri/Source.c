#include<stdio.h>
#include<stdlib.h>

#define VECTOR_SIZE 10

// Deodre
/*suma(int a, int b, int* nr)
{
    *nr = a + b;
}*/

// Vasi
/*void swap(int* a, int* b)
{
    int aux = *a;
    *a = *b;
    *b = aux;
}

void citireTastatura(int* dim, int** v)
{
    printf("Dim = ");
    scanf("%d", dim);
    *v = (int*)malloc(sizeof(int) * (*dim));
    for(int i = 0; i < (*dim); i++)
    {
        printf("v[%d] = ", i);
        //scanf("%d", &(v + i)) // scrisa de mine - greseala
        scanf("%d", *v + i);
    }
}*/

int main()
{
    // by my self
    /*int a = 3;
    int* pa = &a;
    printf("a = %d; *pa = %d\n", a, *pa);
    printf("pa -> %p\n", pa);

    int v[3] = {1, 2, 3};
    int* pv = &v;
    printf("v = %p; *pv = %d\n", v, *pv);
    printf("pv -> %p\n", pv);
    printf("v -> %p\n", v);*/

    // Deodre
    /*int x;
    suma(10, 15, &x);
    printf("x = %d\n", x);*/

    // Vasi
    /*char* ss = malloc(sizeof(char) * 5);
    for(int i = 0; i < 4; i++)
        ss[i] = 97 + i;
    ss[4] = 0;
    printf("ss : %s\n", ss);

    int x = 2, y = 3;
    swap(&x, &y);
    printf("x = %d; y = %d\n", x, y);

    int dim;
    int* v;
    citireTastatura(&dim, &v);

    printf("\nVectorul:\n");
    for(int i = 0; i < dim; i++)
        printf("v[%d] = %d\n", i, v[i]);*/

    // Zamfi
    // voi face la sem 2 si sem1 pt ca e cu structuri

    // Book
    unsigned char x = 103;
    unsigned char* px = NULL;

    px = &x;

    printf("&px : %p\n", &px);
    printf("px  : %p\n", px);
    printf("*px : %u\n", *px);
    printf("&x  : %p\n", &x);
    printf("x   : %u\n", x);


}