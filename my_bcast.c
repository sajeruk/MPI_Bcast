#include "my_bcast.h"

int MY_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    MPI_Comm inner;
    MPI_Comm_dup(comm, &inner);

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

    MPI_Comm_free(&inner);
    return MPI_SUCCESS;
}
