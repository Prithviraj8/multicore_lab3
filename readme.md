# Parallel Traveling Salesman Problem

This directory contains a single source file, `ptsm.c`, which implements both the sequential and OpenMP‑parallel solutions to a modified Traveling Salesman Problem (TSP).

## Overview

The program:

- Reads three arguments:
  - `numCities` — number of cities (e.g. 4, 6, 8, 10, 12)
  - `numThreads` — number of OpenMP threads (1 ≤ threads ≤ numCities)
  - `matrixFile` — path to an N×N distance matrix file

- Always starts at city 0 and visits each other city exactly once
- Splits the first hop across threads when numThreads > 1
- Prints:
  ```yaml
  Best path: ...
  Distance: ...
  Elapsed time: X.YYYYYY seconds
  ```

## Compilation (on NYU CIMS "crunchy")

1. Compile:
```bash
gcc -O2 -fopenmp -o ptsm ptsm.c
```

## Usage

```bash
./ptsm <numCities> <numThreads> <matrixFile>
```

Where:
- `<numCities>` — size of the matrix file
- `<numThreads>` — use 1 for sequential; up to numCities for parallel
- `<matrixFile>` — whitespace‑separated N×N integer matrix

## Correctness Validation

Before performance testing, verify your solver matches the exact result on small inputs:

```bash
chmod +x ./tsmoptimal
./tsmoptimal 4 cities4.txt
./ptsm        4 1 cities4.txt

./tsmoptimal 6 cities6.txt
./ptsm        6 1 cities6.txt
```

Include a brief table in your report comparing the paths and distances.

## Performance Experiments

### Generate Input Files

Use the provided generator:

```bash
chmod +x ./tsm
./tsm 4    # → cities4.txt
./tsm 6    # → cities6.txt
./tsm 8    # → cities8.txt
./tsm 10   # → cities10.txt
./tsm 12   # → cities12.txt
```

### Experiment 1: Speedup vs. Threads

For each city count, run:

```bash
# 4 cities
./ptsm  4 1 cities4.txt
./ptsm  4 2 cities4.txt

# 8 cities
./ptsm  8 1 cities8.txt
./ptsm  8 2 cities8.txt
./ptsm  8 4 cities8.txt

# 12 cities
./ptsm 12 1 cities12.txt
./ptsm 12 2 cities12.txt
./ptsm 12 4 cities12.txt
```

### Experiment 2: Scaling with 4 Threads

```bash
./ptsm  6 4 cities6.txt
./ptsm  8 4 cities8.txt
./ptsm 10 4 cities10.txt
./ptsm 12 4 cities12.txt
```

Record the Elapsed time for each run, compute speedups, and generate your plots.

## Results

[This section will be updated with experimental results and analysis]
