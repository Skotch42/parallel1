#pragma once

#include <iostream>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(time(0));

	int* x = new int[size];
	int* A = 0;
	int* a = new int[size];
	int* y = 0;

	if (rank == 0)
	{
		A = new int[size * size];
		y = new int[size];

		std::cout << "x :\n";
		for (int i = 0; i < size; i++)
		{
			x[i] = rand() % 16 - 8;
			std::cout << x[i] << " ";
		}

		std::cout << "\n\nA :\n";
		for (int i = 0; i < size * size; i++)
		{
			A[i] = rand() % 16 - 8;
			std::cout << A[i] << ' ';
			if (i % size == size - 1) std::cout << "\n";
		}
		std::cout << std::endl;
	}

	MPI_Bcast(x, size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(A, size, MPI_INT, a, size, MPI_INT, 0, MPI_COMM_WORLD);

	int result = 0;
	for (int i = 0; i < size; i++)
	{
		result += a[i] * x[i];
	}

	std::cout << "Process " << rank << " : " << result << std::endl;
	MPI_Gather(&result, 1, MPI_INT, y, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		int* test = new int[size];
		for (int i = 0; i < size; i++)
		{
			test[i] = 0;
			for (int j = 0; j < size; j++)
			{
				test[i] += A[i * size + j] * x[j];
			}
		}

		bool passed = true;
		for (int i = 0; i < size; i++)
		{
			if (test[i] != y[i])
			{
				passed = false;
				break;
			}
		}

		std::cout << "\n" << "Successful : " << (passed ? "Yes" : "No") << "\n";

		delete A;
		delete y;
		delete test;
	}

	delete x;
	delete a;

	MPI_Finalize();
	return EXIT_SUCCESS;
}