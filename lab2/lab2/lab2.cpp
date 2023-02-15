#include <omp.h>
#include <iostream>
using namespace std;
const int NMAX = 18;
const int LIMIT = 10;
void main()
{
    setlocale(LC_ALL, "rus");
    int i, j;
    float sum;
    float a[NMAX][NMAX];

    for (i = 0; i < NMAX; i++)
        for (j = 0; j < NMAX; j++)
            a[i][j] = i + j;
    double __cdecl start = omp_get_wtime();
    #pragma omp parallel shared(a) if (NMAX>LIMIT)
    {
#pragma omp for private(i,j,sum) 
        for (i = 0; i < NMAX; i++)
        {
            sum = 0;
            for (j = 0; j < NMAX; j++)
                sum += a[i][j];
            printf("Сумма элементов строки %d равна %f\n", i, sum);
        }
    } /* Завершение параллельного фрагмента */
    double __cdecl end = omp_get_wtime();
    cout << "Total time " << end - start;

}


