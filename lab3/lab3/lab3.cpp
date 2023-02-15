#include <omp.h>
#include <iostream>
using namespace std;
const int N = 5000;
void main()
{
    setlocale(LC_ALL, "rus");
    srand(time(NULL));
    int *a = new int [N];
    int *b = new int [N];
    // инициализация данных
    for (int i = 0; i < N; i++) {
        a[i] = 1 + rand() % 10;
        b[i] = 1 + rand() % 10;
    }     
    int temp=0;
    long long int comp = 1;
    double __cdecl start = omp_get_wtime();
//#pragma omp parallel shared(a, b)
//    {
//#pragma omp for private(temp) reduction(*:comp)
        for (int i = 0; i < N; i++)
        {
            temp = a[i] + b[i];
            comp = comp*temp;
            //printf("Сумма элементов матрицы равна %d\n", temp);
            //cout << "Произведение равно: " << comp << endl;
        }
    //} /* Завершение параллельного фрагмента */
    double __cdecl end = omp_get_wtime();
    cout<<"Произведение равно: "<<comp<<endl;
    cout << "Total time " << end - start;
    delete[] a;
    delete[] b;
}


