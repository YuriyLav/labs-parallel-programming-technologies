#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include "mpi.h"
using namespace std;


const int N = 32768; 

void Merge(vector<int>& A) //сортировка слиянием
{
	int lb = 0, split = A.size() / 2 - 1, ub = A.size() - 1;

	long pos1 = lb;
	long pos2 = split + 1;
	long pos3 = 0;

	vector<int> temp(ub - lb + 1);

	while (pos1 <= split && pos2 <= ub) {
		if (A[pos1] < A[pos2])
			temp[pos3++] = A[pos1++];
		else
			temp[pos3++] = A[pos2++];
	}

	while (pos2 <= ub)
		temp[pos3++] = A[pos2++];
	while (pos1 <= split)
		temp[pos3++] = A[pos1++];

	for (pos3 = 0; pos3 < ub - lb + 1; pos3++)
		A[lb + pos3] = temp[pos3];
};


int main(int argc, char* argv[])
{
	srand(time(NULL));
	vector<int> vec;
	const size_t numProc = 4;
	int ProcNum, ProcRank, LeftRank, RightRank;

	for (int i = 0; i < N; i++) {
		vec.push_back(rand() % 100000 + 0);
	}

	MPI_Status Status;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	MPI_Group group;
	MPI_Comm_group(MPI_COMM_WORLD, &group); //получения группы, связанной с коммуникатором


	MPI_Group LeftGroup;
	MPI_Comm LeftComm;
	MPI_Group RightGroup;
	MPI_Comm RightComm;


	int Ranks[] = { 0, 1, 2, 3};

	MPI_Group_incl(group, numProc / 2, Ranks, &LeftGroup); //создание новой группы из существующей 

	MPI_Comm_create(MPI_COMM_WORLD, LeftGroup, &LeftComm); //создание нового коммуникатора 

	MPI_Group_incl(group, numProc / 2, &Ranks[(numProc / 2)], &RightGroup); //создание новой группы из существующей 

	MPI_Comm_create(MPI_COMM_WORLD, RightGroup, &RightComm); //создание нового коммуникатора 
	
	MPI_Group_rank(LeftGroup, &LeftRank); //получение ранга текущего процесса в группе
	MPI_Group_rank(RightGroup, &RightRank); //получение ранга текущего процесса в группе
	MPI_Datatype longVector; 
	int lenBlock = 2;
	while (lenBlock < N) {
		for (int i = 0; i < N; i += 4 * lenBlock) {
			int len = 2;
			vector<int> temp(lenBlock);
			for (int j = 0; j < lenBlock; j++)
			{
				temp[j] = vec[i + j + ProcRank * lenBlock];
			}
			Merge(temp);
			MPI_Barrier(MPI_COMM_WORLD);
			while (len < numProc)
			{
				MPI_Type_contiguous(lenBlock * len / 2, MPI_INT, &longVector);
				MPI_Type_commit(&longVector);
				if (LeftRank > -1) {
					if (LeftRank % len == 0) {
						MPI_Send(&temp[0], 1, longVector, LeftRank + len / 2, 0, LeftComm);
					}
					if ((LeftRank % len) == (len - 1)) {
						vector<int> recvTemp(lenBlock * len / 2);
						MPI_Recv(&recvTemp[0], 1, longVector, LeftRank - len / 2, 0, LeftComm, &Status);
						temp.insert(temp.end(), recvTemp.begin(), recvTemp.end());
						Merge(temp);
					}
				}
				else {
					if (RightRank % len == 0) {
						MPI_Send(&temp[0], 1, longVector, RightRank + len / 2, 0, RightComm);
					}
					if ((RightRank % len) == (len - 1)) {
						vector<int> recvTemp(lenBlock * len / 2);
						MPI_Recv(&recvTemp[0], 1, longVector, RightRank - len / 2, 0, RightComm, &Status);
						temp.insert(temp.end(), recvTemp.begin(), recvTemp.end());
						Merge(temp);
					}
				}
				len *= 2;

				MPI_Barrier(MPI_COMM_WORLD);
			}
			MPI_Type_contiguous(lenBlock * len / 2, MPI_INT, &longVector);
			MPI_Type_commit(&longVector);
			if (LeftRank > -1) {
				if (LeftRank % (len / 2) == len / 2 - 1) {
					MPI_Send(&temp[0], 1, longVector, LeftRank + len / 2, 0, MPI_COMM_WORLD);
				}
			}
			else {
				if (RightRank % (len / 2) == len / 2 - 1) {
					vector<int> recvTemp(lenBlock * len / 2);
					MPI_Recv(&recvTemp[0], 1, longVector, RightRank, 0, MPI_COMM_WORLD, &Status);
					temp.insert(temp.end(), recvTemp.begin(), recvTemp.end());
					Merge(temp);
					for (int j = 0; j < temp.size(); j++) {
						vec[i + j] = temp[j];
					}

				}
			}
			MPI_Barrier(MPI_COMM_WORLD);

		}
		lenBlock *= 8;
		MPI_Bcast(&vec[0], N, MPI_INT, 3, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);
	}
	if (ProcRank == 3) {
		for (int i = 0; i < N; i++) {
			cout << vec[i] << "	";
		}
		cout << "\n";
	}
	MPI_Group_free(&group);
	MPI_Group_free(&LeftGroup);
	MPI_Group_free(&RightGroup);
	MPI_Type_free(&longVector);
	MPI_Finalize();
	return 0;
}
