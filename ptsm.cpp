#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int **distMatrix; // The distance matrix
int numCities;    // Number of cities
int bestDistance = INT_MAX;
int *bestPath;

// Utility to allocate a 2D distance matrix
int **allocateMatrix(int size) {
    int **matrix = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        matrix[i] = (int *)malloc(size * sizeof(int));
    }
    return matrix;
}

// Read distance matrix from a file
void readMatrix(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File open failed");
        exit(1);
    }

    distMatrix = allocateMatrix(numCities);

    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            fscanf(file, "%d", &distMatrix[i][j]);
        }
    }

    fclose(file);
}

// Recursively explore all paths using backtracking
void tsp(int currCity, int visited[], int path[], int pathIndex, int currDist) {
    if (pathIndex == numCities) {
        // Reached end, update best distance if needed
        if (currDist < bestDistance) {
            bestDistance = currDist;
            memcpy(bestPath, path, sizeof(int) * numCities);
        }
        return;
    }

    for (int i = 0; i < numCities; i++) {
        if (!visited[i]) {
            visited[i] = 1;
            path[pathIndex] = i;
            tsp(i, visited, path, pathIndex + 1, currDist + distMatrix[currCity][i]);
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
    int numThreads = atoi(argv[2]); // unused for now
    char *filename = argv[3];

    readMatrix(filename);

    // Init visited and path
    int *visited = (int *)calloc(numCities, sizeof(int));
    int *path = (int *)malloc(numCities * sizeof(int));
    bestPath = (int *)malloc(numCities * sizeof(int));

    visited[0] = 1;
    path[0] = 0;

    tsp(0, visited, path, 1, 0);

    // Print result
    printf("Best path: ");
    for (int i = 0; i < numCities; i++) {
        printf("%d ", bestPath[i]);
    }
    printf("\nDistance: %d\n", bestDistance);

    // Cleanup
    for (int i = 0; i < numCities; i++) {
        free(distMatrix[i]);
    }
    free(distMatrix);
    free(visited);
    free(path);
    free(bestPath);

    return 0;
}
