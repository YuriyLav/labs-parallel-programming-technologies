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

    int temp = 0;
    long int comp = 1;
    double __cdecl start = omp_get_wtime();
    double __cdecl end = omp_get_wtime();

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

    //Замок
    temp = 0;
    comp = 1;
    omp_lock_t lock;
    omp_init_lock(&lock); // Инициализация замка
    start = omp_get_wtime();
#pragma omp parallel shared(temp, comp, a, b)
    {
#pragma omp for reduction(*:comp)
        for (int i = 0; i < N; i++)
        {
            omp_set_lock(&lock); // Устанавливаем замок
            temp = a[i] + b[i];
            comp = comp * temp;
            omp_unset_lock(&lock); // Освобождаем замок
        }
    }
    end = omp_get_wtime(); // Конец
    omp_destroy_lock(&lock); // Переводим замок в неиниц. состояние
    cout << "Total time lock " << end - start << endl;
    //cout << "Произведение равно: " << comp << endl;

    //Барьерная
    temp = 0;
    comp = 1;
    int n = omp_get_max_threads(); // создаем массив для всех потоков
    int* arr = new int[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = 1;
    }
    int ind = 0;
    start = omp_get_wtime();
#pragma omp parallel shared(temp, comp, a, b) 
    {
#pragma omp for private(ind) // для каждой нити записываем значение в массив

        for (int i = 0; i < N; i++)
        {
            ind = omp_get_thread_num();
            arr[ind] *= a[i] + b[i];
        }
#pragma omp single nowait

        // неявная барьерная синхронизация с помощью nowait и single
        for (int i = 0; i < n; i++)
        {
            comp *= arr[i];
        }
    }
    end = omp_get_wtime(); // Конец
    cout << "Total time barier " << end - start << endl;
    //cout << "Произведение равно: " << comp << endl;
}