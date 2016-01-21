#include "my_bcast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
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
