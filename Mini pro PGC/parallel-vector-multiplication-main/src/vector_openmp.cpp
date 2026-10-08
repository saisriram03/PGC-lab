#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <omp.h>

int main(int argc, char* argv[]) {

    // Default vector size
    long long N = 1000000;

    // Default number of threads
    int numThreads = 1;

    // First argument = vector size
    if (argc >= 2) {
        N = std::stoll(argv[1]);
    }

    // Second argument = number of OpenMP threads
    if (argc >= 3) {
        numThreads = std::stoi(argv[2]);
    }

    // Tell OpenMP how many threads to use
    omp_set_num_threads(numThreads);

    std::cout << "OpenMP Vector Multiplication\n";
    std::cout << "Vector size: " << N << "\n";
    std::cout << "Threads: " << numThreads << "\n";

    // Create vectors
    std::vector<double> A(N);
    std::vector<double> B(N);
    std::vector<double> C(N);

    // Initialize input vectors
    for (long long i = 0; i < N; i++) {
        A[i] = 1.0 + (i % 100);
        B[i] = 2.0 + (i % 50);
    }

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Parallel element-wise multiplication
    #pragma omp parallel for
    for (long long i = 0; i < N; i++) {
        C[i] = A[i] * B[i];
    }

    // Stop timing
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Execution time: "
              << elapsed.count()
              << " seconds\n";

    // Display sample results
    std::cout << "\nSample results:\n";

    for (int i = 0; i < 5 && i < N; i++) {
        std::cout << "C[" << i << "] = "
                  << C[i] << "\n";
    }

    return 0;
}
