#include <mpi/mpi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int MY_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    static const int TAG = 83241; // some random number
    int size, real_rank;
    if (count < 0) { // обработаем ошибки похожим на MPI_Bcast образом
        return MPI_ERR_COUNT;
    } else if (!buffer) {
        return MPI_ERR_BUFFER;
    } else if (!comm) {
        return MPI_ERR_COMM;
    } else if (MPI_Type_commit(&datatype)) {
        return MPI_ERR_TYPE;
    }
    MPI_Comm_size(comm, &size); // get num of processes
    if (root < 0 || root >= size) {
        return MPI_ERR_ROOT;
    }
    // otherwise - let's do some send-recv
    MPI_Comm_rank(comm, &real_rank);

    int rank = (real_rank - root + size) % size; // reorder, so fake root is zero

    if (rank != 0) { // not root
        // recv
        int from = ((rank - 1) / 2 + root) % size; // fake rank, map to real
        MPI_Status status;
        MPI_Recv(buffer, count, datatype, from, TAG + real_rank, comm, &status);
    }
    int child1 = 2 * rank + 1;
    int child2 = 2 * rank + 2;

    if (child1 < size) {
        child1 = (child1 + root) % size; // mapping to real
        MPI_Send(buffer, count, datatype, child1, TAG + child1, comm);
    }
    if (child2 < size) {
        child2 = (child2 + root) % size; // mapping to real
        MPI_Send(buffer, count, datatype, child2, TAG + child2, comm);
    }

    return MPI_SUCCESS;
}

int main(int argc, char *argv[])
{
    int rank;
    int root = atoi(argv[1]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char buf[100];
    strcpy(buf, "nothing");
    if (rank == root) {
        strcpy(buf, "hello!");
    }

    MPI_Bcast(buf, sizeof(buf), MPI_CHAR, root, MPI_COMM_WORLD);

    char buf1[100];
    strcpy(buf1, "smthing");
    if (rank == root) {
        strcpy(buf1, "hello!");
    }

    MY_Bcast(buf1, sizeof(buf1), MPI_CHAR, root, MPI_COMM_WORLD);
    if (strcmp(buf, buf1) != 0) {
        printf("MPI_Bcast and MY_Bcast differ!\n");
    }

    MPI_Finalize();

    return 0;
}
