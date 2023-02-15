//#define _CRT_SECURE_NO_WARNINGS
//using namespace std;
//#include <iostream>
//#include "mpi.h"
//#include <cstring>
//#include <vector>
//#include <algorithm>
//#define MAX_DIGITS 200
//
//void reverse(char* from, char* to) { //обращение строки char *
//    int len = strlen(from);
//    int l;
//    for (l = 0; l < len; l++) to[l] = from[len - l - 1];
//    to[len] = '\0';
//}
//
//void call_mult(char* first, char* second, char* result) {
//    char F[MAX_DIGITS], S[MAX_DIGITS], temp[MAX_DIGITS];
//    int f_len, s_len, f, s, r, t_len, hold, res;
//    f_len = strlen(first);
//    s_len = strlen(second);
//    reverse(first, F);
//    reverse(second, S);
//    t_len = f_len + s_len;
//    r = -1;
//    for (f = 0; f <= t_len; f++)temp[f] = '0';
//    temp[f] = '\0';
//    for (s = 0; s < s_len; s++) {
//        hold = 0;
//        for (f = 0; f < f_len; f++) {
//            res = (F[f] - '0') * (S[s] - '0') + hold + (temp[f + s] - '0');
//            temp[f + s] = res % 10 + '0';
//            hold = res / 10;
//            if (f + s > r) r = f + s;
//        }
//        while (hold != 0) {
//            res = hold + temp[f + s] - '0';
//            hold = res / 10;
//            temp[f + s] = res % 10 + '0';
//            if (r < f + s) r = f + s;
//            f++;
//        }
//    }
//    for (; r > 0 && temp[r] == '0'; r--);
//    temp[r + 1] = '\0';
//    reverse(temp, result);
//}
//
//
//int main(int argc, char* argv[])
//{
//    char TotalProd[MAX_DIGITS] = "1", ProcProd[MAX_DIGITS] = "1", res[MAX_DIGITS] = "1";
//    const int N = 100;
//    char ptrarray[N][3]; // создание динамического массива вещественных чисел на десять элементов
//    int ProcRank, ProcNum;
//
//    // инициализация 
//    MPI_Init(&argc, &argv);
//    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
//    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
//    MPI_Status Status;
//    MPI_Datatype LONG_INT;
//    MPI_Type_contiguous(MAX_DIGITS, MPI_INT, &LONG_INT);
//    MPI_Type_commit(&LONG_INT);
//
//    if (ProcRank == 0) {
//        // подготовка данных
//        srand(time(0));
//        int digit;
//        for (int count = 0; count < N; count++) {
//            digit = (rand() % 20 + 10); //заполнение массива случайными числами с масштабированием от 10 до 30
//            sprintf(ptrarray[count], "%ld", digit);
//        }
//        cout << "array = ";
//        for (int count = 0; count < N; count++)
//            cout << ptrarray[count] << "    ";
//        cout << endl;
//    }
//    MPI_Bcast(ptrarray, N, LONG_INT, 0, MPI_COMM_WORLD);
//    cout << "Send message to all ranks: " << endl;
//    // вычисление частичного произведения на каждом из процессов
//    // на каждом процессе умножаются элементы вектора x от i1 до i2
//    int k = N / ProcNum;
//    int i1 = k * ProcRank;
//    int i2 = k * (ProcRank + 1);
//    if (ProcRank == ProcNum - 1)
//        i2 = N;
//    for (int i = i1; i < i2; i++) {
//        //ProcProd = ProcProd * ptrarray[i];
//        call_mult(ProcProd, ptrarray[i], ProcProd);
//    }
//
//    // сборка частичных произведений на процессе с рангом 0
//    if (ProcRank == 0)
//    {
//        //TotalProd = ProcProd;
//        std::memcpy(TotalProd, ProcProd, sizeof(char) * MAX_DIGITS);
//        //std::copy_n(ProcProd, MAX_DIGITS, TotalProd);
//        for (int i = 1; i < ProcNum; i++)
//        {
//            MPI_Recv(&ProcProd, 1, LONG_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
//            cout << "Receive message from rank: " << MPI_ANY_SOURCE << endl;
//            //TotalProd = TotalProd * ProcProd;
//            call_mult(TotalProd, ProcProd, TotalProd);
//        }
//    }
//    else {
//        MPI_Send(&ProcProd, 1, LONG_INT, 0, 0, MPI_COMM_WORLD);
//        cout << "Send message to rank: " << 0 << endl;
//    } // все процессы отсылают свои частичные произведения
//
//    /*for (int count = 0; count < N; count++) {
//        call_mult(TotalProd, ptrarray[count], TotalProd);
//    }*/
//
//    if (ProcRank == 0) {
//        printf("\nTotal Multiplication = ");
//        for (int i = 0; i < MAX_DIGITS; i++) {
//            cout << TotalProd[i];
//        }
//    }
//    MPI_Type_free(&LONG_INT);
//    MPI_Finalize();
//}