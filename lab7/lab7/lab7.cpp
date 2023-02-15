//using namespace std;
//#include <iostream>
//#include "mpi.h"
//void star(int ProcRank, int ProcNum) {
//    if (ProcRank == 0) {  // 0 1 2 3 4 5 6 7 8 9 10 11
//        char message[] = "message";
//        for (int i = 1; i < ProcNum; i++) {
//            MPI_Send(message, _countof(message), MPI_CHAR, ProcRank + i, 0, MPI_COMM_WORLD);
//            cout << "Send message to rank: " << ProcRank + i << endl;
//            MPI_Recv(message, _countof(message), MPI_CHAR, ProcRank + i, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
//            cout << "Receive message from rank: " << ProcRank + i << endl;
//        }
//    }
//    else {
//        char message[] = "message";
//        MPI_Recv(message, _countof(message), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
//        cout << "Receive message from rank: " << 0 << endl;
//        MPI_Send(message, _countof(message), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
//        cout << "Send message to rank: " << 0 << endl;
//    }
//}
//int main(int argc, char* argv[])
//{
//    double t1, t2, dt = 0;
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
//        cout << "\nTime: " << t2 - t1;
//    }
//    return 0;
//}
//
