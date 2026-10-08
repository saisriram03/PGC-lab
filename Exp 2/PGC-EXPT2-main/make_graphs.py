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

# ---- results.txt ----
lines = []
lines.append("Sequential runs (s): " + ", ".join("%.6f" % r for r in baseline_runs))
lines.append("Sequential baseline (average) = %.6f s" % baseline)
lines.append("")
lines.append("%-8s %-14s %-9s %-11s %-14s %-9s %-11s" % (
    "Threads", "Pthreads(s)", "Speedup", "Efficiency", "OpenMP(s)", "Speedup", "Efficiency"))
for i, n in enumerate(threads):
    lines.append("%-8d %-14.6f %-9.2f %-11s %-14.6f %-9.2f %-11s" % (
        n, pthread_times[i], pth_speedup[i], "%.1f%%" % pth_eff[i],
        omp_times[i], omp_speedup[i], "%.1f%%" % omp_eff[i]))
text = "\n".join(lines)
print(text)
with open("results.txt", "w") as f:
    f.write(text + "\n")

positions = list(range(len(threads)))
width = 0.38
labels = [str(n) for n in threads]

def grouped_bars(pth, omp, title, ylabel, filename, hline=None, hlabel=None):
    plt.figure(figsize=(8, 5))
    plt.bar([p - width / 2 for p in positions], pth, width, label="Pthreads")
    plt.bar([p + width / 2 for p in positions], omp, width, label="OpenMP")
    if hline is not None:
        plt.axhline(hline, color="red", linestyle="--", label=hlabel)
    plt.xticks(positions, labels)
    plt.xlabel("Number of threads")
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.grid(axis="y", alpha=0.3)
    plt.tight_layout()
    plt.savefig(os.path.join("graphs", filename), dpi=150)
    plt.close()

grouped_bars(pthread_times, omp_times,
             "Execution Time vs Threads", "Time (seconds)", "execution_time.png",
             baseline, "Sequential baseline (%.2f s)" % baseline)
grouped_bars(pth_speedup, omp_speedup,
             "Speedup over Sequential", "Speedup (x)", "speedup.png",
             1.0, "Sequential (1.00x)")
grouped_bars(pth_eff, omp_eff,
             "Parallel Efficiency", "Efficiency (%)", "efficiency.png",
             100, "Ideal (100%)")

print("")
print("Graphs saved in the graphs folder.")
