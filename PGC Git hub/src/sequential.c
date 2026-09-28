#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static size_t get_n(int argc, char **argv) {
    if (argc < 2) return 4000;
    errno = 0;
    char *end = NULL;
    unsigned long n = strtoul(argv[1], &end, 10);
    if (errno || end == argv[1] || *end || n == 0 || n > 20000) {
        fprintf(stderr, "Usage: %s [dimension: 1..20000]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    return (size_t)n;
}

static double now_seconds(void) {
    struct timespec t;
    timespec_get(&t, TIME_UTC);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

int main(int argc, char **argv) {
    const size_t n = get_n(argc, argv);
    if (n > SIZE_MAX / n || n * n > SIZE_MAX / sizeof(double)) return EXIT_FAILURE;
    const size_t count = n * n;
    double *a = malloc(count * sizeof(*a));
    double *b = malloc(count * sizeof(*b));
    double *c = calloc(count, sizeof(*c));
    if (!a || !b || !c) {
        fputs("Allocation failed; try a smaller matrix.\n", stderr);
        free(a); free(b); free(c);
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < count; ++i) a[i] = b[i] = 1.0;
    const double start = now_seconds();
    for (size_t i = 0; i < n; ++i)
        for (size_t k = 0; k < n; ++k) {
            const double aik = a[i * n + k];
            for (size_t j = 0; j < n; ++j) c[i * n + j] += aik * b[k * n + j];
        }
    const double elapsed = now_seconds() - start;
    const double expected = (double)n;
    printf("model=sequential N=%zu seconds=%.6f C[0][0]=%.2f expected=%.2f %s\n",
           n, elapsed, c[0], expected, c[0] == expected ? "PASS" : "FAIL");
    const int failed = c[0] != expected;
    free(a); free(b); free(c);
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
