<div align="center">

# 🌈 PGC • EXPERIMENT 2 • VISUAL EDITION

### 🧵 Pthreads  ·  ⚡ OpenMP  ·  🔐 Synchronization  ·  📊 Benchmarking

![C](https://img.shields.io/badge/C-Programming-2563EB?style=for-the-badge&logo=c)
![Pthreads](https://img.shields.io/badge/Pthreads-Multithreading-7C3AED?style=for-the-badge)
![OpenMP](https://img.shields.io/badge/OpenMP-Parallelism-F97316?style=for-the-badge)
![Benchmark](https://img.shields.io/badge/Benchmark-Performance-10B981?style=for-the-badge)

</div>

> 🎨 **Design enhancement only:** The original experiment content, code, results, screenshots, and report material are retained. This edition adds a cleaner visual hierarchy, colorful badges, polished evidence presentation, and improved repository organization.

---

# Experiment 2: Multithreaded Programming
### Pthreads · OpenMP · Race Condition · Synchronization · Performance

`C` `Pthreads` `OpenMP` `Mutex` `Synchronization` `Multithreading`

**Name:** Chaitra  |  **Roll No:** 253  |  **Course:** PG Parallel Computing

---

## 📌 Aim

To write multithreaded programs in C using **Pthreads** and **OpenMP**, to show a **race condition** and fix it with a **mutex** (Pthreads) and **critical** (OpenMP), to show **barrier** synchronization, and to compare performance for 1, 2, 4, 6 and 16 threads against a sequential baseline.

| # | Part | What it shows |
|---|------|---------------|
| 1 | Pthreads basics | thread creation, passing arguments, join, partial sums |
| 2 | Race condition + mutex | wrong result without locking, correct result with a mutex |
| 3 | OpenMP basics | parallel region, parallel for with reduction |
| 4 | OpenMP race + critical + barrier | same race, fixed with critical, stages with barrier |
| 5 | Performance | sequential vs Pthreads vs OpenMP, speedup and efficiency |

**Problem:** The performance program sums `i * 0.000001` for i = 0 to N-1 with N = 1,000,000,000. The correct answer is `499999999500.00`. This is used as the verification check.

---

## 🖥️ Environment

| Item | Details |
|------|---------|
| Host | Windows with VMware Workstation 17 Player |
| Guest OS | Ubuntu 24.04 64-bit (VM) |
| CPU (VM) | 4 logical CPUs (`nproc` = 4) |
| Compiler | GCC 13.3.0, default flags (no `-O2`) |
| Pthreads | `gcc file.c -o file -pthread` |
| OpenMP | `gcc file.c -o file -fopenmp` |

> **Note:** The reference manual was run on Windows with WSL Ubuntu, where OpenMP used 32 threads and 16 threads gave about 9.6× speedup (sequential baseline 1.35 s). Here the programs ran on a VMware Ubuntu VM with only 4 logical CPUs, so speedup stops at about 2.8× with 4 threads and more threads give no further gain. Absolute times therefore differ from the manual.

---

## 📁 Repository Structure

```
PGCEXPT-2/
├── README.md
├── results.txt
├── make_graphs.py
├── pthreads/
│   ├── thread1.c
│   ├── thread2.c
│   ├── thread_sum.c
│   ├── race.c
│   ├── mutex.c
│   └── output_pthreads.txt
├── openmp/
│   ├── omp1.c
│   ├── omp_sum.c
│   ├── omp_race.c
│   ├── omp_critical.c
│   ├── omp_barrier.c
│   └── output_openmp.txt
├── performance/
│   ├── sequential.c
│   ├── pthread_perf.c
│   └── omp_perf.c
├── graphs/
│   ├── execution_time.png
│   ├── speedup.png
│   └── efficiency.png
└── screenshots/
    ├── pthreads_basic.png
    ├── pthreads_race_mutex.png
    ├── openmp_basic.png
    ├── openmp_race_critical.png
    └── performance.png
```

---

## ▶️ How to Compile and Run

```bash
# Pthreads
cd pthreads
gcc thread1.c    -o thread1    -pthread && ./thread1
gcc thread2.c    -o thread2    -pthread && ./thread2
gcc thread_sum.c -o thread_sum -pthread && ./thread_sum
gcc race.c       -o race       -pthread && ./race
gcc mutex.c      -o mutex      -pthread && ./mutex

# OpenMP
cd ../openmp
gcc omp1.c         -o omp1         -fopenmp && ./omp1
gcc omp_sum.c      -o omp_sum      -fopenmp && ./omp_sum
gcc omp_race.c     -o omp_race     -fopenmp && ./omp_race
gcc omp_critical.c -o omp_critical -fopenmp && ./omp_critical
gcc omp_barrier.c  -o omp_barrier  -fopenmp && ./omp_barrier

# Performance (asks for the number of threads)
cd ../performance
gcc sequential.c   -o sequential
gcc pthread_perf.c -o pthread_perf -pthread
gcc omp_perf.c     -o omp_perf     -fopenmp
./sequential
./pthread_perf
./omp_perf
```

---

## 📊 Results

All performance versions produced the correct verification value 499999999500.00.

### Part 1: Correctness and synchronization

The `race` and `omp_race` programs have no locking on purpose. A wrong count is the expected result there, and it proves the race condition. The `mutex` and `critical` versions fix it.

| Program | Synchronization | Expected | Actual | Observation |
|---------|-----------------|----------|--------|-------------|
| thread_sum (4 threads) | none needed (separate array parts) | 360 | 360 (30 + 70 + 110 + 150) | ✅ Correct |
| race (Pthreads) | none | 400000 | 360173 and 170001 (two runs) | ✅ Race condition demonstrated (count lost, changes each run) |
| mutex (Pthreads) | mutex lock | 400000 | 400000 | ✅ Race fixed |
| omp_sum | reduction | 360 | 360 | ✅ Correct |
| omp_race (OpenMP) | none | 400000 | 293558 | ✅ Race condition demonstrated (count lost) |
| omp_critical (OpenMP) | critical section | 400000 | 400000 | ✅ Race fixed |

`omp_barrier`: all four threads printed "completed Stage 1" before any thread printed "started Stage 2".

### Part 2: Performance

Summary at 4 threads:

| Implementation | Resources | Time (s) | Speedup | Verification |
|----------------|-----------|----------|---------|--------------|
| Sequential | 1 CPU core | 3.774000 | 1.00× | ✅ 499999999500.00 |
| Pthreads | 4 threads | 1.342439 | 2.81× | ✅ 499999999500.00 |
| OpenMP | 4 threads | 1.328977 | 2.84× | ✅ 499999999500.00 |

Sequential baseline: five runs of 3.331169, 3.822338, 3.496280, 4.020941 and 4.199273 s, average **3.774000 s**.

Results for all thread counts:

| Threads | Pthreads time (s) | Speedup | Efficiency | OpenMP time (s) | Speedup | Efficiency |
|---------|-------------------|---------|------------|-----------------|---------|------------|
| 1  | 3.655563 | 1.03× | 103.2% | 4.145702 | 0.91× | 91.0% |
| 2  | 2.325327 | 1.62× | 81.1%  | 2.145581 | 1.76× | 87.9% |
| 4  | 1.342439 | 2.81× | 70.3%  | 1.328977 | 2.84× | 71.0% |
| 6  | 1.419750 | 2.66× | 44.3%  | 1.439114 | 2.62× | 43.7% |
| 16 | 1.355168 | 2.78× | 17.4%  | 1.349932 | 2.80× | 17.5% |

**Speedup formula:** Speedup = Sequential time / Parallel time

---

## 📈 Performance Graphs

### Execution Time
![Execution Time](graphs/execution_time.png)

### Speedup over Sequential
![Speedup](graphs/speedup.png)

### Parallel Efficiency
Efficiency = speedup / number of threads.

| Implementation | Speedup | Efficiency |
|----------------|---------|------------|
| Pthreads (4 threads) | 2.81× | 70.3% |
| OpenMP (4 threads) | 2.84× | 71.0% |

![Efficiency](graphs/efficiency.png)

---

## 📷 Screenshots

### Part A: Pthreads basics
![Pthreads basic](screenshots/pthreads_basic.png)

### Part B: Pthreads race condition and mutex
![Pthreads race and mutex](screenshots/pthreads_race_mutex.png)

### Part C: OpenMP basics and barrier
![OpenMP basic](screenshots/openmp_basic.png)

### Part D: OpenMP race condition and critical
![OpenMP race and critical](screenshots/openmp_race_critical.png)

### Part E: Performance
![Performance](screenshots/performance.png)

---

## 🔍 Observations

- **Race condition:** `counter++` is not one step. It is load, add and store. When 4 threads do this at the same time, updates overwrite each other, so the counter ended below 400000 (360173 and 170001 with Pthreads, 293558 with OpenMP) and changed on every run.
- **Fix:** a Pthreads mutex and an OpenMP critical section let only one thread update the counter at a time. Both gave exactly 400000.
- **Scaling:** time dropped from about 3.7 s (1 thread) to about 1.34 s at 4 threads, a speedup of about 2.8×. This is the best result because the VM has 4 logical CPUs.
- **6 and 16 threads:** no improvement over 4 threads (about 1.35 to 1.44 s). With only 4 CPUs, extra threads just wait for a core and add scheduling overhead, so efficiency fell to about 44% at 6 threads and about 17% at 16 threads.
- **Not perfectly linear:** 4 threads gave 2.8× instead of 4× because the VM's virtual CPUs are shared with the host and other programs. The five sequential runs varied from 3.33 to 4.20 s, so timings on this VM are noisy.
- **Pthreads vs OpenMP:** performance was almost the same (1.342 s vs 1.329 s at 4 threads). OpenMP needs far less code, for example `parallel for reduction` replaces manual thread creation, chunking and joining.
- **OpenMP at 1 thread** was slightly slower than the sequential baseline (0.91×) because of the OpenMP runtime overhead and run-to-run noise.
- **Run-to-run variation:** the `performance.png` screenshot is from a later, faster run (sequential about 1.97 s, 4 threads about 0.73 s). Timings change between runs on this VM, so the tables and graphs above use the earlier full set of measurements. In both runs the speedup at 4 threads was about 2.7× to 2.8×.
- **Measured results depend on hardware.** These numbers come from a 4-CPU VM, so they will differ on other machines and from the reference manual.

---

## ✅ Conclusion

Multithreaded programs were written with Pthreads and OpenMP. The unsynchronized counter produced wrong results, and a mutex (Pthreads) and critical (OpenMP) fixed it. For the 1,000,000,000-iteration sum, both models gave a speedup of about 2.8× with 4 threads on the 4-CPU VM, and using more threads than CPU cores gave no further gain and lowered efficiency. Pthreads gives low-level control, while OpenMP gives the same performance with much simpler code.

---

*Experiment 2 · Parallel Computing Lab*


---

<div align="center">

### 🚀 PARALLEL COMPUTING • MEASURE • COMPARE • ANALYZE 🚀

**Experiment 2 — Multithreaded Programming**

</div>
