#include <cuda_runtime.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <chrono>
#include <cstddef>
#include <limits>

#define CUDA_CHECK(call) do { \
    cudaError_t error = (call); \
    if (error != cudaSuccess) { \
        std::fprintf(stderr, "CUDA error at %s:%d: %s\n", __FILE__, __LINE__, cudaGetErrorString(error)); \
        std::exit(EXIT_FAILURE); \
    } \
} while (0)

__global__ void multiply(const float *a, const float *b, float *c, int n) {
    const int col = blockIdx.x * blockDim.x + threadIdx.x;
    const int row = blockIdx.y * blockDim.y + threadIdx.y;
    if (row < n && col < n) {
        float sum = 0.0f;
        for (int k = 0; k < n; ++k) sum += a[(size_t)row * n + k] * b[(size_t)k * n + col];
        c[(size_t)row * n + col] = sum;
    }
}

int main(int argc, char **argv) {
    errno = 0;
    char *end = nullptr;
    const long parsed = argc > 1 ? std::strtol(argv[1], &end, 10) : 4000;
    if (argc > 1 && (errno || end == argv[1] || *end || parsed < 1 || parsed > 20000)) {
        std::fprintf(stderr, "Usage: %s [dimension: 1..20000]\n", argv[0]);
        return EXIT_FAILURE;
    }
    const int n = (int)parsed;
    const size_t count = (size_t)n * n;
    if (count > std::numeric_limits<size_t>::max() / sizeof(float)) return EXIT_FAILURE;
    const size_t bytes = count * sizeof(float);
    float *a = (float *)std::malloc(bytes), *b = (float *)std::malloc(bytes), *c = (float *)std::malloc(bytes);
    if (!a || !b || !c) { std::fputs("Host allocation failed.\n", stderr); return EXIT_FAILURE; }
    for (size_t i = 0; i < count; ++i) a[i] = b[i] = 1.0f;
    const auto total_start = std::chrono::steady_clock::now();
    float *da, *db, *dc;
    CUDA_CHECK(cudaMalloc((void **)&da, bytes)); CUDA_CHECK(cudaMalloc((void **)&db, bytes)); CUDA_CHECK(cudaMalloc((void **)&dc, bytes));
    CUDA_CHECK(cudaMemcpy(da, a, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(db, b, bytes, cudaMemcpyHostToDevice));
    const dim3 block(16, 16);
    const dim3 grid((n + block.x - 1) / block.x, (n + block.y - 1) / block.y);
    cudaEvent_t begin, finish;
    CUDA_CHECK(cudaEventCreate(&begin)); CUDA_CHECK(cudaEventCreate(&finish));
    CUDA_CHECK(cudaEventRecord(begin));
    multiply<<<grid, block>>>(da, db, dc, n);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaEventRecord(finish)); CUDA_CHECK(cudaEventSynchronize(finish));
    float kernel_ms = 0.0f;
    CUDA_CHECK(cudaEventElapsedTime(&kernel_ms, begin, finish));
    CUDA_CHECK(cudaMemcpy(c, dc, bytes, cudaMemcpyDeviceToHost));
    const double total_seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - total_start).count();
    const float expected = (float)n;
    const bool pass = std::fabs(c[0] - expected) < 0.5f;
    std::printf("model=cuda N=%d kernel_ms=%.3f total_seconds=%.6f C[0][0]=%.2f expected=%.2f %s\n",
                n, kernel_ms, total_seconds, c[0], expected, pass ? "PASS" : "FAIL");
    CUDA_CHECK(cudaEventDestroy(begin)); CUDA_CHECK(cudaEventDestroy(finish));
    CUDA_CHECK(cudaFree(da)); CUDA_CHECK(cudaFree(db)); CUDA_CHECK(cudaFree(dc));
    std::free(a); std::free(b); std::free(c);
    return pass ? EXIT_SUCCESS : EXIT_FAILURE;
}
