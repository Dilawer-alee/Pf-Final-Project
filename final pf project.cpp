
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// Class for User Management
class User {
public:
    string username, password;
    void registerUser();
    bool loginUser();
};

// Class for Movie Ticket Booking
class MovieBooking {
private:
    struct Ticket {
        string movieName, username, showTime;
        int seatNumber, price;
    };
    vector<string> movies = {"Avatar", "Avengers", "Inception", "Titanic", "Joker"};
    vector<string> showTimes = {"12:00 PM", "3:00 PM", "6:00 PM", "9:00 PM"};

public:
    void displayMovies();
    void bookTicket(string username);
    void viewTickets(string username);
    void cancelTicket(string username);
    void manageMovies();
};

// User Registration
void User::registerUser() {
    ofstream file("users.txt", ios::app);
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;
    file << username << " " << password << endl;
    file.close();
    cout << "Registration Successful!\n";
}

// User Login
bool User::loginUser() {
    ifstream file("users.txt");
    string user, pass;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    while (file >> user >> pass) {
        if (user == username && pass == password) {
            cout << "Login Successful!\n";
            return true;
        }
    }
    file.close();
    cout << "Invalid Credentials!\n";
    return false;
}

// Display Movies and Showtimes
void MovieBooking::displayMovies() {
    cout << "\nAvailable Movies:\n";
    for (int i = 0; i < movies.size(); i++) {
        cout << i + 1 << ". " << movies[i] << endl;
    }
    cout << "\nAvailable Show Times:\n";
    for (int i = 0; i < showTimes.size(); i++) {
        cout << i + 1 << ". " << showTimes[i] << endl;
    }
}

// Book a Ticket
void MovieBooking::bookTicket(string username) {
    displayMovies();

    int movieChoice, showTimeChoice, seatNumber, price;
    cout << "\nSelect Movie (1-" << movies.size() << "): ";
    cin >> movieChoice;

    if (movieChoice < 1 || movieChoice > movies.size()) {
        cout << "Invalid Movie Choice!\n";
        return;
    }

    cout << "Select Show Time (1-" << showTimes.size() << "): ";
    cin >> showTimeChoice;

    if (showTimeChoice < 1 || showTimeChoice > showTimes.size()) {
        cout << "Invalid Show Time!\n";
        return;
    }

    cout << "Enter Seat Number (1-100): ";
    cin >> seatNumber;

    if (seatNumber < 1 || seatNumber > 100) {
        cout << "Invalid Seat Number!\n";
        return;
    }

    cout << "Enter Ticket Price: ";
    cin >> price;

    ofstream file("tickets.txt", ios::app);
    file << username << " " << movies[movieChoice - 1] << " " 
         << showTimes[showTimeChoice - 1] << " " << seatNumber << " " << price << endl;
    file.close();

    cout << "\nTicket Booked Successfully!\n";
    cout << "Movie: " << movies[movieChoice - 1] << " | Time: " << showTimes[showTimeChoice - 1]
         << " | Seat: " << seatNumber << " | Price: $" << price << endl;
}

// View Booked Tickets
void MovieBooking::viewTickets(string username) {
    ifstream file("tickets.txt");
    string user, movieName, showTime;
    int seatNumber, price;
    bool found = false;

    cout << "\nYour Booked Tickets:\n";
    cout << left << setw(15) << "Movie" << setw(15) << "Show Time" << setw(10) << "Seat" << "Price\n";
    while (file >> user >> movieName >> showTime >> seatNumber >> price) {
        if (user == username) {
            found = true;
            cout << left << setw(15) << movieName << setw(15) << showTime << setw(10)
                 << seatNumber << "$" << price << endl;
        }
    }
    file.close();
    if (!found) {
        cout << "No Tickets Found!\n";
    }
}

// Cancel Ticket
void MovieBooking::cancelTicket(string username) {
    ifstream file("tickets.txt");
    ofstream temp("temp.txt");

    string user, movieName, showTime;
    int seatNumber, price;
    bool found = false;

    cout << "Enter Movie Name: ";
    cin.ignore();
    getline(cin, movieName);
    cout << "Enter Show Time: ";
    getline(cin, showTime);
    cout << "Enter Seat Number: ";
    cin >> seatNumber;

    while (file >> user >> movieName >> showTime >> seatNumber >> price) {
        if (user == username && movieName == movieName && showTime == showTime && seatNumber == seatNumber) {
            found = true;
            cout << "\nTicket Canceled Successfully!\n";
        } else {
            temp << user << " " << movieName << " " << showTime << " " << seatNumber << " " << price << endl;
        }
    }

    file.close();
    temp.close();
    remove("tickets.txt");
    rename("temp.txt", "tickets.txt");

    if (!found) {
        cout << "No Matching Ticket Found!\n";
    }
}

// Admin Manage Movies
void MovieBooking::manageMovies() {
    cout << "\n=== Admin Panel ===\n";
    cout << "1. Add Movie\n2. View Movies\nEnter Choice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        string newMovie;
        cout << "Enter New Movie Name: ";
        cin.ignore();
        getline(cin, newMovie);
        movies.push_back(newMovie);
        cout << "Movie Added Successfully!\n";
    } else if (choice == 2) {
        displayMovies();
    } else {
        cout << "Invalid Choice!\n";
    }
}

// Main Menu
int main() {
    User user;
    MovieBooking booking;
    int choice;

    while (true) {
        cout << "\n=== Movie Ticket Booking System ===\n";
        cout << "1. Register\n2. Login\n3. Book Ticket\n4. View Tickets\n5. Cancel Ticket\n6. Admin Panel\n7. Exit\n";
        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                user.registerUser();
                break;
            case 2:
                if (user.loginUser()) break;
                else continue;
            case 3:
                booking.bookTicket(user.username);
                break;
            case 4:
                booking.viewTickets(user.username);
                break;
            case 5:
                booking.cancelTicket(user.username);
                break;
            case 6:
                booking.manageMovies();
                break;
            case 7:
                exit(0);
            default:
                cout << "Invalid Choice! Try Again.\n";
        }
    }
}