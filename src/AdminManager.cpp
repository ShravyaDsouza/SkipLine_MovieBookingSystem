#include "AdminManager.h"
#include <iostream>

void AdminManager::addMovie(MovieManager& movieManager, const TheaterManager& theaterManager) {
    Movie movie;
    movie.m_id = movieManager.generateUniqueMovieId();

    std::cin.ignore();  // Clear input buffer

    std::cout << "Enter Movie Name: ";
    std::getline(std::cin, movie.name);

    std::cout << "Enter Genre: ";
    std::getline(std::cin, movie.genre);

    std::cout << "Enter Language: ";
    std::getline(std::cin, movie.language);

    std::cout << "Enter Description: ";
    std::getline(std::cin, movie.description);

    std::cout << "Enter Duration (in hours): ";
    std::cin >> movie.duration;
    std::cin.ignore();

    std::cout << "Enter Age Limit: ";
    std::cin >> movie.ageLimit;
    std::cin.ignore();

    std::cout << "Enter Release Date (dd-mm-yyyy): ";
    std::getline(std::cin, movie.releaseDate);

    std::cout << "Enter Default Seat Type (e.g., Regular/VIP): ";
    std::getline(std::cin, movie.seat_type);

    std::cout << "Enter Ticket Base Price (₹): ";
    std::cin >> movie.ticketPrice;
    std::cin.ignore();

    int numSeatTypes;
    std::cout << "How many seat types (e.g., Regular, VIP)? ";
    std::cin >> numSeatTypes;
    std::cin.ignore();

    for (int i = 0; i < numSeatTypes; ++i) {
        std::string type;
        float price;
        std::cout << "Seat Type " << (i + 1) << ": ";
        std::getline(std::cin, type);
        std::cout << "Price for " << type << ": ₹";
        std::cin >> price;
        std::cin.ignore();
        movie.seatTypeToPrice[type] = price;
    }

    int numTimings;
    std::cout << "Enter number of show timings: ";
    std::cin >> numTimings;
    std::cin.ignore();

    for (int i = 0; i < numTimings; ++i) {
        std::string timing;
        int seats;
        std::cout << "Timing " << (i + 1) << ": ";
        std::getline(std::cin, timing);

        std::cout << "Seats available for " << timing << ": ";
        std::cin >> seats;
        std::cin.ignore();

        movie.showTimings.insert(timing);
        movie.seatsRemainingPerTiming[timing] = seats;
    }

    std::vector<MovieTheater> allTheaters = theaterManager.getAllTheaters();
    std::cout << "\nAvailable Theaters:\n";
    for (const auto& t : allTheaters) {
        std::cout << "- " << t.name << " (" << t.location << ")\n";
    }

    int numTheaters;
    std::cout << "How many theaters to assign this movie to? ";
    std::cin >> numTheaters;
    std::cin.ignore();

    for (int i = 0; i < numTheaters; ++i) {
        std::string selected;
        std::cout << "Enter theater name: ";
        std::getline(std::cin, selected);
        movie.theatersAvailable.push_back(selected);
    }

    movie.ratingInfo = MovieRating();

    movieManager.addMovie(movie);
    std::cout << "Movie added successfully!\n";
}

