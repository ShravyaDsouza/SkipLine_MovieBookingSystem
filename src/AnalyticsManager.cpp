#include "AnalyticsManager.h"
#include <iostream>
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <utility>
#include <iomanip> 

const std::string ANALYTICS_FILE = "data/analytics.txt";

AnalyticsManager::AnalyticsManager() {
    loadAnalyticsFromFile();
}

void AnalyticsManager::updateAnalytics(const Booking& booking, const Movie& movie) {
    data.ticketsSoldPerMovie[booking.m_id] += booking.seatsBooked;
    data.revenuePerMovie[booking.m_id] += booking.totalAmount;
    data.revenuePerTheater[booking.movieTheater] += booking.totalAmount;

    time_t now = std::time(nullptr);
    struct tm* localTime = std::localtime(&now);
    localTime->tm_hour = 0;
    localTime->tm_min = 0;
    localTime->tm_sec = 0;
    time_t dateOnly = std::mktime(localTime);
    data.bookingsPerDay[dateOnly]++;
}

void AnalyticsManager::printRevenuePerTheater() const {
    std::cout << "\n--- Revenue by Theater ---\n";
    for (const auto& [theater, revenue] : data.revenuePerTheater) {
        std::cout << theater << ": ₹" << std::fixed << std::setprecision(2) << revenue << "\n";
    }
}

void AnalyticsManager::printTicketsSoldPerMovie(const std::vector<Movie>& allMovies) const {
    std::cout << "\n--- Tickets Sold per Movie ---\n";
    for (const auto& [movieId, count] : data.ticketsSoldPerMovie) {
        std::string name = "(Unknown Movie)";
        for (const auto& m : allMovies) {
            if (m.m_id == movieId) {
                name = m.name;
                break;
            }
        }
        std::cout << name << " -> " << count << " tickets\n";
    }
}

void AnalyticsManager::displayTopMoviesByBookings(int topN, const std::vector<Movie>& allMovies) const {
    std::vector<std::pair<long, int>> sorted(data.ticketsSoldPerMovie.begin(), data.ticketsSoldPerMovie.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return b.second > a.second;
    });

    std::cout << "\n--- Top " << topN << " Movies by Bookings ---\n";
    for (int i = 0; i < std::min(topN, (int)sorted.size()); ++i) {
        std::string name = "(Unknown Movie)";
        for (const auto& m : allMovies) {
            if (m.m_id == sorted[i].first) {
                name = m.name;
                break;
            }
        }
        std::cout << name << " -> " << sorted[i].second << " bookings\n";
    }
}

void AnalyticsManager::displayRevenueReport() const {
    float total = 0.0f;
    for (const auto& [_, revenue] : data.revenuePerMovie) {
        total += revenue;
    }
    std::cout << "\n--- Revenue Report ---\n";
    std::cout << "Total Revenue: ₹" << std::fixed << std::setprecision(2) << total << "\n";
}

void AnalyticsManager::printBookingsPerDay() const {
    std::cout << "\n--- Bookings Per Day ---\n";
    for (const auto& [date, count] : data.bookingsPerDay) {
        std::tm* tmPtr = std::localtime(&date);
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", tmPtr);
        std::cout << buffer << " -> " << count << " bookings\n";
    }
}

void AnalyticsManager::loadAnalyticsFromFile() {
    std::ifstream in(ANALYTICS_FILE);
    if (!in.is_open()) return;

    data = Analytics{};  // Reset

    std::string line, section;
    while (std::getline(in, line)) {
        if (line == "[RevenuePerTheater]") section = line;
        else if (line == "[TicketsSoldPerMovie]") section = line;
        else if (line == "[RevenuePerMovie]") section = line;
        else if (line == "[BookingsPerDay]") section = line;
        else if (!line.empty()) {
            std::istringstream ss(line);
            if (section == "[RevenuePerTheater]") {
                std::string name; float val;
                ss >> std::quoted(name) >> val;
                data.revenuePerTheater[name] = val;
            } else if (section == "[TicketsSoldPerMovie]") {
                long id; int val;
                ss >> id >> val;
                data.ticketsSoldPerMovie[id] = val;
            } else if (section == "[RevenuePerMovie]") {
                long id; float val;
                ss >> id >> val;
                data.revenuePerMovie[id] = val;
            } else if (section == "[BookingsPerDay]") {
                time_t d; int val;
                ss >> d >> val;
                data.bookingsPerDay[d] = val;
            }
        }
    }
    in.close();
}

void AnalyticsManager::saveAnalyticsToFile() {
    std::ofstream out(ANALYTICS_FILE);
    if (!out.is_open()) return;

    out << "[RevenuePerTheater]\n";
    for (const auto& [name, val] : data.revenuePerTheater) {
        out << std::quoted(name) << " " << val << "\n";
    }

    out << "[TicketsSoldPerMovie]\n";
    for (const auto& [id, val] : data.ticketsSoldPerMovie) {
        out << id << " " << val << "\n";
    }

    out << "[RevenuePerMovie]\n";
    for (const auto& [id, val] : data.revenuePerMovie) {
        out << id << " " << val << "\n";
    }

    out << "[BookingsPerDay]\n";
    for (const auto& [d, val] : data.bookingsPerDay) {
        out << d << " " << val << "\n";
    }

    out.close();
}

void AnalyticsManager::reset() {
    data = Analytics{};
    saveAnalyticsToFile();
}

Analytics AnalyticsManager::getAnalytics() const {
    return data;
}
