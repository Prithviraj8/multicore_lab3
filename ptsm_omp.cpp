#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <omp.h>

int **dist;
int numCities;
int threadCount;

int **allocateMatrix(int size) {
    int **matrix = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        matrix[i] = (int *)malloc(size * sizeof(int));
    }
    return matrix;
}

void readMatrix(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    dist = allocateMatrix(numCities);
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            fscanf(file, "%d", &dist[i][j]);
        }
    }

    fclose(file);
}

// Recursive function to explore paths from current city
void dfs(int curr, int visited[], int path[], int index, int cost, int *bestCost, int bestPath[]) {
    if (index == numCities) {
        #pragma omp critical
        {
            if (cost < *bestCost) {
                *bestCost = cost;
                memcpy(bestPath, path, sizeof(int) * numCities);
            }
        }
        return;
    }

    for (int i = 0; i < numCities; i++) {
        if (!visited[i]) {
            visited[i] = 1;
            path[index] = i;
            dfs(i, visited, path, index + 1, cost + dist[curr][i], bestCost, bestPath);
            visited[i] = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: ./ptsm <numCities> <numThreads> <filename>\n");
        return 1;
    }

    numCities = atoi(argv[1]);
    threadCount = atoi(argv[2]);
    const char *filename = argv[3];

    readMatrix(filename);

    int globalBest = INT_MAX;
    int *finalPath = (int *)malloc(sizeof(int) * numCities);
    
    // Set number of threads
    omp_set_num_threads(threadCount);

    #pragma omp parallel
    {
        // Allocate thread-local arrays dynamically
        int *visited = (int *)malloc(numCities * sizeof(int));
        int *path = (int *)malloc(numCities * sizeof(int));
        int *localPath = (int *)malloc(numCities * sizeof(int));
        int localBest = INT_MAX;

        // Initialize first city (0) as visited
        memset(visited, 0, numCities * sizeof(int));
        visited[0] = 1;
        path[0] = 0;

        // Parallel loop with dynamic scheduling for better load balancing
        #pragma omp for schedule(dynamic, 1)
        for (int i = 1; i < numCities; i++) {
            visited[i] = 1;
            path[1] = i;
            dfs(i, visited, path, 2, dist[0][i], &localBest, localPath);
            visited[i] = 0;
        }

        // Update global best solution using critical section
        #pragma omp critical
        {
            if (localBest < globalBest) {
                globalBest = localBest;
                memcpy(finalPath, localPath, sizeof(int) * numCities);
            }
        }

        // Free thread-local memory
        free(visited);
        free(path);
        free(localPath);
    }

    // Print result
    printf("Best path: ");
    for (int i = 0; i < numCities; i++) {
        printf("%d ", finalPath[i]);
    }
    printf("\nDistance: %d\n", globalBest);

    // Cleanup
    for (int i = 0; i < numCities; i++) {
        free(dist[i]);
    }
    free(dist);
    free(finalPath);

    return 0;
}