void AdminManager::updateMovie(MovieManager& movieManager, long movieId) {
    std::optional<Movie> optMovie = movieManager.getMovieById(movieId);
    if (!optMovie.has_value()) {
        std::cout << "Movie not found.\n";
        return;
    }

    Movie updated = optMovie.value(); 

    std::cin.ignore();
    std::string input;

    std::cout << "Enter new name (current: " << updated.name << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.name = input;

    std::cout << "Enter new genre (current: " << updated.genre << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.genre = input;

    std::cout << "Enter new language (current: " << updated.language << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.language = input;

    std::cout << "Enter new description (current: " << updated.description << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.description = input;

    std::cout << "Enter new seat type (current: " << updated.seat_type << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.seat_type = input;

    std::cout << "Do you want to update seat type-wise pricing? (y/n): ";
    std::getline(std::cin, input);
    if (input == "y" || input == "Y") {
        updated.seatTypeToPrice.clear();

        int types;
        std::cout << "How many seat types to enter? ";
        std::cin >> types;
        std::cin.ignore();

        for (int i = 0; i < types; ++i) {
            std::string seatType;
            float price;
            std::cout << "Enter seat type (e.g., Regular, VIP): ";
            std::getline(std::cin, seatType);
            std::cout << "Enter price for " << seatType << ": ₹";
            std::cin >> price;
            std::cin.ignore();

            updated.seatTypeToPrice[seatType] = price;
        }
    }

    std::cout << "Enter new duration in minutes (current: " << updated.duration << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.duration = std::stof(input);

    std::cout << "Enter new ticket price (current: ₹" << updated.ticketPrice << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.ticketPrice = std::stof(input);

    std::cout << "Enter new age limit (current: " << updated.ageLimit << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.ageLimit = std::stoi(input);

    std::cout << "Enter new release date (current: " << updated.releaseDate << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.releaseDate = input;

    std::cout << "Do you want to update the list of theaters showing this movie? (y/n): ";
    std::getline(std::cin, input);
    if (input == "y" || input == "Y") {
        updated.theatersAvailable.clear();

        int num;
        std::cout << "How many theaters to assign? ";
        std::cin >> num;
        std::cin.ignore();

        for (int i = 0; i < num; ++i) {
            std::string theaterName;
            std::cout << "Enter theater name: ";
            std::getline(std::cin, theaterName);
            updated.theatersAvailable.push_back(theaterName);
        }
    }

    std::cout << "Do you want to update show timings? (y/n): ";
    std::getline(std::cin, input);
    if (input == "y" || input == "Y") {
        updated.showTimings.clear();
        updated.seatsRemainingPerTiming.clear();

        int numTimings;
        std::cout << "Enter number of show timings: ";
        std::cin >> numTimings;
        std::cin.ignore();
        for (int i = 0; i < numTimings; ++i) {
            std::string timing;
            int seats;
            std::cout << "Timing " << (i + 1) << ": ";
            std::getline(std::cin, timing);
            std::cout << "Seats for " << timing << ": ";
            std::cin >> seats;
            std::cin.ignore();
            updated.showTimings.insert(timing);
            updated.seatsRemainingPerTiming[timing] = seats;
        }
    }

    movieManager.updateMovie(movieId, updated);
    std::cout << "Movie updated successfully.\n";
}


void AdminManager::deleteMovie(MovieManager& movieManager, long movieId) {
    auto movieOpt = movieManager.getMovieById(movieId);
    if (!movieOpt.has_value()) {
        std::cout << "Movie with ID " << movieId << " not found.\n";
        return;
    }

    const Movie& movie = movieOpt.value();
    std::cout << "Are you sure you want to delete the movie \"" << movie.name << "\"? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        movieManager.deleteMovie(movieId);
        std::cout << "Movie deleted successfully.\n";
    } else {
        std::cout << "Deletion cancelled.\n";
    }
}

