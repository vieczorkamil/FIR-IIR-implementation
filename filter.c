/* 
 * @AUTOR Kamil Wieczorek
 */

#include <stdio.h>
#include <stdlib.h>
#include "params.h"
#include <string.h>
#include <math.h>
#include <time.h>

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

double buffer_filter1[ARRAY_SIZE(b_iir)] = {0}; //size b_iir = size a_irr
double output_filter1[ARRAY_SIZE(b_iir)] = {0};
int buffer_filter1_fixed[ARRAY_SIZE(b_iir)] = {0}; //size b_iir = size a_irr
int output_filter1_fixed[ARRAY_SIZE(b_iir)] = {0};
int offset_filter1 = 0;
double buffer_filter2[ARRAY_SIZE(b_fir)] = {0};
int buffer_filter2_fixed[ARRAY_SIZE(b_fir)] = {0};
int offset_filter2 = 0;
char f1[] = "IIR.txt";
char f2[] = "FIR.txt";
char f3[] = "fixed_point_IIR.txt";
char f4[] = "fixed_point_FIR.txt";

// in Linux/UNIX system use drand48()
double giveRandom()
{
    double a = 1.0;
    srand((unsigned int)time(NULL));
    return (((double)rand() / (double)(RAND_MAX)) * a);
}

// write raw filtered data in a file
void writeRawData(char *path, int data)
{
    FILE *fd = fopen(path, "a");
    fprintf(fd, "%d\n", data);
    fclose(fd);
}

static double iir(int x)
{

    double *b = b_iir;
    double *a = a_iir;

    double *value = buffer_filter1 + offset_filter1;
    double *output = output_filter1 + offset_filter1;
    double out = 0;

    *value = (double)x;
    *output = 0;

    for (int i = offset_filter1; i >= 0; i--)
    {
        out += (*value-- * *b++) - (*output-- * *a++);
    }

    value = buffer_filter1 + ARRAY_SIZE(b_iir) - 1;  //przesunicie na koniec
    output = output_filter1 + ARRAY_SIZE(b_iir) - 1; //przesunicie na koniec

    for (int i = offset_filter1; i < ARRAY_SIZE(b_iir); i++)
    {
        out += (*value-- * *b++) - (*output-- * *a++);
    }

    output = output_filter1 + offset_filter1;
    *output = out; //wpisuje wyjscie

    offset_filter1++;
    if (offset_filter1 >= ARRAY_SIZE(b_iir))
    {
        offset_filter1 = 0;
    }

    return out;
}

static int iir_fixed(int x)
{
    //to do
}

static double fir(int x)
{
    double *b = b_fir;
    double *value = buffer_filter2 + offset_filter2;
    double out = 0;

    *value = (double)x;
    for (int i = offset_filter2; i >= 0; i--)
    {
        out += *value-- * *b++;
    }
    value = buffer_filter2 + ARRAY_SIZE(b_fir) - 1; //przesuniecie na koniec bufora
    for (int i = offset_filter2; i < ARRAY_SIZE(b_fir); i++)
    {
        out += *value-- * *b++;
    }
    offset_filter2++;
    if (offset_filter2 >= ARRAY_SIZE(b_fir))
    {
        offset_filter2 = 0;
    }
    return out;
}

static int fir_fixed(int x)
{
    int *b; //mamy mieć tą poniższą zamianę zrobić w matlabie
    b = (int *)malloc(sizeof(int) * ARRAY_SIZE(b_fir));
    int q = 20;
    for (int i = 0; i < ARRAY_SIZE(b_fir); i++)
    {
        b[i] = (int)round(b_fir[i] * pow(2, q));
    }
    int *value = buffer_filter2_fixed + offset_filter2;
    long long out = 0;

    *value = x;
    for (int i = offset_filter2; i >= 0; i--)
    {
        out += *value-- * *b++;
    }
    value = buffer_filter2_fixed + ARRAY_SIZE(b_fir) - 1; //przesuniecie na koniec bufora
    for (int i = offset_filter2; i < ARRAY_SIZE(b_fir); i++)
    {
        out += *value-- * *b++;
    }
    offset_filter2++;
    if (offset_filter2 >= ARRAY_SIZE(b_fir))
    {
        offset_filter2 = 0;
    }
    x = out >> q;
    return x;
}

static void filter1(int x)
{
    x = (int)round(iir(x) - giveRandom() + giveRandom());
    printf("%d\n", x);
    writeRawData(f1, x);
}

static void filter2(int x)
{
    x = (int)round(fir(x) - giveRandom() + giveRandom());
    printf("%d\n", x);
    writeRawData(f2, x);
}

static void filter3(int x)
{
    x = (int)round(iir_fixed(x) - giveRandom() + giveRandom());
    printf("%d\n", x);
    writeRawData(f3, x);
}

static void filter4(int x)
{
    x = (int)round(fir_fixed(x) - giveRandom() + giveRandom());
    printf("%d\n", x);
    writeRawData(f4, x);
}

int main(int argc, char **argv)
{
    void (*f)(int);
    int x;
    FILE *fd;

    if (argc < 2)
    {
        printf("usage: filter FILTER\n");
        printf("\n");
        printf("Filters:\n");
        printf("        f1 - IIR\n");
        printf("        f2 - FIR\n");
        printf("        f3 - fixed-point IIR\n");
        printf("        f4 - fixed-point FIR\n");
        exit(1);
    }
    if (!strcmp(argv[1], "f1"))
    {
        f = filter1;
        if (fd = fopen(f1, "r"))
        {
            fclose(fd);
            if (remove(f1))
            {
                exit(1);
            }
        }
    }
    else if (!strcmp(argv[1], "f2"))
    {
        f = filter2;
        if (fd = fopen(f2, "r"))
        {
            fclose(fd);
            if (remove(f2))
            {
                exit(1);
            }
        }
    }
    else if (!strcmp(argv[1], "f3"))
    {
        f = filter3;
        if (fd = fopen(f3, "r"))
        {
            fclose(fd);
            if (remove(f3))
            {
                exit(1);
            }
        }
    }
    else if (!strcmp(argv[1], "f4"))
    {
        f = filter4;
        if (fd = fopen(f4, "r"))
        {
            fclose(fd);
            if (remove(f4))
            {
                exit(1);
            }
        }
    }
    else
        exit(1);

    while (!feof(stdin))
    {
        if (scanf("%d", &x) < 1)
            continue;
        f(x);
    }
    return 0;
}