#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include "mpi.h"
using namespace std;

int main(int argc, char* argv[])
{
	const size_t kProc = 8;
	int ProcNum, ProcRank, DecRank, GraphRank;

	MPI_Status Status;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	MPI_Group MainGroup;
	MPI_Comm_group(MPI_COMM_WORLD, &MainGroup);


	MPI_Group DecGroup;
	MPI_Comm DecComm;
	MPI_Group GraphGroup;
	MPI_Comm GraphComm;


	int Ranks[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	MPI_Group_incl(MainGroup, kProc / 2, Ranks, &DecGroup);

	MPI_Comm_create(MPI_COMM_WORLD, DecGroup, &DecComm);
	MPI_Group_rank(DecGroup, &DecRank);

	MPI_Group_incl(MainGroup, kProc / 2, &Ranks[kProc / 2], &GraphGroup);

	MPI_Comm_create(MPI_COMM_WORLD, GraphGroup, &GraphComm);
	MPI_Group_rank(GraphGroup, &GraphRank);

	//MPI_Comm CartComm;

	int dims[1], periods[1], reorder = 1;
	dims[0] = 4;
	periods[0] = 1;
	if (ProcRank < 4) {
		int sendmsg, recievedmsg = -1;
		MPI_Cart_create(DecComm, 1, dims, periods, reorder, &DecComm);
		int* ranks = new int[2];
		MPI_Cart_shift(DecComm, 0, 1, &ranks[0], &ranks[1]);
		int rank;
		MPI_Comm_rank(DecComm, &rank);
		sendmsg = rank;
		printf("%d process send message to %d\n", sendmsg, ranks[1]);
		MPI_Sendrecv(&sendmsg, 1, MPI_INT, ranks[1], 0, &recievedmsg, 1, MPI_INT, ranks[0], 0, DecComm, &Status);
		printf("%d process receive message from slave %d\n", sendmsg, recievedmsg);
	}
	else {
		MPI_Comm grComm;
		int nnodes = 4; 
		int index[4] = { 3, 4, 5, 6 }; 
		int edges[6] = { 1, 2, 3, 0, 0, 0 }; 
		int reorder2 = 1; 
		MPI_Graph_create(GraphComm, nnodes, index, edges, reorder2, &grComm);
		int rank;
		MPI_Comm_rank(GraphComm, &rank);
		if (rank == 0) {
			//int neighs[3];
			int sendMes, getMes = -1;
			sendMes = rank;
			for (int i = 1; i < 4; i++) {
				MPI_Send(&sendMes, 1, MPI_INT, i, 0, grComm);
				printf("%d process send message to slave %d\n", sendMes, i);
			}
			for (int i = 1; i < 4; i++) {
				MPI_Recv(&getMes, 1, MPI_INT, i, 0, grComm, &Status);
				printf("%d process receive message from slave %d\n", sendMes, getMes);
			}
		}
		else {
			int sendMes, getMes = -1;
			sendMes = rank;
			MPI_Recv(&getMes, 1, MPI_INT, 0, 0, grComm, &Status);
			printf("%d process receive message from master %d\n", sendMes, getMes);
			MPI_Send(&sendMes, 1, MPI_INT, 0, 0, grComm);
			printf("%d process send message to master 0\n", sendMes);
		}
	}
	MPI_Group_free(&MainGroup);
	MPI_Group_free(&GraphGroup);
	MPI_Group_free(&DecGroup);
	MPI_Finalize();
	return 0;
}