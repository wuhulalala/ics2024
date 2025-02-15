#include <stdio.h>
#include <math.h>  

void test_math_functions() {
    float values[] = {1.5, -1.5, 2.0, -3.0, -2.0, 3.7, -3.7};  // 测试值
    int num_values = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < num_values; i++) {
        float val = values[i];
        printf("Value: %.2f, floor: %.2f, ceil: %.2f\n", 
               val, floor(val), ceil(val));
    }
}

int main() {
    test_math_functions();
    return 0;
}
