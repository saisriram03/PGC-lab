import csv
import os
import matplotlib.pyplot as plt

INPUT_FILE = "results/raw_results.csv"
OUTPUT_DIR = "graphs"

os.makedirs(OUTPUT_DIR, exist_ok=True)

data = []

with open(INPUT_FILE, "r") as file:
    reader = csv.DictReader(file)

    for row in reader:
        data.append({
            "vector_size": int(row["vector_size"]),
            "threads": int(row["threads"]),
            "sequential_time": float(row["sequential_time"]),
            "parallel_time": float(row["parallel_time"]),
            "speedup": float(row["speedup"]),
            "efficiency": float(row["efficiency"])
        })


# --------------------------------------------------
# Graph 1: Execution Time vs Vector Size
# --------------------------------------------------

sizes = sorted(set(row["vector_size"] for row in data))

sequential_times = []

for size in sizes:
    row = next(r for r in data if r["vector_size"] == size)
    sequential_times.append(row["sequential_time"])

plt.figure(figsize=(10, 6))

plt.plot(
    sizes,
    sequential_times,
    marker="o",
    label="Sequential"
)

for threads in [1, 2, 4, 8, 12]:

    times = [
        next(
            r["parallel_time"]
            for r in data
            if r["vector_size"] == size
            and r["threads"] == threads
        )
        for size in sizes
    ]

    plt.plot(
        sizes,
        times,
        marker="o",
        label=f"OpenMP {threads} threads"
    )

plt.xlabel("Vector Size")
plt.ylabel("Execution Time (seconds)")
plt.title("Execution Time vs Vector Size")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()

plt.savefig(
    f"{OUTPUT_DIR}/execution_time_vs_vector_size.png",
    dpi=300
)

plt.close()


# --------------------------------------------------
# Graph 2: Speedup vs Threads
# --------------------------------------------------

plt.figure(figsize=(10, 6))

for size in sizes:

    threads = []
    speedups = []

    for row in data:

        if row["vector_size"] == size:

            threads.append(row["threads"])
            speedups.append(row["speedup"])

    plt.plot(
        threads,
        speedups,
        marker="o",
        label=f"{size:,} elements"
    )

plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.title("Speedup vs Number of Threads")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()

plt.savefig(
    f"{OUTPUT_DIR}/speedup_vs_threads.png",
    dpi=300
)

plt.close()


# --------------------------------------------------
# Graph 3: Efficiency vs Threads
# --------------------------------------------------

plt.figure(figsize=(10, 6))

for size in sizes:

    threads = []
    efficiency = []

    for row in data:

        if row["vector_size"] == size:

            threads.append(row["threads"])
            efficiency.append(row["efficiency"])

    plt.plot(
        threads,
        efficiency,
        marker="o",
        label=f"{size:,} elements"
    )

plt.xlabel("Number of Threads")
plt.ylabel("Efficiency (%)")
plt.title("Parallel Efficiency vs Number of Threads")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()

plt.savefig(
    f"{OUTPUT_DIR}/efficiency_vs_threads.png",
    dpi=300
)

plt.close()


# --------------------------------------------------
# Graph 4: Execution Time vs Threads
# --------------------------------------------------

plt.figure(figsize=(10, 6))

for size in sizes:

    threads = []
    times = []

    for row in data:

        if row["vector_size"] == size:

            threads.append(row["threads"])
            times.append(row["parallel_time"])

    plt.plot(
        threads,
        times,
        marker="o",
        label=f"{size:,} elements"
    )

plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (seconds)")
plt.title("OpenMP Execution Time vs Number of Threads")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()

plt.savefig(
    f"{OUTPUT_DIR}/execution_time_vs_threads.png",
    dpi=300
)

plt.close()


print("Graphs generated successfully!")
print()
print("Created:")

print("graphs/execution_time_vs_vector_size.png")
print("graphs/speedup_vs_threads.png")
print("graphs/efficiency_vs_threads.png")
print("graphs/execution_time_vs_threads.png")
