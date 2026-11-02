#include "Utils.h"
#include <limits>
#include <ctime>
#include <algorithm>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif

namespace Utils {

void enableVirtualTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printDivider(char ch, int length) {
    std::cout << CYAN << std::string(length, ch) << RESET << "\n";
}

void printHeader(const std::string& title) {
    clearScreen();
    printDivider('=', 74);
    std::cout << BOLD << YELLOW << "   MCA COMPUTER APTITUDE ASSESSMENT & EVALUATION SYSTEM (CU)\n" << RESET;
    printDivider('-', 74);
    std::cout << BOLD << CYAN << "   >> " << title << "\n" << RESET;
    printDivider('=', 74);
    std::cout << "\n";
}

void pause() {
    std::cout << "\n" << YELLOW << "Press Enter to continue..." << RESET;
    std::string dummy;
    std::getline(std::cin, dummy);
}

int getSafeInt(int minVal, int maxVal, const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (val >= minVal && val <= maxVal) {
                return val;
            }
            std::cout << RED << " [!] Please enter a number between " << minVal << " and " << maxVal << ".\n" << RESET;
        } else {
            std::cout << RED << " [!] Invalid input! Please enter an integer.\n" << RESET;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

char getSafeChar(const std::string& validOptions, const std::string& prompt) {
    std::string line;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, line);
        line = trim(line);
        if (line.length() == 1) {
            char ch = static_cast<char>(std::toupper(line[0]));
            if (validOptions.find(ch) != std::string::npos) {
                return ch;
            }
        }
        std::cout << RED << " [!] Invalid option! Valid choices: (" << validOptions << ")\n" << RESET;
    }
}

std::string getSafeString(const std::string& prompt) {
    std::string str;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, str);
        str = trim(str);
        if (!str.empty()) {
            return str;
        }
        std::cout << RED << " [!] Field cannot be empty. Please enter again.\n" << RESET;
    }
}

std::string getSafePassword(const std::string& prompt) {
    std::cout << prompt;
    std::string password;
#ifdef _WIN32
    if (!_isatty(_fileno(stdin))) {
        // Piped or automated input
        std::getline(std::cin, password);
        return trim(password);
    }
    char ch;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        } else if (ch >= 32 && ch <= 126) {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << "\n";
#else
    std::getline(std::cin, password);
#endif
    return trim(password);
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

std::string getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    char buf[80];
    struct tm timeinfo;
#ifdef _WIN32
    localtime_s(&timeinfo, &now);
#else
    localtime_r(&now, &timeinfo);
#endif
    std::strftime(buf, sizeof(buf), "%d-%b-%Y %H:%M:%S", &timeinfo);
    return std::string(buf);
}

} // namespace Utils
