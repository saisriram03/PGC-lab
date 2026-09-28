CC ?= gcc
MPICC ?= mpicc
NVCC ?= nvcc
CUDA_ARCH ?=
CFLAGS ?= -O2 -Wall -Wextra -std=c11
OMPFLAGS ?= -fopenmp
BUILD := build

.PHONY: all sequential openmp mpi cuda clean

all: sequential openmp

$(BUILD):
	mkdir -p $(BUILD)

sequential: | $(BUILD)
	$(CC) $(CFLAGS) src/sequential.c -o $(BUILD)/sequential

openmp: | $(BUILD)
	$(CC) $(CFLAGS) $(OMPFLAGS) src/openmp.c -o $(BUILD)/openmp

mpi: | $(BUILD)
	$(MPICC) $(CFLAGS) src/mpi.c -o $(BUILD)/mpi

cuda: | $(BUILD)
	$(NVCC) -O2 $(if $(CUDA_ARCH),-arch=$(CUDA_ARCH),) src/cuda.cu -o $(BUILD)/cuda

clean:
	rm -rf $(BUILD)
