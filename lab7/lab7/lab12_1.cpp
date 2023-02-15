//#include <stdio.h>
//#include "mpi.h"
//#define MAXPROC 128
//#define MAXEDGES 512
//int main(int argc, char** argv)
//{
//	int rank, rank1, i, size;
//	int a, b;
//	MPI_Status status;
//	MPI_Comm comm_graph;
//	int num, neighbors[MAXPROC];
//	MPI_Init(&argc, &argv);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	int nnodes = 5;
//	int index[5] = { 4, 6, 8, 9, 10 };
//	int edges[10] = { 1, 2, 3,4, 0,2, 0, 1, 0, 0};
//	int reorder2 = 1;
//
//	MPI_Graph_create(MPI_COMM_WORLD, nnodes, index, edges, 1, &comm_graph);
//	MPI_Graph_neighbors_count(comm_graph, rank, &num);
//	MPI_Graph_neighbors(comm_graph, rank, num, neighbors);
//	for (i = 0; i < num; i++) {
//		MPI_Sendrecv(&rank, 1, MPI_INT, neighbors[i], 1,
//			&rank1, 1, MPI_INT, neighbors[i], 1, comm_graph,
//			&status);
//		printf("process %d communicate with process %d\n",
//			rank, rank1);
//	}
//	MPI_Finalize();
//}