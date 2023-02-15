//using namespace std;
//#include <iostream>
//#include "mpi.h"
//char message[] = "message";
//char message1[] = "message1";
//void star(int ProcRank, int ProcNum) {
//// 0 1 2 3 4 5 6 7 8 9 10 11
//    MPI_Bcast(message, _countof(message), MPI_CHAR, 0, MPI_COMM_WORLD);
//    cout << "0 rank Send message to: " << ProcRank << " rank"<<endl;
//    MPI_Reduce(message1, message, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
//    cout << "Send message to 0 rank by: " << ProcRank << endl;
//}
//int main(int argc, char* argv[])
//{
//    double t1, t2, dt=0;
//    int ProcNum, ProcRank;
//    int recvFrom = 0;
//    MPI_Init(&argc, &argv);
//    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
//    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
//    MPI_Barrier(MPI_COMM_WORLD);
//    t1 = MPI_Wtime();
//    for (int i = 0; i < 3; i++) {
//        star(ProcRank, ProcNum);
//    }
//    MPI_Barrier(MPI_COMM_WORLD);
//    t2 = MPI_Wtime();
//    MPI_Finalize();
//    if (ProcRank == 0) {
//        cout << "\nTime: " << t2-t1;
//    }
//    return 0;
//}
