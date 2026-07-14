#include <stdio.h>
#include <stdlib.h>

// === ONLY CODE WITHIN THE DESIGNATED AREA . DO NOT MODIFY OTHER PARTS ===
int islandPerimeter(int** grid, int rowSize, int* colSizes) {
    int perimeter = 0;

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSizes[i]; j++) {
            if (grid[i][j] == 1) {
                perimeter += 4;

                if (j + 1 < colSizes[i] && grid[i][j + 1] == 1) {
                    perimeter -= 2;
                }

                if (i + 1 < rowSize && grid[i + 1][j] == 1) {
                    perimeter -= 2;
                }
            }
        }
    }

    return perimeter;
}
// === DO NOT MODIFY OTHER PARTS ===

int main() {
    int rows, cols;
    scanf("%d %d", &rows, &cols);
    int** grid = (int**)malloc(rows * sizeof(int*));
    int* colSizes = (int*)malloc(rows * sizeof(int));
    for (int i = 0; i < rows; i++) {
        grid[i] = (int*)malloc(cols * sizeof(int));
        colSizes[i] = cols;
        for (int j = 0; j < cols; j++) scanf("%d", &grid[i][j]);
    }
    printf("\n__JUDGE_RESULT__\n%d", islandPerimeter(grid, rows, colSizes));
    for (int i = 0; i < rows; i++) free(grid[i]);
    free(grid); free(colSizes);
    return 0;
}
