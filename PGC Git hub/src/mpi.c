#include <mpi.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static size_t get_n(int argc, char **argv) {
    if (argc < 2) return 4000;
    char *end = NULL;
    unsigned long n = strtoul(argv[1], &end, 10);
    if (!end || *end || n == 0 || n > 20000) {
        fprintf(stderr, "Usage: %s [dimension: 1..20000]\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    return (size_t)n;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, tasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    const size_t n = get_n(argc, argv);
    if (n > SIZE_MAX / n || n * n > SIZE_MAX / sizeof(double) || n * n > (size_t)INT_MAX) {
        if (rank == 0) fputs("Dimension exceeds supported MPI count limits.\n", stderr);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    const size_t total = n * n;
    const size_t ranks = (size_t)tasks;
    const size_t rows = n / ranks + ((size_t)rank < n % ranks);
    const size_t first = (size_t)rank * (n / ranks) + ((size_t)rank < n % ranks ? (size_t)rank : n % ranks);
    const size_t local_count = rows * n;
    double *b = malloc(total * sizeof(*b));
    double *local_a = malloc((local_count ? local_count : 1) * sizeof(*local_a));
    double *local_c = calloc(local_count ? local_count : 1, sizeof(*local_c));
    double *a = rank == 0 ? malloc(total * sizeof(*a)) : NULL;
    double *c = rank == 0 ? malloc(total * sizeof(*c)) : NULL;
    int *counts = rank == 0 ? malloc(ranks * sizeof(*counts)) : NULL;
    int *displs = rank == 0 ? malloc(ranks * sizeof(*displs)) : NULL;
    if (!b || !local_a || !local_c || (rank == 0 && (!a || !c || !counts || !displs))) {
        fprintf(stderr, "Rank %d: allocation failed.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    for (size_t i = 0; i < total; ++i) b[i] = 1.0;
    if (rank == 0) {
        for (size_t i = 0; i < total; ++i) a[i] = 1.0;
        for (int r = 0; r < tasks; ++r) {
            const size_t rr = n / ranks + ((size_t)r < n % ranks);
            const size_t offset = (size_t)r * (n / ranks) + ((size_t)r < n % ranks ? (size_t)r : n % ranks);
            counts[r] = (int)(rr * n);
            displs[r] = (int)(offset * n);
        }
    }
    (void)first;
    MPI_Barrier(MPI_COMM_WORLD);
    const double start = MPI_Wtime();
    MPI_Scatterv(a, counts, displs, MPI_DOUBLE, local_a, (int)local_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, (int)total, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for (size_t i = 0; i < rows; ++i)
        for (size_t k = 0; k < n; ++k) {
            const double aik = local_a[i * n + k];
            for (size_t j = 0; j < n; ++j) local_c[i * n + j] += aik * b[k * n + j];
        }
    MPI_Gatherv(local_c, (int)local_count, MPI_DOUBLE, c, counts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    const double elapsed = MPI_Wtime() - start;
    int failed = 0;
    if (rank == 0) {
        const double expected = (double)n;
        failed = c[0] != expected;
        printf("model=mpi ranks=%d N=%zu seconds=%.6f C[0][0]=%.2f expected=%.2f %s\n",
               tasks, n, elapsed, c[0], expected, failed ? "FAIL" : "PASS");
    }
    MPI_Bcast(&failed, 1, MPI_INT, 0, MPI_COMM_WORLD);
    free(a); free(b); free(c); free(local_a); free(local_c); free(counts); free(displs);
    MPI_Finalize();
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
