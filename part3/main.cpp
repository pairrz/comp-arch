#include <iostream>

extern "C" long long mul2();

int main() {
    long long result = mul2();
    std::cout << "5 * 6 = " << result << std::endl;
    return 0;
}
