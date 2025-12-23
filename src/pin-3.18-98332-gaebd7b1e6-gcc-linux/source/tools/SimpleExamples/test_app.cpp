#include <iostream>

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main() {
    std::cout << "计算阶乘:" << std::endl;
    
    for (int i = 1; i <= 5; i++) {
        std::cout << i << "! = " << factorial(i) << std::endl;
    }
    
    // 一些条件分支来测试基本块覆盖
    int x = 10;
    if (x > 5) {
        std::cout << "x 大于 5" << std::endl;
    } else {
        std::cout << "x 小于等于 5" << std::endl;
    }
    
    // 循环来创建更多基本块
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            sum += i;
        }
    }
    std::cout << "偶数和: " << sum << std::endl;
    
    return 0;
}
