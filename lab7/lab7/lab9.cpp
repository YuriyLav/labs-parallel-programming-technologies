//using namespace std;
//#include <iostream>
//#include "mpi.h"
//int main(int argc, char* argv[])
//{
//    int ProcNum, ProcRank;
//    int recvFrom = 0;
//    MPI_Init(&argc, &argv);
//    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
//    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
//    int* senddata = new int[ProcNum * 2];
//    int* recvdata = new int[ProcNum * 2];
//    if (ProcRank == 0)
//    {
//        for (int i = 0; i < ProcNum * 2; i++)
//        {
//            senddata[i] = i;
//        }
//    }
//    for (auto i = 0; i < 2; i++)
//        recvdata[i] = 0;
//    for (int i = 0; i < 3; i++) {
//        MPI_Barrier(MPI_COMM_WORLD);
//        MPI_Scatter(senddata, 2, MPI_INT, recvdata, 2, MPI_INT, 0, MPI_COMM_WORLD);
//        MPI_Gather(recvdata, 2, MPI_INT, senddata, 2, MPI_INT, 0, MPI_COMM_WORLD);
//        MPI_Barrier(MPI_COMM_WORLD);
//    }
//    cout << "recvdata: " << *recvdata << endl;
//    delete[] senddata;
//    delete[] recvdata;
//    MPI_Finalize();
//    return 0;
//}
