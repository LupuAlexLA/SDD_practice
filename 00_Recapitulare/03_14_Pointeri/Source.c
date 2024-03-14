#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
    printf("Pointer la char\n");
    unsigned char x = 103;
    unsigned char* px = NULL;

    px = &x;

    printf("&px : %p\n", &px);
    printf("px  : %p\n", px);
    printf("*px : %u\n", *px);
    printf("&x  : %p\n", &x);
    printf("x   : %u\n", x);

    printf("\n");
    *px = x + 3;

    printf("&px : %p\n", &px);
    printf("px  : %p\n", px);
    printf("*px : %u\n", *px);
    printf("&x  : %p\n", &x);
    printf("x   : %u\n", x);

    printf("\nPointer constant la int constant\n");

    int vint = 13;
    int j = 9;
    // ptr const la int
    int* const pci = &vint; // initializare oblig la declarare
    // pci = &j; // nu i se poate schimba adresa

    printf("&pci : %p\n", &pci);
    printf("pci  : %p\n", pci);
    printf("*pci : %u\n", *pci);
    printf("&vint: %p\n", &vint);
    printf("vint : %u\n", vint);

    printf("\nPointer la int constant\n");

    // ptr la int const
    int const* pint;
    pint = &vint;
    //*pint = vint + 5; // nu i se poate schimba valoarea

    printf("&pint : %p\n", &pint);
    printf("pint  : %p\n", pint);
    printf("*pint : %u\n", *pint);
    printf("&vint : %p\n", &vint);
    printf("vint  : %u\n", vint);

    printf("\nPointer constant la int constant");

    // ptr const la int const
    const int* const pint2 = &vint;
    // nu i se poate schimba nici valoare nici adresa
    //pint2 = &j;
    //*pint = vint + 5;

    printf("&pint2 : %p\n", &pint2);
    printf("pint2  : %p\n", pint2);
    printf("*pint2 : %u\n", *pint2);
    printf("&vint  : %p\n", &vint);
    printf("vint   : %u\n", vint);

    printf("\nPointer la vector de char\n");

    unsigned char vx[VECTOR_SIZE];
    px = vx;

    for(int i = 0; i < sizeof(vx); i++)
        px[i] = x + i; // *(px + i)

    printf("&px : %p\n", &px);
    printf("px  : %p\n", px);
    printf("&vx : %p\n", &vx);

    for(unsigned char i = 0; i < sizeof(vx); i++)
        printf("vx[%d] : adresa = px + i : %p; continut = px[i] = %d\n", i + 1, px + i, px[i]);

    printf("\nPointeri la stringuri\n");

    char str[] = "Structuri de date";
    unsigned char* ppc = (unsigned char*)str;

    printf("&ppc  : %p\n", &ppc);
    printf("ppc   : %p\n", ppc);
    printf("&str  : %p\n", &str);
    printf("Dim = %u elem\nLungime de %d carcatere\ncontinut str/ppc = %s\n", sizeof(str), strlen(str), str);

}