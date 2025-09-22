#include "Logger.h"
#include <fstream>
#include <ctime>
#include <iomanip>

void Logger::log(const std::string& message) {
    std::ofstream logFile("logs/system.log", std::ios::app);
    if (!logFile) return;

    time_t now = time(nullptr);
    tm* timeInfo = localtime(&now);

    logFile << "[" << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S") << "] ";
    logFile << message << std::endl;

    logFile.close();
}
