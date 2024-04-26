#include <stdio.h>

// Function to calculate binomial coefficient
int binomialCoeff(int n, int k) {
    int res = 1;
    if (k > n - k)
        k = n - k;
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}

// Function to print Pascal's Triangle
void printPascalTriangle(int numRows) {
    for (int line = 0; line < numRows; ++line) {
        for (int i = 0; i <= line; ++i)
            printf("%d ", binomialCoeff(line, i));
        printf("\n");
    }
}

int main() {
    int numRows;
    printf("Enter the number of rows for Pascal's Triangle: ");
    scanf("%d", &numRows);
    printf("Pascal's Triangle with %d rows:\n", numRows);
    printPascalTriangle(numRows);
    return 0;
}
