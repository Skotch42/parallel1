#pragma once

#include <iostream>
#include <mpi.h>

int main(int argc, char** argv)
{
	int size, rank, tag = 50, message;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		for (int i = 1; i < size; i++)
		{
			message = i * 2;
			MPI_Send(&message, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(&message, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		std::cout << "Process " << rank << " : got " << message << '\n';
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}