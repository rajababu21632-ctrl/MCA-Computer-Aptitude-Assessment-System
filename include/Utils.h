#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>

namespace Utils {
    // Console color utilities
    void enableVirtualTerminal();
    void clearScreen();
    void printHeader(const std::string& title);
    void printDivider(char ch = '=', int length = 70);
    void pause();

    // Color codes
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[1;31m";
    const std::string GREEN   = "\033[1;32m";
    const std::string YELLOW  = "\033[1;33m";
    const std::string BLUE    = "\033[1;34m";
    const std::string CYAN    = "\033[1;36m";
    const std::string MAGENTA = "\033[1;35m";
    const std::string BOLD    = "\033[1m";

    // Safe input handling
    int getSafeInt(int minVal, int maxVal, const std::string& prompt = "Enter choice: ");
    char getSafeChar(const std::string& validOptions, const std::string& prompt = "Enter option: ");
    std::string getSafeString(const std::string& prompt = "Enter value: ");
    std::string getSafePassword(const std::string& prompt = "Enter password: ");

    // String helpers
    std::string trim(const std::string& str);
    std::string getCurrentDateTime();
}

#endif // UTILS_H
