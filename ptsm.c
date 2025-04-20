#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <omp.h>
#include <time.h>

static int **dist;        // distance matrix
static int  numCities;    // number of cities

// allocate an N×N integer matrix
static int **allocMatrix(int N) {
    int **m = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        m[i] = malloc(N * sizeof(int));
    }
    return m;
}

// load the matrix from a text file
static void loadMatrix(const char *fname) {
    FILE *f = fopen(fname, "r");
    if (!f) {
        perror("cannot open matrix file");
        exit(1);
    }
    dist = allocMatrix(numCities);
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            if (fscanf(f, "%d", &dist[i][j]) != 1) {
                fprintf(stderr, "bad data at %d,%d\n", i, j);
                exit(1);
            }
        }
    }
    fclose(f);
}

// recursive backtracking
static void dfs(int city, int visited[], int path[], int depth,
                int cost, int *bestCost, int bestPath[]) {
    if (depth == numCities) {
        if (cost < *bestCost) {
            *bestCost = cost;
            memcpy(bestPath, path, numCities * sizeof(int));
        }
        return;
    }
    for (int nxt = 0; nxt < numCities; nxt++) {
        if (!visited[nxt]) {
            visited[nxt] = 1;
            path[depth]  = nxt;
            dfs(nxt, visited, path, depth + 1,
                cost + dist[city][nxt], bestCost, bestPath);
            visited[nxt] = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <cities> <threads> <matrix.txt>\n", argv[0]);
        return 1;
    }
    numCities = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    const char *file = argv[3];

    loadMatrix(file);

    int globalBest = INT_MAX;
    int *globalPath = malloc(numCities * sizeof(int));

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    omp_set_num_threads(numThreads);
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int slice = (numCities - 1 + numThreads - 1) / numThreads;
        int lo = 1 + tid * slice;
        int hi = lo + slice;
        if (hi > numCities) hi = numCities;

        int localBest = INT_MAX;
        int localPath[numCities];

        for (int first = lo; first < hi; first++) {
            int visited[numCities];
            int path[numCities];

            memset(visited, 0, sizeof(visited));
            visited[0] = 1;
            visited[first] = 1;

            path[0] = 0;
            path[1] = first;

            dfs(first, visited, path, 2, dist[0][first],
                &localBest, localPath);
        }

        #pragma omp critical
        {
            if (localBest < globalBest) {
                globalBest = localBest;
                memcpy(globalPath, localPath, numCities * sizeof(int));
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec  - t0.tv_sec)
                   + (t1.tv_nsec - t0.tv_nsec)/1e9;

    // output
    printf("Best path: ");
    for (int i = 0; i < numCities; i++) {
        printf("%d ", globalPath[i]);
    }
    printf("\nDistance: %d\n", globalBest);
    printf("Elapsed time: %.6f s\n", elapsed);

    // cleanup
    for (int i = 0; i < numCities; i++) free(dist[i]);
    free(dist);
    free(globalPath);

    return 0;
}