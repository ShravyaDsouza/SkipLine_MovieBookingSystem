g++ src/*.cpp -Iheaders -std=c++17 -Wall -o bookingApp
./bookingApp

Optional Enhancements (for later-Attempts in main.cpp):
Feature	Example
🕓 Sleep after 3 attempts	sleep(5); to slow brute force
🔐 Lockout for session	Block email temporarily in memory
📄 Persistent lockout	Store blacklisted emails in a file
🔔 Admin alert	Log alert: ALERT: repeated failed login for email ...

🔁 Optional Enhancements (For Future - MovieManager.h)
Filter by show timing availability
Integrate theater location filter
Support sorting by popularity/rating
Return optional<Movie> (instead of raw pointer)

| Feature                                     | Status |
| ------------------------------------------- | ------ |
| User/Admin Registration & Login             | ✅ Done |
| Password hashing & validation               | ✅ Done |
| Logger with success/failure events          | ✅ Done |
| Movie CRUD (add, update, delete)            | ✅ Done |
| Rating & Review system (per user per movie) | ✅ Done |
| View all ratings/comments for a movie       | ✅ Done |
| Admin moderation of reviews                 | ✅ Done |
| Promo codes & discount logic                | ✅ Done |
| Booking system with tax, discount, seats    | ✅ Done |
| Booking cancellation                        | ✅ Done |
| CSV import/export for movies                | ✅ Done |
| Search movies by genre/language/name        | ✅ Done |
| Filter by timing and theater location       | ✅ Done |
| Sort movies by rating                       | ✅ Done |
| Lockout after 3 failed login attempts       | ✅ Done |
| Show filtered messages and logs             | ✅ Done |
| Analytics reports for admin                 | ✅ Done |
| Admin: View all bookings/users/movies       | ✅ Done |

| Feature                                                                            | Status     |
| ---------------------------------------------------------------------------------- | ---------- |
| Allow users to view **all their bookings**                                         | 🔄 Working |
| Allow users to **modify booking** if the show is more than 5 hours away            | 🔄 Working |
| Allow admin to **set ticket price** while adding movie (instead of hardcoded ₹150) | 🔄 Working |
| Ensure `ticketPrice` is fetched from `Movie` struct in BookingManager              | 🔄 Working |


✅ Admin Features to Test (Checklist)
✅ Verified & Confirmed Sections
🎬 Movie Management
 Add / Update / Delete movies

 View all movies

 Import/Export movies from CSV

 Moderate reviews & ratings (View/Delete)

 Filter by show timings

 Filter by theater location

 Sort by popularity/rating

 Support multiple formats (optional, if implemented)

✅ Nothing missing here.

🏟 Theater Management
 Add / Update / Delete theaters

 Seat pricing & service charge (per-theater customization)

 Manage seat layout & capacity

 View/Delete reviews (optional)

✅ Complete for all standard use cases.

💳 Promo Code Management
 Add, View, Remove promo codes

 Validate during booking

 Auto-consume after booking

 Promo usage limit or expiry (if you’ve implemented this)

✅ Optional edge-case: limit redemptions per user or total.

📈 Analytics & Reports
 Top movies by bookings

 Revenue by movie, theater

 Ticket sales stats

 Load/save analytics

 Reset analytics

✅ Looks complete here.

🎟 Booking Management (Admin View)
 View all bookings (by user, movie, date, etc.)

 Cancel or modify bookings (with time-based rules)

 Manual check-in

 Lookup booking by ID

✅ Optional enhancement: export bookings CSV.

👤 User Management
 View all users

 Blacklist/delete users

 View user history

✅ Well covered.

⚙️ Miscellaneous
 Logout from admin

 System log / audit (if Logger.h is integrated)
