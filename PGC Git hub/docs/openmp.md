# OpenMP Shared-Memory CPU

**Author: Sai Sri Ram**

OpenMP distributes output rows among CPU threads sharing one address space. The eight-thread local run and its screenshots are documented in [the main README](../README.md#2-openmp-shared-memory-cpu).

```bash
make openmp
OMP_NUM_THREADS=8 ./build/openmp 4000
```
