#include <omp.h>
#include <iostream>
using namespace std;
const int N = 10000000;
void main()
{
    setlocale(LC_ALL, "rus");
    srand(time(NULL));
    int* a = new int[N];
    int* b = new int[N];
    // инициализация данных
    for (int i = 0; i < N; i++) {
        a[i] = 1 + rand() % 10;
        b[i] = 1 + rand() % 10;
    }

    int temp = 0, i;
    long int comp = 1;
    double __cdecl start = omp_get_wtime();
    double __cdecl end = omp_get_wtime();
    
    //10 sections
    omp_set_num_threads(50);
    start = omp_get_wtime();
    i = 0;
# pragma omp parallel sections shared(a, b) private(i) reduction(*:comp)
    {
        omp_get_num_threads();
#pragma omp section
        {
            for (i = 0; i < N / 10; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
#pragma omp section
        {
            for (i = N / 10; i < N/5; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
#pragma omp section
        {
            for (i = N / 5; i < 3*N / 10; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
#pragma omp section
        {
            for (i = 3*N / 10; i < 2*N/5; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
#pragma omp section
        {
            for (i = 2 * N / 5; i < N / 2; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
#pragma omp section
        {
            for (i = N / 2; i < 3 * N / 5; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
#pragma omp section
        {
            for (i = 3 * N / 5; i < 7 * N / 10; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
#pragma omp section
        {
            for (i = 7 * N / 10; i < 4 * N / 5; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
#pragma omp section
        {
            for (i = 4 * N / 5; i < 9 * N / 10; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
#pragma omp section
        {
            for (i = 9 * N / 10; i < N; i++)
            {
                comp = comp * (a[i] + b[i]);
            }
        }
    }
    end = omp_get_wtime();
    cout << "Total time 10 sections " << end - start << endl;
    delete[] a;
    delete[] b;
}
