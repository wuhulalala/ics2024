#include <stdio.h>
#include <sys/time.h>
#include "NDL.h"

int main() {
    NDL_Init(0);

    uint32_t last_time = NDL_GetTicks(); // 获取初始时间

    while (1) {
        uint32_t current_time = NDL_GetTicks(); // 获取当前时间

        if (current_time - last_time >= 1000) {
            printf("current time (ms): %u\n", current_time);
            last_time = current_time; // 更新上次打印时间
        }

    }

    return 0;
}
