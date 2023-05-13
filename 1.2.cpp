#pragma once

#include <iostream>
#include <mpi.h>

int getNext(int rank, int size)
{
	return (rank < size - 1) ? rank + 1 : 0;
}

int getPrevious(int rank)
{
	return (rank > 0) ? rank - 1 : 0;
}

int main(int argc, char** argv)
{
	int size, rank, tag = 50, message;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		srand(time(0));
		message = rand() % 10;

		MPI_Send(&message, 1, MPI_INT, getNext(rank, size), tag, MPI_COMM_WORLD);
		std::cout << "Process 0 : sent " << message << '\n';
		MPI_Recv(&message, 1, MPI_INT, (size - 1), tag, MPI_COMM_WORLD, &status);
		std::cout << "Process 0 : got " << message << '\n';
	}
	else
	{
		MPI_Recv(&message, 1, MPI_INT, getPrevious(rank), tag, MPI_COMM_WORLD, &status);
		std::cout << "Process " << rank << " : got " << message << ", sent " << message + rank << '\n';
		message = message + rank;
		MPI_Send(&message, 1, MPI_INT, getNext(rank, size), tag, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}