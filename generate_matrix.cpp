#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

void generateMatrix(int n, const std::string& filename) {
    std::vector<std::vector<int> > matrix(n, std::vector<int>(n, 0));
    srand(time(0));

    // Fill upper triangle and mirror to lower triangle
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int dist = 10 + rand() % 90; // Distance between 10 and 99
            matrix[i][j] = matrix[j][i] = dist;
        }
    }

    std::ofstream out(filename);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            out << matrix[i][j];
            if (j != n - 1) out << " ";
        }
        out << "\n";
    }

    out.close();
    std::cout << "Generated " << filename << " with " << n << " cities.\n";
}

int main() {
    int numCities;
    std::cout << "Enter number of cities: ";
    std::cin >> numCities;

    std::string filename = "cities" + std::to_string(numCities) + ".txt";
    generateMatrix(numCities, filename);

    return 0;
}
