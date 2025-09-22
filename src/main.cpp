#include "AdminManager.h"
#include "PromoManaher.h"
#include "UserManager.h"
#include "Validator.h"
#include "TerminalUtils.h"
#include "Hasher.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <set>
#include <thread>
#include <chrono>
#include <utility>
#include <filesystem>

using namespace std;
using namespace std::chrono;

long generateUserId() {
    static long id = 1000;
    return id++;
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void setupSuperAdmin() {
    string email, password;
    cout << "\nSuper Admin Setup\n";

    cout << "Enter admin email: ";
    cin.ignore(); 
    getline(cin, email);
    while (!isValidEmail(email)) {
        cout << "Invalid email format. Please re-enter: ";
        getline(cin, email);
    }

    cout << "Enter password: ";
    password = getHiddenPassword();

    ofstream outFile("data/admin.txt");
    outFile << email << "\n" << hashPassword(password);
    outFile.close();

    cout << "Super Admin saved successfully.\n";
}

bool isAdminFileValid() {
    ifstream file("data/admin.txt");
    string email, hash;
    if (!file.is_open()) return false;
    getline(file, email);
    getline(file, hash);
    return !(email.empty() || hash.empty());
}

pair<string, string> loadAdminCredentials() {
    ifstream file("data/admin.txt");
    string email, hash;
    getline(file, email);
    getline(file, hash);
    return {email, hash};
}

void initializeSuperAdminIfNeeded() {
    if (!filesystem::exists("data/admin.txt") || !isAdminFileValid()) {
        cout << "\nAdmin credentials are corrupt.\n";
        setupSuperAdmin();
    } else {
        char choice;
        cout << "\nAdmin credentials already exist.\n";
        cout << "Do you want to overwrite and reconfigure Super Admin? (y/n): ";
        cin >> choice;
        cin.ignore();
        if (tolower(choice) == 'y') {
            setupSuperAdmin();
        }
    }
}

void showPromoCodeMenu(){
    while(true){
         cout << "\n===== Promo Codes Manager =====\n";
         cout << "1. Add Promo Code\n";
         cout << "2. Delete Promo Code\n";
         cout << "3. View All Promo Codes\n";
         cout << "0. Exit\n";
         cout << "=============================\n";
         cout << "Enter your choice: ";

         int choice;
         cin >> choice;
         cin.ignore();
         switch(choice){
         case 1:
            adminManager.createPromoCode(promoManager);
            break;
         case 2:
            adminManager.viewAllPromos(promoManager);
            break;
         case 3:
            adminManager.removePromo(promoManager);
            break;
         case 0: cout << "Exiting...\n"; return;
         default: cout << "Invalid choice. Try again.\n";
    }
}
}
void showAdminMainMenu() {
    while (true) {
        cout << "\n===== Admin Main Menu =====\n";
        cout << "1. Movie Management\n";
        cout << "2. Theater Management\n";
        cout << "3. Promo Code Management\n";
        cout << "4. Analytics & Reports\n";
        cout << "5. Booking Management\n";
        cout << "6. User Management\n";
        cout << "0. Logout\n";
        cout << "=============================\n";
        cout << "Enter your choice: ";

        int mainChoice;
        cin >> mainChoice;
        cin.ignore();

        switch (mainChoice) {
            case 1: showMovieManagementMenu(); break;
            case 2: showTheaterManagementMenu(); break;
            case 3: showPromoCodeMenu(); break;
            case 4: showAnalyticsMenu(); break;
            case 5: showBookingManagementMenu(); break;
            case 6: showUserManagementMenu(); break;
            case 0: cout << "Logging out...\n"; return;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}

int main() {
    UserManager userManager;
    AdminManager adminManager;
    PromoManager promoManager;

    set<string> lockedEmailsThisSession;

    ifstream blacklistedFile("data/blacklist.txt");
    set<string> blacklistedEmails;
    string line;
    while (getline(blacklistedFile, line)) blacklistedEmails.insert(line);
    blacklistedFile.close();

    int choice;
    initializeSuperAdminIfNeeded();

    while (true) {
        cout << "\n🎬 Movie Booking System\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            User newUser;
            newUser.u_id = generateUserId();

            cout << "Enter Name: ";
            cin.ignore(); getline(cin, newUser.name);

            cout << "Enter Phone Number: ";
            getline(cin, newUser.phoneno);
            while (!isValidPhone(newUser.phoneno)) {
                cout << "Invalid phone number. Enter again: ";
                getline(cin, newUser.phoneno);
            }

            cout << "Enter Email: ";
            getline(cin, newUser.email);
            while (!isValidEmail(newUser.email)) {
                cout << "Invalid email. Enter again: ";
                getline(cin, newUser.email);
            }

            cout << "Enter Password: ";
            string rawPass = getHiddenPassword();
            newUser.hashPassword = hashPassword(rawPass);

            newUser.isAdmin = false;
            newUser.type = "user";

            if (userManager.registerUser(newUser)) {
                cout << "Registration successful!\n";
            } else {
                cout << "Registration failed. Email already exists.\n";
            }

        } else if (choice == 2) {
            int attempts = 0;
            string email, password;
            User* loggedInUser = nullptr;

            cin.ignore();
            cout << "Enter Email: ";
            getline(cin, email);
            while (!isValidEmail(email)) {
                cout << "Invalid email format. Please re-enter: ";
                getline(cin, email);
            }

            auto [adminEmail, adminHash] = loadAdminCredentials();
            if (email == adminEmail) {
                int adminAttempts = 0;
                while (adminAttempts < 3) {
                    cout << "Enter Password: ";
                    password = getHiddenPassword();

                    if (hashPassword(password) == adminHash) {
                        cout << "\n✅ Super Admin logged in.\n";
                        cout << "[Admin Mode Active]\n";
                        showAdminMenu();
                        break;
                    } else {
                        adminAttempts++;
                        cout << "Incorrect password. Attempts left: " << (3 - adminAttempts) << "\n";
                    }
                }
                if (adminAttempts == 3) {
                    cout << "Too many failed admin login attempts.\n";
                }
                continue;
            }

            while (attempts < 3 && !loggedInUser) {
                cout << "Enter Password: ";
                password = getHiddenPassword();

                loggedInUser = userManager.loginUser(email, password);

                if (!loggedInUser) {
                    attempts++;
                    Logger::log("Login Failed for email: " + email + " [Attempt " + to_string(attempts) + "]");
                    if (attempts < 3)
                        cout << "Incorrect password. Attempts left: " << (3 - attempts) << endl;
                }
            }

            if (loggedInUser) {
                Logger::log("Login Success: " + loggedInUser->email);
                cout << "\nLogin successful. Welcome, " << loggedInUser->name << "!\n";
                cout << "[User Mode Active]\n";
                continue;
            } else {
                Logger::log("ALERT: Repeated login failures for email: " + email);
                lockedEmailsThisSession.insert(email);
                ofstream blacklistFile("data/blacklist.txt", ios::app);
                blacklistFile << email << endl;
                blacklistFile.close();
                cout << "Too many failed attempts. Account temporarily/persistently locked.\n";
                this_thread::sleep_for(seconds(5));
            }

        } else if (choice == 3) {
            cout << "Exiting... Goodbye!\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
