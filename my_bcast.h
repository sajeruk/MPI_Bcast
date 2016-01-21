#pragma once

#include <mpi/mpi.h>

int MY_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
