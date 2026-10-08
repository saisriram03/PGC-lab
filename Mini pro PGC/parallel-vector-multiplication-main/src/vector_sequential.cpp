#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

int main(int argc, char* argv[]) {

    // Default vector size
    long long N = 1000000;

    // Allow vector size to be provided from command line
    if (argc >= 2) {
        N = std::stoll(argv[1]);
    }

    std::cout << "Sequential Vector Multiplication\n";
    std::cout << "Vector size: " << N << "\n";

    // Create three vectors
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

    // Sequential element-wise multiplication
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

    // Display a few values to verify correctness
    std::cout << "\nSample results:\n";

    for (int i = 0; i < 5 && i < N; i++) {
        std::cout << "C[" << i << "] = "
                  << C[i] << "\n";
    }

    return 0;
}
