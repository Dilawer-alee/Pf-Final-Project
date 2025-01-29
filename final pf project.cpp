#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>  

using namespace std;

struct User {
    string username, password;
};

struct Ticket {
    string movieName, username, showTime;
    int seatNumber, price;
};

vector<User> users;
vector<Ticket> tickets;
vector<string> movies = {"Avatar", "Avenger", "Inception", "Titanic", "bajrangi"};
vector<string> showTimes = {"12:00 PM", "3:00 PM", "6:00 PM", "9:00 PM"};
vector<int> prices = {500, 500, 500, 1000};  // Default prices for the times: "12:00 PM", "3:00 PM", "6:00 PM", "9:00 PM"

void saveUsers() {
    try {
        ofstream file("users.txt");
        if (!file) throw runtime_error("Failed to open users file for writing.");
        
        for (const auto &user : users) {
            file << user.username << " " << user.password << endl;
        }
        file.close();
    } catch (const runtime_error &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void loadUsers() {
    try {
        ifstream file("users.txt");
        if (!file) throw runtime_error("Failed to open users file for reading.");
        
        User user;
        while (file >> user.username >> user.password) {
            users.push_back(user);
        }
        file.close();
    } catch (const runtime_error &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void saveTickets() {
    try {
        ofstream file("tickets.txt");
        if (!file) throw runtime_error("Failed to open tickets file for writing.");
        
        for (const auto &ticket : tickets) {
            file << ticket.username << " " << ticket.movieName << " " << ticket.showTime << " "
                 << ticket.seatNumber << " " << ticket.price << endl;
        }
        file.close();
    } catch (const runtime_error &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void loadTickets() {
    try {
        ifstream file("tickets.txt");
        if (!file) throw runtime_error("Failed to open tickets file for reading.");
        
        Ticket ticket;
        while (file >> ticket.username >> ticket.movieName >> ticket.showTime >> ticket.seatNumber >> ticket.price) {
            tickets.push_back(ticket);
        }
        file.close();
    } catch (const runtime_error &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void registerUser() {
    try {
        User newUser;
        cout << "Enter Username: ";
        cin >> newUser.username;
        cout << "Enter Password: ";
        cin >> newUser.password;
        users.push_back(newUser);
        saveUsers();
        cout << "Registration Successful!\n";
    } catch (const exception &e) {
        cout << "Error during registration: " << e.what() << endl;
    }
}

bool loginUser(string &currentUser) {
    try {
        string username, password;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        for (auto &user : users) {
            if (user.username == username && user.password == password) {
                cout << "Login Successful!\n";
                currentUser = username;
                return true;
            }
        }
        cout << "Invalid Credentials!\n";
        return false;
    } catch (const exception &e) {
        cout << "Error during login: " << e.what() << endl;
        return false;
    }
}

void displayMovies() {
    cout << "\nAvailable Movies:\n";
    for (int i = 0; i < movies.size(); i++) {
        cout << i + 1 << ". " << movies[i] << endl;
    }
    cout << "\nAvailable Show Times:\n";
    for (int i = 0; i < showTimes.size(); i++) {
        cout << i + 1 << ". " << showTimes[i] << endl;
    }
}

void bookTicket(string username) {
    try {
        displayMovies();

        int movieChoice, showTimeChoice, seatNumber;
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

        // Use the price from the 'prices' vector based on the selected showtime
        int price = prices[showTimeChoice - 1];

        Ticket newTicket = {movies[movieChoice - 1], username, showTimes[showTimeChoice - 1], seatNumber, price};
        tickets.push_back(newTicket);
        saveTickets();

        cout << "\nTicket Booked Successfully!\n";
        cout << "Movie: " << movies[movieChoice - 1] << " | Time: " << showTimes[showTimeChoice - 1]
             << " | Seat: " << seatNumber << " | Price: PKR " << price << endl;
    } catch (const exception &e) {
        cout << "Error during ticket booking: " << e.what() << endl;
    }
}

void viewTickets(string username) {
    try {
        bool found = false;
        cout << "\nYour Booked Tickets:\n";
        cout << left << setw(15) << "Movie" << setw(15) << "Show Time" << setw(10) << "Seat" << "Price\n";
        for (auto &ticket : tickets) {
            if (ticket.username == username) {
                found = true;
                cout << left << setw(15) << ticket.movieName << setw(15) << ticket.showTime << setw(10)
                     << ticket.seatNumber << "PKR " << ticket.price << endl;
            }
        }
        if (!found) {
            cout << "No Tickets Found!\n";
        }
    } catch (const exception &e) {
        cout << "Error during viewing tickets: " << e.what() << endl;
    }
}

void cancelTicket(string username) {
    try {
        string movieName, showTime;
        int seatNumber;
        bool found = false;

        cout << "Enter Movie Name: ";
        cin.ignore();
        getline(cin, movieName);
        cout << "Enter Show Time: ";
        getline(cin, showTime);
        cout << "Enter Seat Number: ";
        cin >> seatNumber;

        for (auto it = tickets.begin(); it != tickets.end(); ++it) {
            if (it->username == username && it->movieName == movieName && it->showTime == showTime && it->seatNumber == seatNumber) {
                tickets.erase(it);
                saveTickets();
                found = true;
                cout << "\nTicket Canceled Successfully!\n";
                break;
            }
        }

        if (!found) {
            cout << "No Matching Ticket Found!\n";
        }
    } catch (const exception &e) {
        cout << "Error during ticket cancellation: " << e.what() << endl;
    }
}

void manageMovies() {
    try {
        cout << "\n=== Admin Panel ===\n";
        cout << "1. Add Movie\n2. View Movies\n3. Adjust Ticket Prices\nEnter Choice: ";
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
        } else if (choice == 3) {
            // Adjust ticket prices
            for (int i = 0; i < showTimes.size(); i++) {
                cout << "\nCurrent price for " << showTimes[i] << ": PKR " << prices[i] << endl;
                cout << "Enter new price for " << showTimes[i] << ": ";
                cin >> prices[i];
                cout << "Price updated for " << showTimes[i] << " to PKR " << prices[i] << endl;
            }
        } else {
            cout << "Invalid Choice!\n";
        }
    } catch (const exception &e) {
        cout << "Error during movie management: " << e.what() << endl;
    }
}

void displayLoggedInMenu(string &currentUser) {
    int choice;
    while (true) {
        cout << "\n=== Movie Ticket Booking System ===\n";
        cout << "1. Book Ticket\n2. View Tickets\n3. Cancel Ticket\n4. Admin Panel\n5. Logout\n";
        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bookTicket(currentUser);
                break;
            case 2:
                viewTickets(currentUser);
                break;
            case 3:
                cancelTicket(currentUser);
                break;
            case 4:
                manageMovies();
                break;
            case 5:
                currentUser = ""; // Logout
                cout << "Logged out successfully!\n";
                return;
            default:
                cout << "Invalid Choice! Try Again.\n";
        }
    }
}

int main() {
    try {
        loadUsers();
        loadTickets();
        int choice;
        string currentUser;

        while (true) {
            cout << "\n=== Movie Ticket Booking System ===\n";
            cout << "1. Register\n2. Login\n3. Exit\n";
            cout << "Enter Your Choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    if (loginUser(currentUser)) {
                        displayLoggedInMenu(currentUser);
                    }
                    break;
                case 3:
                    exit(0);
                default:
                    cout << "Invalid Choice! Try Again.\n";
            }
        }
    } catch (const exception &e) {
        cout << "Unexpected Error: " << e.what() << endl;
    }

    return 0;
}