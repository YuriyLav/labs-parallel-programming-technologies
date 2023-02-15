#include <iostream>
#include "mpi.h"
#include <ctime>
#include <vector>
using namespace std;

int* multiply(int A[], int B[], int m, int n) {
    int* res = new int[m + n - 1];
    for (int i = 0; i < m + n - 1; i++) {
        res[i] = 0;
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            res[i + j] += A[i] * B[j];
        }
    }
    return res;
}

void print(int polynomial[], int n) {
    for (int i = n - 1; i >= 0; i--) {
        cout << polynomial[i];
        if (i != 0) {
            cout << "x^" << i;
            cout << " + ";
        }
    }
    cout << endl;
}

int main(int* argc, char** argv) {
    int A = 10; //количество полиномов
    int degree = 3; //степень полинома
    int maxDegree = A * (degree - 1) + 1; //максимальная степень полинома
    int a = 0; 
    int b = 15;
    int numProc, rankProc;
    int counterProc = 0; //количество требуемых процессов
    MPI_Init(argc, &argv);
    MPI_Datatype MPI_POLYNOMIAL;
    MPI_Type_contiguous(maxDegree, MPI_INT, &MPI_POLYNOMIAL);
    MPI_Type_commit(&MPI_POLYNOMIAL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    if (rankProc == 0) {
        srand(time(NULL));
        vector<int*>polynomials;
        //инициализация полиномов
        for (int i = 0; i < A; i++) {
            polynomials.push_back(new int[maxDegree]);
            for (int j = 0; j < maxDegree; j++) {
                if (j < degree)
                    polynomials[i][j] = a + rand() % (b - a);
                else polynomials[i][j] = 0;
            }
        }
        //вывод полиномов
        cout << "Polynomials: " << endl;
        for (int i = 0; i < A; i++) {
            print(polynomials[i], degree);
        }
        while (true) {
            counterProc = 0;
            //отправляем полиномы другим процессам
            for (int i = 1; i < numProc; i++) {
                if (polynomials.size() > 1) {
                    counterProc++;
                    MPI_Send(polynomials[0], 1, MPI_POLYNOMIAL, i, 0, MPI_COMM_WORLD);
                    MPI_Send(polynomials[1], 1, MPI_POLYNOMIAL, i, 0, MPI_COMM_WORLD);
                    polynomials.erase(polynomials.begin(), polynomials.begin() + 2);
                }
            }
            //отправляем число задействованных процессов
            MPI_Bcast(&counterProc, 1, MPI_INT, 0, MPI_COMM_WORLD);
            //получаем результаты вычислений
            for (int i = 0; i < counterProc; i++) {
                int* buf = new int[maxDegree];
                MPI_Recv(buf, 1, MPI_POLYNOMIAL, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                polynomials.push_back(buf);
            }
            //если доступен только один полином, завершаем главный процесс
            if (polynomials.size() == 1) {
                counterProc = 0;
                MPI_Bcast(&counterProc, 1, MPI_INT, 0, MPI_COMM_WORLD);
                cout << "\nResult: ";
                print(polynomials[0], maxDegree);
                cout << "\n";
                break;
            }
        }
        //cout << "end:" << rankProc << endl;
    }
    else {
        while (true) {
            //получаем число задействованных процессов 
            MPI_Bcast(&counterProc, 1, MPI_INT, 0, MPI_COMM_WORLD);
            if (rankProc < counterProc + 1) {
                //cout << "rank:" << rankProc << " " << "counterProc:" << counterProc << endl;
                int* polynomial_1 = new int[maxDegree];
                int* polynomial_2 = new int[maxDegree];
                MPI_Recv(polynomial_1, 1, MPI_POLYNOMIAL, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                MPI_Recv(polynomial_2, 1, MPI_POLYNOMIAL, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                int* result = multiply(polynomial_1, polynomial_2, maxDegree, maxDegree);
                MPI_Send(result, 1, MPI_POLYNOMIAL, 0, 0, MPI_COMM_WORLD);
            }
            else break;
        }
        //cout << "end:" << rankProc << endl;
    }
    MPI_Type_free(&MPI_POLYNOMIAL);
    MPI_Finalize();
    return 0;
}