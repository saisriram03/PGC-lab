# MPI Distributed Memory

**Author: Sai Sri Ram**

The MPI program partitions rows across processes with scatter, broadcast, and gather collectives. Architecture and communication screenshots appear in [the main README](../README.md#3-mpi-distributed-memory-system).

```bash
make mpi
mpirun -np 4 ./build/mpi 4000
```
