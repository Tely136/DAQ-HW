#include <stdio.h>

void example(int* a, int N);

int main() {
    int i;

    int N = 50;
    int A[N];
    int B[N];

    for (i = 0; i < N; i++) {
        A[i] = i;
    }

    example(B,N);

    // Compare results
    for (i = 0; i < N; i++) {
        if (B[i] != A[i]) {
            printf("i = %d A = %d B= %d\n", i, A[i], B[i]);
            printf("ERROR HW and SW results mismatch\n");
            return 1;
        }
    }
    printf("Success HW and SW results match\n");
    return 0;
}