// -- 
void AdminManager::addTheater(TheaterManager& theaterManager) {
    MovieTheater theater;

    std::cin.ignore();
    std::cout << "🎬 Enter Theater Name: ";
    std::getline(std::cin, theater.name);

    std::cout << "📍 Enter Theater Location: ";
    std::getline(std::cin, theater.location);

    std::cout << "📫 Enter Pincode: ";
    std::getline(std::cin, theater.pincode);

    int rows, cols;
    std::cout << "📐 Enter number of rows (max 26): ";
    std::cin >> rows;
    std::cout << "📏 Enter number of seats per row: ";
    std::cin >> cols;
    std::cin.ignore();

    std::set<std::string> availableTypes = {"Gold", "Silver", "Platinum"};
    std::map<char, std::string> rowToType;

    theater.totalSeats = rows * cols;
    theater.seatLayout.clear();
    theater.seatTypePerSeat.clear();

    std::cout << "\n💺 Available Seat Types: ";
    for (const auto& t : availableTypes) std::cout << t << " ";
    std::cout << "\n";

    for (int i = 0; i < rows; ++i) {
        char rowChar = 'A' + i;
        std::string type;
        std::cout << "Enter seat type for row " << rowChar << ": ";
        std::getline(std::cin, type);
        while (availableTypes.find(type) == availableTypes.end()) {
            std::cout << "Invalid type. Please enter Gold, Silver, or Platinum: ";
            std::getline(std::cin, type);
        }
        rowToType[rowChar] = type;

        for (int j = 1; j <= cols; ++j) {
            std::string seat = std::string(1, rowChar) + std::to_string(j);
            theater.seatLayout.push_back(seat);
            theater.seatTypePerSeat[seat] = type;
        }
    }

    int showCount;
    std::cout << "\n Enter number of show timings: ";
    std::cin >> showCount;
    std::cin.ignore();
    for (int i = 0; i < showCount; ++i) {
        std::string timing;
        std::cout << "Enter timing " << i + 1 << ": ";
        std::getline(std::cin, timing);
        for (const auto& seat : theater.seatLayout) {
            theater.seatAvailabilityPerShowTime[timing][seat] = true;
        }
    }

    std::cout << "\n Previewing Seating Layout:\n\n";
    std::cout << "        [Screen This Side]\n";
    for (int i = 0; i < rows; ++i) {
        char rowChar = 'A' + i;
        std::cout << rowChar << "  ";
        for (int j = 1; j <= cols; ++j) {
            std::string seat = std::string(1, rowChar) + std::to_string(j);
            std::string type = theater.seatTypePerSeat[seat];

            if (type == "Gold") std::cout << "[G]";
            else if (type == "Silver") std::cout << "[S]";
            else if (type == "Platinum") std::cout << "[P]";
            else std::cout << "[ ]";
        }
        std::cout << "  (" << rowToType[rowChar] << ")\n";
    }
    std::cout << "\nLegend: [G] Gold  [S] Silver  [P] Platinum\n\n";

    std::string confirm;
    std::cout << "Do you want to save this theater? (y/n): ";
    std::getline(std::cin, confirm);
    if (confirm == "y" || confirm == "Y") {
        theaterManager.addTheater(theater);
        std::cout << "Theater added successfully.\n";
    } else {
        std::cout << "Theater creation cancelled.\n";
    }
}

void AdminManager::viewAllBookings(const BookingManager& bookingManager) const {
    auto bookings = bookingManager.getAllBookings();
    std::cout << "\n--- All Bookings ---\n";
    for (const auto& booking : bookings) {
        std::cout << "Booking ID: " << booking.b_id
                  << ", User ID: " << booking.u_id
                  << ", Movie ID: " << booking.m_id
                  << ", Status: " << booking.status
                  << ", Seats: " << booking.seatsBooked
                  << ", Timing: " << booking.showTiming << "\n";
    }
}

void AdminManager::viewAllUsers(const UserManager& userManager) const {
    auto users = userManager.getAllUsers();
    std::cout << "\n--- Registered Users ---\n";
    for (const auto& user : users) {
        std::cout << "ID: " << user.u_id << ", Name: " << user.name
                  << ", Email: " << user.email
                  << (user.isAdmin ? " (Admin)" : "") << "\n";
    }
}

void AdminManager::deleteUser(UserManager& userManager, long userId) {
    userManager.deleteUser(userId);
    std::cout << "✅ User deleted successfully.\n";
}

void AdminManager::createPromoCode(PromoManager& promoManager) {
    PromoCode promo;
    std::cin.ignore();
    std::cout << "Enter Promo Code: ";
    std::getline(std::cin, promo.code);
    std::cout << "Enter Discount (%): ";
    std::cin >> promo.discount;
    std::cin.ignore();

    promoManager.addPromo(promo);
    std::cout << "✅ Promo code created.\n";
}

