# plot_speedup_simple.py

import matplotlib
matplotlib.use('Agg')   # use the simple, non‑MKL Agg backend
import matplotlib.pyplot as plt

def plot_and_save(x, y, xlabel, ylabel, title, outfname):
    plt.figure()
    plt.plot(x, y, marker='o', linestyle='-')
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(outfname)
    plt.close()

# ===== Experiment 1: Speedup vs Threads =====

# Example data (replace with your actual speedups)
threads = [1, 2]
speedup_4 = [0.55, 1.00]      # e.g. [T1/T1, T1/T2] for 4 cities
plot_and_save(
    threads, speedup_4,
    'Number of Threads', 'Speedup',
    'Experiment 1: 4 Cities', 
    'speedup_4cities.png'
)

threads = [1, 2, 4]
speedup_8 = [1.0, 0.48, 0.66]  # e.g. [T1/T1, T1/T2, T1/T4] for 8 cities
plot_and_save(
    threads, speedup_8,
    'Number of Threads', 'Speedup',
    'Experiment 1: 8 Cities', 
    'speedup_8cities.png'
)

threads = [1, 2, 4]
speedup_12 = [1.0, 0.82, 3.32] # e.g. [T1/T1, T1/T2, T1/T4] for 12 cities
plot_and_save(
    threads, speedup_12,
    'Number of Threads', 'Speedup',
    'Experiment 1: 12 Cities', 
    'speedup_12cities.png'
)

# ===== Experiment 2: Speedup vs Number of Cities (4 Threads) =====


# Replace these with your measured speedups: (T1-thread / T4-thread)
cities = [6, 8, 10, 12]
speedup_cities = [
    0.0034113447,   # speedup for 6 cities
    0.6582604220,   # speedup for 8 cities
    2.0190485246,   # speedup for 10 cities
    3.3181131076    # speedup for 12 cities
]

plot_and_save(
    cities,
    speedup_cities,
    'Number of Cities',
    'Speedup (1‑thread / 4‑thread)',
    'Experiment 2: Scaling with 4 Threads',
    'speedup_scaling.png'
)

print("Plot saved as speedup_scaling.png")