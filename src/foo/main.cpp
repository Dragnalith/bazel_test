#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Je suis foo!" << std::endl;
    std::cerr << "LOG: foo.exe is running properly" << std::endl;
    for (int i = 1; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
    return 0;
}