void AdminManager::viewAnalytics(const AnalyticsManager& analyticsManager) const {
    analyticsManager.printBookingAnalytics();
}

void AdminManager::addMovie(MovieManager& movieManager) {
    Movie movie;
    movie.m_id = movieManager.generateUniqueMovieId();

    std::cin.ignore();
    std::cout << "Enter Movie Name: ";
    std::getline(std::cin, movie.name);

    std::cout << "Enter Genre: ";
    std::getline(std::cin, movie.genre);

    std::cout << "Enter Language: ";
    std::getline(std::cin, movie.language);

    std::cout << "Enter Theater Location: ";
    std::getline(std::cin, theater.location);

    int numTimings;
    std::cout << "Enter number of show timings: ";
    std::cin >> numTimings;
    std::cin.ignore();

    for (int i = 0; i < numTimings; ++i) {
        std::string timing;
        int seats;
        std::cout << "Timing " << (i + 1) << ": ";
        std::getline(std::cin, timing);
        std::cout << "Seats for " << timing << ": ";
        std::cin >> seats;
        std::cin.ignore();
        movie.seatsRemainingPerTiming[timing] = seats;
    }

    std::cout << "Enter Ticket Price (₹): ";
    std::cin >> movie.ticketPrice;
    std::cin.ignore();

    movieManager.addMovie(movie);
    std::cout << "✅ Movie added successfully.\n";
}

void AdminManager::updateMovie(MovieManager& movieManager, long movieId) {
    auto movieOpt = movieManager.getMovieById(movieId);
    if (!movieOpt.has_value()) {
        std::cout << "❌ Movie not found.\n";
        return;
    }
    Movie movie = movieOpt.value();

    std::cin.ignore();
    std::cout << "Enter new name (current: " << movie.name << "): ";
    std::getline(std::cin, movie.name);

    std::cout << "Enter new genre: ";
    std::getline(std::cin, movie.genre);

    std::cout << "Enter new language: ";
    std::getline(std::cin, movie.language);

    std::cout << "Enter new ticket price: ";
    std::cin >> movie.ticketPrice;
    std::cin.ignore();

    movieManager.updateMovie(movieId, movie);
    std::cout << "✅ Movie updated.\n";
}

void AdminManager::deleteMovie(MovieManager& movieManager, long movieId) {
    movieManager.deleteMovie(movieId);
    std::cout << "✅ Movie deleted successfully.\n";
}

void AdminManager::addTheater(TheaterManager& theaterManager) {
    MovieTheater theater;
    std::cin.ignore();
    std::cout << "Enter Theater Name: ";
    std::getline(std::cin, theater.name);
    std::cout << "Enter Location: ";
    std::getline(std::cin, theater.location);
    std::cout << "Enter Pincode: ";
    std::getline(std::cin, theater.pincode);
    std::cout << "Enter Total Seats: ";
    std::cin >> theater.totalSeats;
    std::cin.ignore();

    theaterManager.addTheater(theater);
    std::cout << "✅ Theater added successfully.\n";
}

