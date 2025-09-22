#include "TerminalUtils.h"

#ifdef _WIN32
#include <conio.h>
std::string getHiddenPassword() {
    std::string password;
    char ch;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        } else {
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return password;
}
#else
#include <termios.h>
#include <unistd.h>
#include <iostream>

std::string getHiddenPassword() {
    termios oldt, newt;
    std::string password;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::getline(std::cin, password);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    std::cout << std::endl;
    return password;
}
#endif
