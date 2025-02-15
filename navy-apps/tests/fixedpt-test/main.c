#include <stdio.h>
#include <stdint.h>
#include "fixedptc.h"  // 假设你的头文件在这个路径下

void test_fixedpt_functions() {
    fixedpt a = fixedpt_rconst(1.5);  // 1.5 转换为 fixedpt
    fixedpt b = fixedpt_rconst(2.5);  // 2.5 转换为 fixedpt
    fixedpt c = fixedpt_rconst(-3.0); // -3.0 转换为 fixedpt

    // 测试乘法
    printf("expected: 3    actual: %d fixedpt_muli(a, 2) = %d\n", 
           fixedpt_rconst(3.0), fixedpt_muli(a, 2) >> 8); // 1.5 * 2 = 3.0
    printf("expected: 3.75 actual: %d fixedpt_mul(a, b) = %d\n", 
           fixedpt_rconst(3.75), fixedpt_mul(a, b) >> 8);   // 1.5 * 2.5 = 3.75

    // 测试除法
    printf("expected: 0.75 actual: %d fixedpt_divi(a, 2) = %d\n", 
           fixedpt_rconst(0.75), fixedpt_divi(a, 2) >> 8); // 1.5 / 2 = 0.75
    printf("expected: 0.6  actual: %d fixedpt_div(a, b) = %d\n", 
           fixedpt_rconst(0.6), fixedpt_div(a, b) >> 8);   // 1.5 / 2.5 = 0.6

    // 测试绝对值
    printf("expected: 3    actual: %d fixedpt_abs(c) = %d\n", 
           fixedpt_rconst(3.0), fixedpt_abs(c) >> 8);         // |-3.0| = 3.0

    // 测试向下取整
    printf("expected: 1    actual: %d fixedpt_floor(a) = %d\n", 
           fixedpt_rconst(1.0), fixedpt_floor(a) >> 8);     // floor(1.5) = 1.0
    printf("expected: -3   actual: %d fixedpt_floor(c) = %d\n", 
           fixedpt_rconst(-3.0), fixedpt_floor(c) >> 8);     // floor(-3.0) = -3.0

    // 测试向上取整
    printf("expected: 2    actual: %d fixedpt_ceil(a) = %d\n", 
           fixedpt_rconst(2.0), fixedpt_ceil(a) >> 8);       // ceil(1.5) = 2.0
    printf("expected: -3   actual: %d fixedpt_ceil(c) = %d\n", 
           fixedpt_rconst(-3.0), fixedpt_ceil(c) >> 8);       // ceil(-3.0) = -3.0
}

int main() {
    test_fixedpt_functions();
    return 0;
}