void AdminManager::updateTheaterDetails(TheaterManager& theaterManager) {
    std::cin.ignore();
    std::string name;
    std::cout << "Enter the name of the theater to update: ";
    std::getline(std::cin, name);

    MovieTheater* theater = theaterManager.getTheaterByName(name);
    if (!theater) {
        std::cout << "Theater not found.\n";
        return;
    }

    MovieTheater updated = *theater; 
    std::string input;

    std::cout << "\nUpdating Theater: " << name << "\n";

    std::cout << "📍 Current location: " << updated.location << "\n";
    std::cout << "Enter new location (or press Enter to keep current): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.location = input;

    std::cout << "📫 Current pincode: " << updated.pincode << "\n";
    std::cout << "Enter new pincode (or press Enter to keep current): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.pincode = input;

    std::cout << "💺 Current total seats: " << updated.totalSeats << "\n";
    std::cout << "Enter new total seats (or press Enter to keep current): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.totalSeats = std::stoi(input);

    std::cout << "💰 Current service charge: ₹" << updated.serviceCharge << "\n";
    std::cout << "Enter new service charge (or press Enter to keep current): ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.serviceCharge = std::stof(input);

    std::cout << "🪑 Do you want to update the seat layout and tiers? (y/n): ";
    std::getline(std::cin, input);
    if (input == "y" || input == "Y") {
        int rows, cols;
        std::cout << "Enter number of rows: ";
        std::cin >> rows;
        std::cout << "Enter number of seats per row: ";
        std::cin >> cols;
        std::cin.ignore();

        updated.seatLayout.clear();
        updated.seatTypePerSeat.clear();

        for (int i = 0; i < rows; ++i) {
            char rowLabel = 'A' + i;
            for (int j = 1; j <= cols; ++j) {
                std::string seat = rowLabel + std::to_string(j);
                updated.seatLayout.push_back(seat);

                std::cout << "Enter seat type (Gold/Silver/Platinum) for " << seat << ": ";
                std::getline(std::cin, input);
                updated.seatTypePerSeat[seat] = input;
            }
        }

        updated.totalSeats = rows * cols;
    }

    for (auto& [showTime, availabilityMap] : updated.seatAvailabilityPerShowTime) {
        for (const auto& seat : updated.seatLayout) {
            if (availabilityMap.find(seat) == availabilityMap.end()) {
                availabilityMap[seat] = true;
            }
        }
    }

    theaterManager.updateTheaterDetails(name, updated);
}


void AdminManager::deleteTheater(TheaterManager& theaterManager) {
    std::cin.ignore();
    std::string name;
    std::cout << "Enter the name of the theater to delete: ";
    std::getline(std::cin, name);

    std::cout << "Are you sure you want to delete theater \"" << name << "\"? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Deletion cancelled by admin.\n";
        return;
    }

    bool deleted = theaterManager.deleteTheater(name);
    if (deleted) {
        std::cout << "Theater deleted successfully.\n";
    } else {
        std::cout << "Theater not found.\n";
    }
}


void AdminManager::viewAllBookings(const BookingManager& bookingManager) const {
    auto bookings = bookingManager.getAllBookings();
    std::cout << "\n--- All Bookings ---\n";
    for (const auto& booking : bookings) {
        std::cout << "Booking ID: " << booking.b_id
                  << ", User ID: " << booking.u_id
                  << ", Movie ID: " << booking.m_id
                  << ", Status: " << static_cast<int>(booking.status)
                  << ", Seats: " << booking.seatsBooked
                  << ", Timing: " << booking.showTiming << "\n";
    }
}

void AdminManager::viewBookingById(const BookingManager& bookingManager, long bookingId) const {
    auto bookingOpt = bookingManager.getBookingById(bookingId);
    if (bookingOpt.has_value()) {
        const Booking& booking = bookingOpt.value();
        std::cout << "Booking ID: " << booking.b_id << ", User ID: " << booking.u_id
                  << ", Movie ID: " << booking.m_id << ", Timing: " << booking.showTiming
                  << ", Status: " << static_cast<int>(booking.status) << ", Seats: " << booking.seatsBooked << "\n";
    } else {
        std::cout << "❌ Booking not found.\n";
    }
}

