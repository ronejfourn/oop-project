#include <iostream>

namespace Logger {
    static void LogError(const char *msg, int err_code = -1) {
        std::cout << "[ERROR]: " << msg << std::endl;
        exit(err_code);
    }

    static void LogWarning(const char *msg) {
        std::cout << "[WARNING]: " << msg << std::endl;
    }
}
