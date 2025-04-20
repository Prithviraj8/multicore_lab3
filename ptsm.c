#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>

static int **distMatrix;
static int  numCities;
static int  bestDistance = INT_MAX;
static int *bestPath;

// allocate an NxN matrix
static int **allocateMatrix(int N) {
    int **m = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        m[i] = malloc(N * sizeof(int));
    }
    return m;
}

// read the distance matrix from a file
static void readMatrix(const char *fname) {
    FILE *f = fopen(fname, "r");
    if (!f) {
        perror("Failed to open matrix file");
        exit(EXIT_FAILURE);
    }
    distMatrix = allocateMatrix(numCities);
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            if (fscanf(f, "%d", &distMatrix[i][j]) != 1) {
                fprintf(stderr, "Error reading [%d][%d]\n", i, j);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(f);
}

// backtracking DFS to explore all tours
static void tsp(int city, int visited[], int path[], int depth, int cost) {
    if (depth == numCities) {
        if (cost < bestDistance) {
            bestDistance = cost;
            memcpy(bestPath, path, numCities * sizeof(int));
        }
        return;
    }
    for (int nxt = 0; nxt < numCities; nxt++) {
        if (!visited[nxt]) {
            visited[nxt] = 1;
            path[depth]   = nxt;
            tsp(nxt, visited, path, depth + 1, cost + distMatrix[city][nxt]);
            visited[nxt] = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <numCities> <numThreads> <matrixFile>\n", argv[0]);
        return EXIT_FAILURE;
    }
    numCities = atoi(argv[1]);
    // int numThreads = atoi(argv[2]);  // unused in this sequential version
    const char *matrixFile = argv[3];

    readMatrix(matrixFile);

    int *visited = calloc(numCities, sizeof(int));
    int *path    = malloc(numCities * sizeof(int));
    bestPath     = malloc(numCities * sizeof(int));

    visited[0] = 1;
    path[0]    = 0;

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    tsp(0, visited, path, 1, 0);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec  - t0.tv_sec)
                   + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    printf("Best path: ");
    for (int i = 0; i < numCities; i++) {
        printf("%d ", bestPath[i]);
    }
    printf("\nDistance: %d\n", bestDistance);
    printf("Elapsed time: %.6f seconds\n", elapsed);

    for (int i = 0; i < numCities; i++) {
        free(distMatrix[i]);
    }
    free(distMatrix);
    free(visited);
    free(path);
    free(bestPath);

    return EXIT_SUCCESS;
}