//------------- User Manager ------------------
void AdminManager::viewAllUsers(const UserManager& userManager) const {
    auto users = userManager.getAllUsers();
    std::cout << "\n--- Registered Users ---\n";
    for (const auto& user : users) {
        std::cout << "ID: " << user.u_id << ", Name: " << user.name
                  << ", Email: " << user.email
                  << (user.isAdmin ? " (Admin)" : "") << "\n";
    }
}
void AdminManager::viewUserDetails(UserManager& userManager, long userId) const {
    User* user = userManager.getUserById(userId);
    if (user) {
        std::cout << "ID: " << user->u_id << "\n";
        std::cout << "Name: " << user->name << "\n";
        std::cout << "Email: " << user->email << "\n";
        std::cout << "Phone: " << user->phoneno << "\n";
        std::cout << "Wallet: ₹" << user->walletBalance << "\n";
        std::cout << "Membership: " << user->membershipLevel << "\n";
        std::cout << "Loyalty Points: " << user->loyaltyPoints << "\n";
        std::cout << "Is Admin: " << (user->isAdmin ? "Yes" : "No") << "\n";
        std::cout << "\nBooking History:\n";
        if (user->bookingHistory.empty()) {
            std::cout << "No bookings yet.\n";
        } else {
            for (long bookingId : user->bookingHistory) {
                std::cout << "- Booking ID: " << bookingId << "\n";
            }
        }
    } else {
        std::cout << "User not found.\n";
    }
}

void AdminManager::deleteUser(UserManager& userManager, long userId) {
    userManager.deleteUser(userId);
}

//------------- Promo Manager ------------------
void AdminManager::createPromoCode(PromoManager& promoManager) {
    PromoCode promo;
    std::cin.ignore();
    std::cout << "Enter Promo Code: ";
    std::getline(std::cin, promo.code);

    std::cout << "Enter Discount Percentage: ";
    std::cin >> promo.discountPercent;

    std::cout << "Enter Usage Limit: ";
    std::cin >> promo.usageLimit;

    std::cout << "Enter Minimum Purchase Amount (₹): ";
    std::cin >> promo.minPurchaseAmount;

    std::cin.ignore();
    std::string expiryDateStr;
    std::cout << "Enter Expiry Date (YYYY-MM-DD): ";
    std::getline(std::cin, expiryDateStr);

    struct tm tm{};
    strptime(expiryDateStr.c_str(), "%Y-%m-%d", &tm);
    promo.expiryDate = mktime(&tm);

    promo.createdWhen = time(nullptr);
    promo.isValid = true;

    promoManager.addPromo(promo);
}

void AdminManager::viewAllPromos(const PromoManager& promoManager) const {
    promoManager.viewPromo();
}

void AdminManager::removePromo(PromoManager& promoManager) {
    std::cin.ignore();
    std::string code;
    std::cout << "Enter Promo Code to Expire: ";
    std::getline(std::cin, code);
    promoManager.removePromo(code);
}

//--------------- Analytics Manager -------------------
void AdminManager::viewAnalytics(const AnalyticsManager& analyticsManager) const {
    int choice;
    do {
        std::cout << "\n===== Analytics Dashboard =====\n";
        std::cout << "1. View Revenue Report\n";
        std::cout << "2. View Revenue per Theater\n";
        std::cout << "3. View Tickets Sold per Movie\n";
        std::cout << "4. View Bookings per Day\n";
        std::cout << "5. View Top N Movies by Bookings\n";
        std::cout << "0. Back to Admin Menu\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                analyticsManager.displayRevenueReport();
                break;
            case 2:
                analyticsManager.printRevenuePerTheater();
                break;
            case 3: {
                std::vector<Movie> allMovies; 
                analyticsManager.printTicketsSoldPerMovie(allMovies);
                break;
            }
            case 4:
                analyticsManager.printBookingsPerDay();
                break;
            case 5: {
                int topN;
                std::cout << "Enter N: ";
                std::cin >> topN;
                std::vector<Movie> allMovies;
                analyticsManager.displayTopMoviesByBookings(topN, allMovies);
                break;
            }
            case 0:
                std::cout << "Returning to Admin Menu...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);
}

void AdminManager::resetAnalytics(AnalyticsManager& analyticsManager) {
    analyticsManager.reset();
    std::cout << "✅ Analytics data reset.\n";
}
