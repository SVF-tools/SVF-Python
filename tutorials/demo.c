#include <string.h>
#include <stdio.h>
extern void* alloca(size_t);

int add_or_sub(int a, int b, int flag) {
    int result;
    if (flag) {
        result = a + b;
    } else {
        result = a - b;
    }
    return result;
}

int main() {
    int a = 5, b = 3;
    int* p = (int*)alloca(sizeof(int) * 2);  // alloca
    p[0] = a;                                // store + getelementptr
    p[1] = b;

    int x = p[0];                            // load + getelementptr
    int y = p[1];

    int result = add_or_sub(x, y, 1);        // call

    int* q = (int*)alloca(sizeof(int) * 2);
    memcpy(q, p, sizeof(int) * 2);           // copy (memcpy)

    printf("%d\n", result);
    return 0;
}
