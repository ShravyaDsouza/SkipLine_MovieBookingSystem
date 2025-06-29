#ifndef MOVIE_BOOKING_STRUCTS_H
#define MOVIE_BOOKING_STRUCTS_H

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <ctime>
#include <map>

using namespace std;

enum class BookingStatus { Booked, Cancelled, CheckedIn };
enum class PaymentMethod { UPI, CreditCard, DebitCard, NetBanking, Cash };

struct PromoCode {
    string code;                     
    float discountPercent;          
    bool isValid;                   
    string createdByAdmin;          
    time_t expiryDate;              
    int usageLimit; 
    float minPurchaseAmount; 
    map<long, int> usagePerUser;   
};

struct MovieRating {
    float avgRating = 0.0;
    map<long, int> userRatings;
    map<long, string> userComments;
};

struct User {
    long u_id;
    string type;
    string phoneno;
    string name;
    string email;
    string hashPassword;
    bool isAdmin;
    vector<long> bookingHistory;
    float walletBalance = 0.0f;     
    int loyaltyPoints = 0;
    string membershipLevel = "Regular"; 
};

struct MovieTheater {
    string name;
    string location;
    string pincode;
    int totalSeats;
    vector<string> seatLayout;
    map<string, map<string, bool>> seatAvailabilityPerShowTime;
    float serviceCharge = 30.0f;
};

struct Movie {
    long m_id;
    string name;
    string genre;
    string language;
    float duration;
    float ticketPrice;
    string seat_type;
    string description;
    int ageLimit;
    string releaseDate;
    set<string> showTimings;
    map<string, int> seatsRemainingPerTiming;
    map<string, float> seatTypeToPrice; 
    vector<string> theatersAvailable;
    MovieRating ratingInfo;
};

struct Show {
    string showId;
    string movieName;
    string theaterName;
    string showTiming;
    float price;
    map<string, bool> seatAvailability;
};

struct Booking {
    long b_id;
    long m_id;
    long u_id;
    int seatsBooked;
    float baseAmount;
    float tax;
    float serviceCharge;
    float discountAmount;
    float totalAmount;
    vector<string> seatNumbers;
    time_t bookingTime;
    string bookingDate;
    string showTime;
    string movieTheater;
    string transactionId;
    BookingStatus status;
    PaymentMethod paymentMethod;
    string appliedPromoCode;
    bool usedWalletBalance = false; 
    bool isRefunded = false;      
    float refundAmount = 0.0f;    
    string refundReason;          
    time_t cancellationTime;   
};

struct Analytics {
    map<long, int> ticketsSoldPerMovie;
    map<string, float> revenuePerTheater;
    map<long, float> revenuePerMovie;
};

struct BookingLog {
    long bookingId;
    long userId;
    string action;
    time_t timestamp;
};

struct AdminLog {
    long adminId;
    string action;        
    time_t timestamp;
    string targetEntity;   
};

#endif 
