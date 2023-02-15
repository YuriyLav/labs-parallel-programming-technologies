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

    //однопоток
    int temp = 0;
    long int comp = 1;
    double __cdecl start = omp_get_wtime();
        for (int i = 0; i < N; i++)
        {
            temp = a[i] + b[i];
            comp = comp * temp;
        }
    double __cdecl end = omp_get_wtime();
    //cout << "Произведение равно: " << comp << endl;
    cout << "Total time single " << end - start<<endl;

    //редукция
    temp = 0;
    comp = 1;
    start = omp_get_wtime();
#pragma omp parallel shared(temp, comp, a, b)
    {
#pragma omp for reduction(*:comp)
        for (int i = 0; i < N; i++)
        {
            temp = a[i] + b[i];
            comp = comp * temp;
        }
    } 
    end = omp_get_wtime();
    //cout << "Произведение равно: " << comp << endl;
    cout << "Total time reduction " << end - start << endl;

    //atomic
    temp = 0;
    comp = 1;
    start = omp_get_wtime();
#pragma omp parallel shared(temp, comp, a, b)
    {
#pragma omp for 
        for (int i = 0; i < N; i++)
        {
            temp = a[i] + b[i];
            #pragma omp atomic
            comp = comp * temp;
        }
    } 
    end = omp_get_wtime();
    //cout << "Произведение равно: " << comp << endl;
    cout << "Total time atomic " << end - start << endl;

    //critical
    temp = 0;
    comp = 1;
    start = omp_get_wtime();
#pragma omp parallel shared(temp, comp, a, b)
    {
#pragma omp for 
        for (int i = 0; i < N; i++)
        {
            temp = a[i] + b[i];
            #pragma omp critical
            comp = comp * temp;
        }
    } 
    end = omp_get_wtime();
    //cout << "Произведение равно: " << comp << endl;
    cout << "Total time critical " << end - start << endl;

    delete[] a;
    delete[] b;
}
