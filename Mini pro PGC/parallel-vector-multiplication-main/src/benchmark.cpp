#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace chrono;

// Run sequential vector multiplication once
double runSequential(
    const vector<double>& A,
    const vector<double>& B,
    vector<double>& C
) {
    auto start = high_resolution_clock::now();

    for (long long i = 0; i < (long long)A.size(); i++) {
        C[i] = A[i] * B[i];
    }

    auto end = high_resolution_clock::now();

    // Prevent the compiler from removing the calculation
    volatile double checksum = C[0] + C[C.size() - 1];
    (void)checksum;

    return duration<double>(end - start).count();
}

// Run OpenMP vector multiplication once
double runParallel(
    const vector<double>& A,
    const vector<double>& B,
    vector<double>& C,
    int threads
) {
    omp_set_num_threads(threads);

    auto start = high_resolution_clock::now();

    #pragma omp parallel for
    for (long long i = 0; i < (long long)A.size(); i++) {
        C[i] = A[i] * B[i];
    }

    auto end = high_resolution_clock::now();

    // Prevent the compiler from removing the calculation
    volatile double checksum = C[0] + C[C.size() - 1];
    (void)checksum;

    return duration<double>(end - start).count();
}

int main() {

    // Vector sizes used in the experiment
    vector<long long> sizes = {
        100000,
        1000000,
        5000000,
        10000000
    };

    // Thread counts available for our experiment
    vector<int> threadCounts = {
        1,
        2,
        4,
        8,
        12
    };

    // Number of repetitions for each configuration
    const int repetitions = 5;

    // Create results directory/file
    ofstream file("results/raw_results.csv");

    if (!file.is_open()) {
        cerr << "Error: Could not create results/raw_results.csv\n";
        return 1;
    }

    file << "vector_size,threads,sequential_time,"
         << "parallel_time,speedup,efficiency\n";

    cout << "=============================================\n";
    cout << " OpenMP Parallel Vector Multiplication\n";
    cout << " Automated Benchmark\n";
    cout << "=============================================\n\n";

    cout << "Repetitions per configuration: "
         << repetitions << "\n\n";

    cout << fixed << setprecision(6);

    for (long long N : sizes) {

        cout << "---------------------------------------------\n";
        cout << "Vector size: " << N << "\n";
        cout << "---------------------------------------------\n";

        // Allocate vectors
        vector<double> A(N);
        vector<double> B(N);
        vector<double> C(N);

        // Initialize input vectors
        for (long long i = 0; i < N; i++) {
            A[i] = 1.0 + (i % 100);
            B[i] = 2.0 + (i % 50);
        }

        // -----------------------------------------
        // Sequential baseline
        // -----------------------------------------

        double sequentialTotal = 0.0;

        for (int r = 0; r < repetitions; r++) {
            sequentialTotal += runSequential(A, B, C);
        }

        double sequentialAverage =
            sequentialTotal / repetitions;

        cout << "Sequential average: "
             << sequentialAverage
             << " seconds\n\n";

        // -----------------------------------------
        // Parallel experiments
        // -----------------------------------------

        for (int threads : threadCounts) {

            double parallelTotal = 0.0;

            // One warm-up execution
            runParallel(A, B, C, threads);

            // Actual measurements
            for (int r = 0; r < repetitions; r++) {
                parallelTotal +=
                    runParallel(A, B, C, threads);
            }

            double parallelAverage =
                parallelTotal / repetitions;

            double speedup =
                sequentialAverage / parallelAverage;

            double efficiency =
                speedup / threads;

            cout << "Threads: "
                 << setw(2) << threads
                 << " | Parallel: "
                 << parallelAverage
                 << " s"
                 << " | Speedup: "
                 << speedup
                 << "x"
                 << " | Efficiency: "
                 << efficiency * 100.0
                 << "%\n";

            file << N << ","
                 << threads << ","
                 << sequentialAverage << ","
                 << parallelAverage << ","
                 << speedup << ","
                 << efficiency * 100.0
                 << "\n";
        }

        cout << "\n";
    }

    file.close();

    cout << "=============================================\n";
    cout << "Benchmark complete!\n";
    cout << "Results saved to:\n";
    cout << "results/raw_results.csv\n";
    cout << "=============================================\n";

    return 0;
}
