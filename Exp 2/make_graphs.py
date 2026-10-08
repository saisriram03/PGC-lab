import os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

baseline_runs = [3.331169, 3.822338, 3.496280, 4.020941, 4.199273]
baseline = sum(baseline_runs) / len(baseline_runs)

threads = [1, 2, 4, 6, 16]
pthread_times = [3.655563, 2.325327, 1.342439, 1.419750, 1.355168]
omp_times = [4.145702, 2.145581, 1.328977, 1.439114, 1.349932]

pth_speedup = [baseline / t for t in pthread_times]
omp_speedup = [baseline / t for t in omp_times]
pth_eff = [s / n * 100 for s, n in zip(pth_speedup, threads)]
omp_eff = [s / n * 100 for s, n in zip(omp_speedup, threads)]

os.makedirs("graphs", exist_ok=True)

lines = [
    "Sequential runs (s): " + ", ".join(f"{r:.6f}" for r in baseline_runs),
    f"Sequential baseline (average) = {baseline:.6f} s",
    "",
    "Threads  Pthreads(s)    Speedup   Efficiency  OpenMP(s)      Speedup   Efficiency"
]

for i, n in enumerate(threads):
    lines.append(
        f"{n:<8d}{pthread_times[i]:<15.6f}{pth_speedup[i]:<10.2f}"
        f"{pth_eff[i]:<12.1f}%{omp_times[i]:<15.6f}{omp_speedup[i]:<10.2f}{omp_eff[i]:.1f}%"
    )

with open("results.txt", "w") as f:
    f.write("\n".join(lines) + "\n")

positions = list(range(len(threads)))
width = 0.38
labels = [str(n) for n in threads]

def grouped_bars(pth, omp, title, ylabel, filename, hline=None, hlabel=None):
    plt.figure(figsize=(10, 6))
    plt.bar([p - width / 2 for p in positions], pth, width, label="Pthreads")
    plt.bar([p + width / 2 for p in positions], omp, width, label="OpenMP")
    if hline is not None:
        plt.axhline(hline, linestyle="--", label=hlabel)
    plt.xticks(positions, labels)
    plt.xlabel("Number of threads")
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.grid(axis="y", alpha=0.25)
    plt.tight_layout()
    plt.savefig(os.path.join("graphs", filename), dpi=180)
    plt.close()

grouped_bars(pthread_times, omp_times, "Execution Time vs Threads", "Time (seconds)",
             "execution_time.png", baseline, f"Sequential baseline ({baseline:.2f} s)")
grouped_bars(pth_speedup, omp_speedup, "Speedup over Sequential", "Speedup (×)",
             "speedup.png", 1.0, "Sequential (1.00×)")
grouped_bars(pth_eff, omp_eff, "Parallel Efficiency", "Efficiency (%)",
             "efficiency.png", 100, "Ideal (100%)")

print("Graphs saved in the graphs folder.")
