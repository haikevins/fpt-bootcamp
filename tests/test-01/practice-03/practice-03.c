#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// === ONLY CODE WITHIN THE DESIGNATED AREA . DO NOT MODIFY OTHER PARTS ===
int lastAntFall(int n, int* right2left, int rightSize, int* left2right, int leftSize) {
    int maxTime = 0;

    for (int i = 0; i < rightSize; i++) {
        if (right2left[i] > maxTime) {
            maxTime = right2left[i];
        }
    }

    for (int i = 0; i < leftSize; i++) {
        int time = n - left2right[i];
        if (time > maxTime) {
            maxTime = time;
        }
    }

    return maxTime;
}
// === DO NOT MODIFY OTHER PARTS ===

void parseLine(char* line, int* arr, int* size) {
    *size = 0;
    char* token = strtok(line, " \r\n\t");
    while (token) {
        arr[(*size)++] = atoi(token);
        token = strtok(NULL, " \r\n\t");
    }
}

int main() {
    char line[4096];
    if (!fgets(line, sizeof(line), stdin)) return 0;
    int n = atoi(line);

    char leftLine[4096] = "";
    char rightLine[4096] = "";
    if (!fgets(leftLine, sizeof(leftLine), stdin)) leftLine[0] = '\0';
    if (!fgets(rightLine, sizeof(rightLine), stdin)) rightLine[0] = '\0';

    int left[1000], right[1000];
    int leftSize = 0, rightSize = 0;
    parseLine(leftLine, left, &leftSize);
    parseLine(rightLine, right, &rightSize);

    printf("\n__JUDGE_RESULT__\n%d", lastAntFall(n, left, leftSize, right, rightSize));
    return 0;
}
