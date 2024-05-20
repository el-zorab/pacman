#include <iostream>
#include <fstream>
#include <string>

std::ifstream in("in.dat");

int main() {
    for (int j = 0; j < 31; j++) {
        for (int i = 0; i < 28; i++) {
            char c;
            in >> c;
            std::cout << int(c) - '0' << ' ';
        }
        std::cout << std::endl;
    }
    return 0;
}
