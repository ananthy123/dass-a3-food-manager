#include <iostream>
#include <memory>
#include <stdexcept>
#include "CLI/CLIManager.h"

int main() {
    try {
        diet::CLIManager cliManager;
        cliManager.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
